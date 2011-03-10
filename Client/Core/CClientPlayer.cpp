//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPlayer.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#define PASSENGER_KEY_HOLD_TIME 1000 // 1 Second

extern CClient * g_pClient;

CClientPlayer::CClientPlayer(bool bIsLocalPlayer) : CStreamableEntity(g_pClient->GetStreamer(), ENTITY_TYPE_PLAYER, 200.0f)
{
	m_bIsLocalPlayer = bIsLocalPlayer;
	m_playerId = INVALID_ENTITY_ID;
	m_pContextData = NULL;
	memset(&m_previousNetPadState, 0, sizeof(CNetworkPadState));
	memset(&m_currentNetPadState, 0, sizeof(CNetworkPadState));
	m_interp.pos.ulFinishTime = 0;
	m_uiHealth = 0;
	m_dwWeaponType = 0;
	m_dwAmmo = 0;
	m_pVehicle = NULL;
	ResetVehicleEnterExit();

	if(bIsLocalPlayer)
	{
		// Set the player index to local player index
		m_byteInternalPlayerNumber = (BYTE)CPools::GetLocalPlayerIndex();
		CLogFile::Printf("CClientPlayer 1\n");

		// Create the player ped instance with the local player ped pointer
		m_pPlayerPed = new CIVPlayerPed(CPools::GetPlayerInfoFromIndex(0)->m_pPlayerPed);
		CLogFile::Printf("CClientPlayer 2\n");

		// Create the player info instance with the local player info pointer
		m_pPlayerInfo = new CIVPlayerInfo(CPools::GetPlayerInfoFromIndex(0));
		CLogFile::Printf("CClientPlayer 3\n");

		// Set the model hash to the default player model hash
		m_dwModelHash = MODEL_PLAYER;
	}
	else
	{
		// Invalidate the player number
		m_byteInternalPlayerNumber = INVALID_PLAYER_PED;

		// Set the player ped instance to NULL
		m_pPlayerPed = NULL;

		// Set the player info instance to NULL
		m_pPlayerInfo = NULL;

		// Set the model hash to the default player model hash
		m_dwModelHash = MODEL_PLAYER;
	}
}

CClientPlayer::~CClientPlayer()
{
	// Notify the stream that we have been deleted
	OnDelete();
}

// TODO: Use this to create local player ped too instead of using scripting natives?
bool CClientPlayer::Create()
{
	// Are we not already created?
	if(!IsSpawned())
	{
		// Find a free player number
		m_byteInternalPlayerNumber = (BYTE)CPools::FindFreePlayerInfoIndex();

		CLogFile::Printf("PlayerNumber: %d\n", m_byteInternalPlayerNumber);

		// Invalid player number?
		if(m_byteInternalPlayerNumber == INVALID_PLAYER_PED)
			return false;

		// Load model (if needed) and get model index
		int iModelIndex = g_pClient->GetGame()->LoadModel(m_dwModelHash);

		// Invalid model hash?
		if(iModelIndex == -1)
			return false;

		// Patch to allow us to create like local player (Only needed if player data is MAKEWORD(1, 1), 
		// (which crashes anyways))
		//CPatcher::InstallNopPatch((g_pClient->GetBaseAddress()+ 0x81CD52), 2);

		// Patch for something(?)
		//*(BYTE *)(g_pClient->GetBaseAddress() + 0x97C283) = 0xEB;

		// Create player info instance
		m_pPlayerInfo = new CIVPlayerInfo(m_byteInternalPlayerNumber);

		// Create a context data instance for this player
		m_pContextData = CContextDataManager::CreateContextData(m_pPlayerInfo);

		// Create player ped
#define FUNC_CreatePlayerPed 0x96D870
		unsigned int uiPlayerIndex = (unsigned int)m_byteInternalPlayerNumber;
		WORD wPlayerData = MAKEWORD(0, 1);
		WORD * pwPlayerData = &wPlayerData;
		IVPlayerPed * pPlayerPed = NULL;
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CreatePlayerPed);
		_asm
		{
			push uiPlayerIndex
			push iModelIndex
			push pwPlayerData
			call dwFunc
			mov pPlayerPed, eax
		}

		// Ensure the player ped pointer is valid
		if(!pPlayerPed)
		{
			Destroy();
			return false;
		}

		CLogFile::Printf("Player info for player ped 0x%p, our player info 0x%p\n", pPlayerPed->m_pPlayerInfo, m_pPlayerInfo->GetPlayerInfo());

		// Setup the player ped
#define VAR_PedFactory 0x15E35A0
#define FUNC_SetupPed 0x43A6A0
		DWORD dwPedFactory = (g_pClient->GetBaseAddress() + VAR_PedFactory);
		Matrix34 * pMatrix = NULL;
		dwFunc = (g_pClient->GetBaseAddress() + FUNC_SetupPed);
		_asm
		{
			push iModelIndex
			push dwPedFactory
			mov edi, pMatrix
			mov esi, pPlayerPed
			call dwFunc
		}

		*(DWORD *)(pPlayerPed + 0x260) |= 1u;

		// Setup the player ped intelligence
#define FUNC_SetupPedIntelligence 0x89EC20
		dwFunc = (g_pClient->GetBaseAddress() + FUNC_SetupPedIntelligence);
		_asm
		{
			push 2
			mov ecx, pPlayerPed
			call dwFunc
		}

		//*(DWORD *)(pPlayerInfo + 0x4DC) = 2;

		// Set our player infos ped pointer
		m_pPlayerInfo->SetPlayerPed(pPlayerPed);

		// Set our player peds player info pointer
		pPlayerPed->m_pPlayerInfo = m_pPlayerInfo->GetPlayerInfo();

		// Set game player info pointer
		CPools::SetPlayerInfoAtIndex((unsigned int)m_byteInternalPlayerNumber, m_pPlayerInfo->GetPlayerInfo());

		// Create player ped instance
		m_pPlayerPed = new CIVPlayerPed(pPlayerPed);

		// Set the context data player ped pointer
		m_pContextData->SetPlayerPed(m_pPlayerPed);

		// Add to world
		m_pPlayerPed->AddToWorld();

		// TODO: Assign initial task?

		// Recreate the ped
		//SetModelByHash(MODEL_PLAYER);

		// Set initial health
		SetHealth(200);

		// Set initial position
		Teleport(Vector3());

		// Set initial rotation
		SetRotation(Vector3());

		CLogFile::Printf("Done: PlayerNumber: %d, ScriptingHandle: %d\n", m_byteInternalPlayerNumber, GetScriptingHandle());
		return true;
	}

	// Already created
	return false;
}

void CClientPlayer::Destroy()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Get the player ped pointer
		IVPlayerPed * pPlayerPed = m_pPlayerPed->GetPlayerPed();

		IVPedIntelligence * pPedIntelligence = pPlayerPed->m_pPedIntelligence;
#define FUNC_ShutdownPedIntelligence 0x9C4DF0
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_ShutdownPedIntelligence);
		_asm
		{
			push 0
			mov ecx, pPedIntelligence
			call dwFunc
		}

		*(DWORD *)(pPlayerPed + 0x260) &= 0xFFFFFFFEu;

		// Remove the player ped from the world
		m_pPlayerPed->RemoveFromWorld();

		// Delete the player ped
		// We use the CPed destructor and not the CPlayerPed destructor because the CPlayerPed destructor
		// messes with our player info (which we handle manually)
		//DWORD dwFunc = m_pPlayerPed->GetPlayerPed()->m_VFTable->ScalarDeletingDestructor;
#define FUNC_CPed__ScalarDeletingDestructor 0x8ACAC0
		dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPed__ScalarDeletingDestructor);
		_asm
		{
			push 1
			mov ecx, pPlayerPed
			call dwFunc
		}
	}

	// Do we have a context data instance
	if(m_pContextData)
	{
		// Delete the context data instance
		CContextDataManager::DestroyContextData(m_pContextData);

		// Set the context data pointer to NULL
		m_pContextData = NULL;
	}

	// Delete the player ped instance
	SAFE_DELETE(m_pPlayerPed);

	// Delete our player info instance
	SAFE_DELETE(m_pPlayerInfo);

	// Do we have a valid player number?
	if(m_byteInternalPlayerNumber != INVALID_PLAYER_PED)
	{
		// Reset game player info pointer
		CPools::SetPlayerInfoAtIndex((unsigned int)m_byteInternalPlayerNumber, NULL);

		// Invalidate the player number
		m_byteInternalPlayerNumber = INVALID_PLAYER_PED;
	}
}

void CClientPlayer::StreamIn()
{
	// Are we not the local player?
	if(!IsLocalPlayer())
	{
		// Attempt to create the player
		if(Create())
		{
			// Set the position
			Teleport(m_vecPosition);

			// Set the rotation
			SetRotation(m_vecRotation);

			// Set the move speed
			SetMoveSpeed(m_vecMoveSpeed);

			// Set the turn speed
			SetTurnSpeed(m_vecTurnSpeed);

			// Set the health
			SetHealth(m_uiHealth);

			// Set the current weapon
			GiveWeapon(m_dwWeaponType, m_dwAmmo);
			SetCurrentWeapon(m_dwWeaponType);
		}
	}
}

void CClientPlayer::StreamOut()
{
	// Are we not the local player?
	if(!IsLocalPlayer())
	{
		// Get the position
		GetPosition(m_vecPosition);

		// Get the rotation
		GetRotation(m_vecRotation);

		// Get the move speed
		GetMoveSpeed(m_vecMoveSpeed);

		// Get the turn speed
		GetTurnSpeed(m_vecTurnSpeed);

		// Get the health
		m_uiHealth = GetHealth();

		// Get the current weapon
		m_dwWeaponType = GetCurrentWeaponType();
		m_dwAmmo = GetWeaponAmmo(m_dwWeaponType);

		// Destroy the player
		Destroy();
	}
}

void CClientPlayer::Process()
{
	if(IsSpawned())
	{
		// Process vehicle entry/exit
		ProcessVehicleEntryExit();

		if(IsLocalPlayer())
		{
			// If input is enabled check vehicle entry/exit key
			//if(CGame::GetInputState())
				CheckVehicleEntryExitKey();

			// Copy the current net pad state to the previous net pad state
			memcpy(&m_previousNetPadState, &m_currentNetPadState, sizeof(CNetworkPadState));

			// Update the current net pad state
			CClientPadState padState;
			GetGamePadState(&padState);
			padState.ToNetPadState(m_currentNetPadState, !IsInVehicle());
		}
		else
		{
			// Are we not in a vehicle?
			if(!IsInVehicle())
			{
				// Process interpolation
				Interpolate();
			}
		}
	}
}

unsigned int CClientPlayer::GetScriptingHandle()
{
	if(IsSpawned())
		return CPools::GetHandleFromPed(m_pPlayerPed->GetPed());

	return 0;
}

bool CClientPlayer::InternalIsInVehicle()
{
	if(IsSpawned())
		return (m_pPlayerPed->IsInVehicle() && m_pPlayerPed->GetCurrentVehicle());

	return false;
}

CClientVehicle * CClientPlayer::InternalGetVehicle()
{
	if(IsSpawned() && IsInVehicle())
		//return m_pPlayerPed->GetCurrentVehicle();
		// TODO: We need our own pool system to store our class created players/vehicle/e.t.c.
		return NULL;

	return NULL;
}

bool CClientPlayer::SetModelByHash(DWORD dwModelHash)
{
	// Get the model index from the hash
	int iModelIndex = g_pClient->GetGame()->GetModelIndexFromHash(dwModelHash);

	// Is the model index valid?
	if(iModelIndex != -1)
	{
		// Set the model hash
		m_dwModelHash = dwModelHash;

		// TODO: Find a proper way to do this (this native just destroys/recreates the ped)
		InvokeNative<void *>(NATIVE_CHANGE_PLAYER_MODEL, (unsigned int)m_byteInternalPlayerNumber, dwModelHash);
		m_pPlayerPed->SetPlayerPed(m_pPlayerInfo->GetPlayerPed());
		InvokeNative<void *>(NATIVE_SET_CHAR_DEFAULT_COMPONENT_VARIATION, GetScriptingHandle());
		return true;
	}

	// Invalid model hash
	return false;
}

DWORD CClientPlayer::GetModelHash()
{
	// TODO: CGame::GetModelHashFromIndex and use IVEntity::m_wModelIndex;
	return m_dwModelHash;
}

void CClientPlayer::SetNetPadState(const CNetworkPadState& netPadState)
{
	// Copy the current net pad state to the previous net pad state
	memcpy(&m_previousNetPadState, &m_currentNetPadState, sizeof(CNetworkPadState));

	// Copy the net pad state to the current net pad state
	memcpy(&m_currentNetPadState, &netPadState, sizeof(CNetworkPadState));

	// Get the appropriate pad state
	CClientPadState padState;

	// Is this the local player?
	if(IsLocalPlayer())
	{
		// Get the current pad state
		GetGamePadState(&padState);
	}
	else
	{
		// Get the player pad state
		if(m_pContextData)
			memcpy(&padState, m_pContextData->GetPadState(), sizeof(CClientPadState));
	}

	// Copy the net pad state to the pad state
	padState.Invalidate();
	padState.FromNetPadState(netPadState, !IsInVehicle());

	// Set the appropriate pad state
	// Is this the local player?
	if(IsLocalPlayer())
	{
		// Set the current pad state
		SetGamePadState(&padState);
	}
	else
	{
		// Set the player pad state
		if(m_pContextData)
			memcpy(m_pContextData->GetPadState(), &padState, sizeof(CClientPadState));
	}
}

void CClientPlayer::GetPreviousNetPadState(CNetworkPadState& netPadState)
{
	// Copy the previous net pad state to the net pad state
	memcpy(&netPadState, &m_previousNetPadState, sizeof(CNetworkPadState));
}

void CClientPlayer::GetNetPadState(CNetworkPadState& netPadState)
{
	// Copy the current net pad state to the net pad state
	memcpy(&netPadState, &m_currentNetPadState, sizeof(CNetworkPadState));
}

void CClientPlayer::SetAimData(const Matrix& matAim)
{
	if(IsLocalPlayer())
		SetGameCameraMatrix((Matrix *)&matAim);
	else
	{
		if(m_pContextData)
			memcpy(m_pContextData->GetCameraMatrix(), (Matrix *)&matAim, sizeof(Matrix));
	}
}

void CClientPlayer::GetAimData(Matrix& matAim)
{
	if(IsLocalPlayer())
		GetGameCameraMatrix(&matAim);
	else
	{
		if(m_pContextData)
			memcpy(&matAim, m_pContextData->GetCameraMatrix(), sizeof(Matrix));
		else
			matAim.Identity();
	}
}

void CClientPlayer::SetHealth(unsigned int uiHealth)
{
	if(IsSpawned())
		InvokeNative<void *>(NATIVE_SET_CHAR_HEALTH, GetScriptingHandle(), uiHealth);

	m_uiHealth = uiHealth;
}

unsigned int CClientPlayer::GetHealth()
{
	if(IsSpawned())
	{
		unsigned int uiHealth;
		InvokeNative<void *>(NATIVE_GET_CHAR_HEALTH, GetScriptingHandle(), &uiHealth);
		return uiHealth;
	}

	return m_uiHealth;
}

// TODO: CClientPlayer::WarpFromVehicleToPosition

void CClientPlayer::Teleport(const Vector3& vecPosition)
{
	if(IsSpawned())
	{
		// Teleport the player ped
		// TODO: Use reversed code from this native
		InvokeNative<void *>(NATIVE_SET_CHAR_COORDINATES, GetScriptingHandle(), vecPosition.fX, vecPosition.fY, vecPosition.fZ);
	}

	m_vecPosition = vecPosition;
}

void CClientPlayer::SetPosition(const Vector3& vecPosition)
{
	// TODO: Use reversed code from SET_CHAR_COORDINATES native to fix this
	// (atm it doesn't work properly)
	if(IsSpawned())
	{
		// Remove the player ped from the world
		m_pPlayerPed->RemoveFromWorld();

		// Set the position in the player ped matrix
		m_pPlayerPed->SetPosition((Vector3 *)&vecPosition);

		// Re-add the player ped to the world
		m_pPlayerPed->AddToWorld();
	}

	m_vecPosition = vecPosition;
}

void CClientPlayer::GetPosition(Vector3& vecPosition)
{
	if(IsSpawned())
		m_pPlayerPed->GetPosition(&vecPosition);
	else
		vecPosition = m_vecPosition;
}

void CClientPlayer::SetRotation(const Vector3& vecRotation)
{
	if(IsSpawned())
	{
		// Remove the player ped from the world
		m_pPlayerPed->RemoveFromWorld();

		// Get the player ped matrix
		Matrix matMatrix;
		m_pPlayerPed->GetMatrix(&matMatrix);

		// Convert the rotation to radians and apply it to the player ped matrix
		Vector3 vecNewRotation = ((Vector3)vecRotation).ToRadians();
		g_pClient->GetGame()->ConvertEulerAnglesToRotationMatrix(&/*vecRotation.ToRadians()*/vecNewRotation, &matMatrix);

		// Set the new player ped matrix
		m_pPlayerPed->SetMatrix(&matMatrix);

		// Re-add the player ped to the world
		m_pPlayerPed->AddToWorld();
	}

	m_vecRotation = vecRotation;
}

void CClientPlayer::GetRotation(Vector3& vecRotation)
{
	if(IsSpawned())
	{
		// Get the player ped matrix
		Matrix matMatrix;
		m_pPlayerPed->GetMatrix(&matMatrix);

		// Convert the matrix to euler angles
		g_pClient->GetGame()->ConvertRotationMatrixToEulerAngles(&matMatrix, &vecRotation);

		// Convert the rotation from radians to degrees
		vecRotation = vecRotation.ToDegrees();
	}
	else
		vecRotation = m_vecRotation;
}

void CClientPlayer::SetMoveSpeed(const Vector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->SetMoveSpeed((Vector3 *)&vecMoveSpeed);

	m_vecMoveSpeed = vecMoveSpeed;
}

void CClientPlayer::GetMoveSpeed(Vector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->GetMoveSpeed(&vecMoveSpeed);
	else
		vecMoveSpeed = m_vecMoveSpeed;
}

void CClientPlayer::SetTurnSpeed(const Vector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->SetTurnSpeed((Vector3 *)&vecTurnSpeed);

	m_vecTurnSpeed = vecTurnSpeed;
}

void CClientPlayer::GetTurnSpeed(Vector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->GetTurnSpeed(&vecTurnSpeed);
	else
		vecTurnSpeed = m_vecTurnSpeed;
}

void CClientPlayer::GiveWeapon(DWORD dwWeaponType, DWORD dwAmmo)
{
	if(IsSpawned())
	{
		// Last param could be equip as soon as weapon model is loaded?
		InvokeNative<void *>(NATIVE_GIVE_WEAPON_TO_CHAR, GetScriptingHandle(), dwWeaponType, dwAmmo, false);
	}

	m_dwWeaponType = dwWeaponType;
	m_dwAmmo = dwAmmo;
}

void CClientPlayer::RemoveWeapon(DWORD dwWeaponType)
{
	if(IsSpawned())
		InvokeNative<void *>(NATIVE_REMOVE_WEAPON_FROM_CHAR, GetScriptingHandle(), dwWeaponType);

	m_dwWeaponType = 0;
}

void CClientPlayer::RemoveAllWeapons()
{
	if(IsSpawned())
		InvokeNative<void *>(NATIVE_REMOVE_ALL_CHAR_WEAPONS, GetScriptingHandle());

	m_dwWeaponType = 0;
}

void CClientPlayer::SetCurrentWeapon(DWORD dwWeaponType)
{
	if(IsSpawned())
		InvokeNative<void *>(NATIVE_SET_CURRENT_CHAR_WEAPON, GetScriptingHandle(), dwWeaponType, true);

	m_dwWeaponType = dwWeaponType;
}

eWeaponSlot CClientPlayer::GetCurrentWeaponSlot()
{
	if(IsSpawned())
		//return (eWeaponSlot)m_pPlayerPed->GetPed()->m_weapons.m_byteCurrentWeaponSlot;
		return (eWeaponSlot)0;

	return WEAPON_SLOT_UNARMED;
}

DWORD CClientPlayer::GetCurrentWeaponType()
{
	if(IsSpawned())
	{
		// FIXME
		//return m_pPlayerPed->GetPed()->m_weapons.m_weapons[GetCurrentWeaponSlot()].m_dwWeaponType;
		DWORD dwWeaponType;
		InvokeNative<void *>(NATIVE_GET_CURRENT_CHAR_WEAPON, GetScriptingHandle(), &dwWeaponType);
		return dwWeaponType;
	}

	return m_dwWeaponType;
}

void CClientPlayer::SetWeaponAmmo(DWORD dwWeaponType, DWORD dwAmmo)
{
	if(IsSpawned())
		InvokeNative<void *>(NATIVE_SET_CHAR_AMMO, GetScriptingHandle(), dwWeaponType, dwAmmo);

	m_dwAmmo = dwAmmo;
}

DWORD CClientPlayer::GetWeaponAmmo(DWORD dwWeaponType)
{
	if(IsSpawned())
	{
		// TODO: Create a function for SetAmmoInClip
		//SetAmmoInClip()
		DWORD dwAmmo;
		InvokeNative<void *>(NATIVE_GET_AMMO_IN_CHAR_WEAPON, GetScriptingHandle(), dwWeaponType, &dwAmmo);
		return dwAmmo;
	}

	return m_dwAmmo;
}

void CClientPlayer::ClearPrimaryTask(bool bImmidiately)
{
	if(IsSpawned())
	{
		// Get primary task
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		// Do we have a valid primary task?
		if(pTask)
		{
			// Abort the primary task
			pTask->MakeAbortable(m_pPlayerPed, (bImmidiately ? ABORT_PRIORITY_IMMEDIATE : ABORT_PRIORITY_LEISURE));
		}

		// Reset vehicle entry/exit flags
		ResetVehicleEnterExit();
	}
}

bool CClientPlayer::IsGettingInToAVehicle()
{
	bool bReturn = false;

	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_NEW_GET_IN_VEHICLE)
				bReturn = true;
		}
	}

	return bReturn;
}

bool CClientPlayer::IsGettingOutOfAVehicle()
{
	bool bReturn = false;

	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_COMPLEX_NEW_EXIT_VEHICLE)
				bReturn = true;
		}
	}

	return bReturn;
}

bool CClientPlayer::IsJackingAVehicle()
{
	bool bReturn = false;

	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_SIMPLE_CAR_SLOW_DRAG_OUT_PED)
				bReturn = true;
		}
	}

	return bReturn;
}

bool CClientPlayer::IsGettingJackedFromVehicle()
{
	bool bReturn = false;

	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_SIMPLE_CAR_SLOW_BE_DRAGGED_OUT)
				bReturn = true;
		}
	}

	return bReturn;
}

bool CClientPlayer::GetClosestVehicle(bool bPassenger, CClientVehicle ** pVehicle, BYTE& byteSeatId)
{
	// TODO: Get closest vehicle door not vehicle and add door parameter
	// Are we spawned?
	if(IsSpawned())
	{
		float fCurrent = 6.0f; // Maximum distance 6.0f
		Vector3 vecVehiclePos;
		CClientVehicle * pClosestVehicle = NULL;

		// Get our position
		Vector3 vecPlayerPos;
		GetPosition(vecPlayerPos);

		// Loop through all streamed in vehicles
		std::list<CStreamableEntity *> * streamedVehicles = g_pClient->GetStreamer()->GetStreamedInEntitiesOfType(ENTITY_TYPE_VEHICLE);

		for(std::list<CStreamableEntity *>::iterator iter = streamedVehicles->begin(); iter != streamedVehicles->end(); ++iter)
		{
			CClientVehicle * pTestVehicle = reinterpret_cast<CClientVehicle *>(*iter);

			// Get the vehicle position
			pTestVehicle->GetPosition(vecVehiclePos);

			// Get the distance between us and the vehicle
			float fDistance = GetDistanceBetweenPoints3D(vecPlayerPos.fX, vecPlayerPos.fY, vecPlayerPos.fZ, vecVehiclePos.fX, vecVehiclePos.fY, vecVehiclePos.fZ);

			// Is the distance less than the current distance?
			if(fDistance < fCurrent)
			{
				// Set the current distance
				fCurrent = fDistance;

				// Set the closest vehicle pointer
				pClosestVehicle = pTestVehicle;
			}
		}

		// Do we have a valid closest vehicle pointer?
		if(pClosestVehicle == NULL)
			return false;

		// Are we looking for a passenger seat?
		if(bPassenger)
		{
			// Loop through all passenger seats
			BYTE byteTestSeatId = 0;

			for(BYTE i = 0; i < pClosestVehicle->GetMaxPassengers(); i++)
			{
				// Does this passenger seat contain a passenger?
				if(!pClosestVehicle->GetPassenger(i))
				{
					byteTestSeatId = (i + 1);
					break;
				}
			}

			// Do we have a valid test seat id?
			if(byteTestSeatId == 0)
				return false;

			// Set the seat id
			byteSeatId = byteTestSeatId;
		}
		else
		{
			// Set the seat id to the driver seat
			byteSeatId = 0;
		}

		// Set the vehicle pointer
		*pVehicle = pClosestVehicle;
		return true;
	}

	return false;
}

void CClientPlayer::EnterVehicle(CClientVehicle * pVehicle, BYTE byteSeatId)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Is the vehicle invalid?
		if(!pVehicle)
			return;

		// Is the vehicle not spawned?
		if(!pVehicle->IsSpawned())
		{
			// Are we the local player?
			if(IsLocalPlayer())
			{
				// Force the vehicle to stream in
				/*g_pClient->GetStreamer()->ForceStreamIn(pVehicle, true);*/
			}
			else
				return;
		}

		// Are we already in a vehicle?
		if(IsInVehicle())
			return;

		// Are we entering as the driver?
		if(byteSeatId == 0)
		{
			// Start the enter car as driver task
			InvokeNative<void *>(TASK_ENTER_CAR_AS_DRIVER, GetScriptingHandle(), pVehicle->GetScriptingHandle(), -2);
		}
		else
		{
			// Start the enter car as passenger task
			InvokeNative<void *>(TASK_ENTER_CAR_AS_PASSENGER, GetScriptingHandle(), pVehicle->GetScriptingHandle(), -2, (byteSeatId - 1));
		}

		// Mark ourselves as entering a vehicle and store our vehicle and seat
		m_vehicleEnterExit.bEntering = true;
		m_vehicleEnterExit.pVehicle = pVehicle;
		m_vehicleEnterExit.byteSeatId = byteSeatId;

		// Reset interpolation
		ResetInterpolation();
	}
}

void CClientPlayer::ExitVehicle(bool bWarp)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Are we in a vehicle?
		if(m_pVehicle)
		{
			if(bWarp)
			{
				// Warp ourselves out of the vehicle
				Vector3 vecPos;
				m_pVehicle->GetPosition(vecPos);
				InvokeNative<void *>(WARP_CHAR_FROM_CAR_TO_COORD, GetScriptingHandle(), vecPos.fX, vecPos.fY, (vecPos.fZ + 1.0f));

				// Reset vehicle entry/exit flags
				ResetVehicleEnterExit();
			}
			else
			{
				// Start the leave car task
				InvokeNative<void *>(TASK_LEAVE_CAR, GetScriptingHandle(), m_pVehicle->GetScriptingHandle());

				// Mark ourselves as exiting a vehicle
				m_vehicleEnterExit.bExiting = true;
			}
		}
		else
		{
			// Are we entering a vehicle?
			if(HasVehicleEnterExit())
			{
				// Cancel the vehicle entry
				ClearPrimaryTask(true);
			}
		}

		// Reset interpolation
		ResetInterpolation();
	}
}

void CClientPlayer::PutInVehicle(CClientVehicle * pVehicle, BYTE byteSeatId)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Is the vehicle invalid?
		if(!pVehicle)
			return;

		// Is the vehicle not spawned?
		if(!pVehicle->IsSpawned())
		{
			// Are we the local player?
			if(IsLocalPlayer())
			{
				// Force the vehicle to stream in
				/*g_pClient->GetStreamer()->ForceStreamIn(pVehicle, true);*/
			}
			else
				return;
		}

		// Are we already in a vehicle?
		if(IsInVehicle())
		{
			// Exit our current vehicle
			ExitVehicle(true);
		}

		// Is the seat the driver seat?
		if(byteSeatId == 0)
			InvokeNative<void *>(NATIVE_WARP_CHAR_INTO_CAR, GetScriptingHandle(), pVehicle->GetScriptingHandle());
		else
		{
			// Is the passenger seat valid?
			if(byteSeatId <= pVehicle->GetMaxPassengers())
				InvokeNative<void *>(NATIVE_WARP_CHAR_INTO_CAR, GetScriptingHandle(), pVehicle->GetScriptingHandle(), (byteSeatId - 1));
		}

		// Reset vehicle entry/exit
		ResetVehicleEnterExit();
		m_pVehicle = pVehicle;
		pVehicle->SetOccupant(byteSeatId, this);
	}
}

void CClientPlayer::CheckVehicleEntryExitKey()
{
	if(IsSpawned())
	{
		// Has the enter/exit vehicle key just been pressed?
		if(!m_previousNetPadState.IsUsingEnterExitVehicle() && m_currentNetPadState.IsUsingEnterExitVehicle())
		{
			// Save the current time into the enter/exit vehicle key hold start time
			m_vehicleEnterExit.ulKeyHoldStartTime = SharedUtility::GetTime();

			if(IsLocalPlayer())
				CLogFile::Printf("EnterExitVehicleKeyHoldStart(LocalPlayer)");
			else
				CLogFile::Printf("EnterExitVehicleKeyHoldStart(%d)", m_playerId);
		}
		else
		{
			// Has the enter/exit vehicle key just been released?
			if(m_previousNetPadState.IsUsingEnterExitVehicle() && !m_currentNetPadState.IsUsingEnterExitVehicle())
			{
				if(IsLocalPlayer())
					CLogFile::Printf("EnterExitVehicleKeyHoldEnd(LocalPlayer)");
				else
					CLogFile::Printf("EnterExitVehicleKeyHoldEnd(%d)", m_playerId);

				if(IsInVehicle() && !m_vehicleEnterExit.bExiting)
				{
					if(IsLocalPlayer())
						CLogFile::Printf("HandleVehicleExit(LocalPlayer)");
					else
						CLogFile::Printf("HandleVehicleExit(%d)", m_playerId);

					// Exit the vehicle
					ExitVehicle(false);

					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send vehicle exit notification to server
						/*g_pNetworkManager->GetRakClient()->RPC(RPC_VehicleExit, NULL, HIGH_PRIORITY, RELIABLE);*/
					}
				}
				else if(!IsInVehicle() && !m_vehicleEnterExit.bEntering)
				{
					// Get the time the enter/exit vehicle key was held for
					unsigned long ulKeyHoldTime = (SharedUtility::GetTime() - m_vehicleEnterExit.ulKeyHoldStartTime);

					// Has the key been held long enough to enter as a passenger?
					if(ulKeyHoldTime >= PASSENGER_KEY_HOLD_TIME)
					{
						// Do we have a close vehicle?
						CClientVehicle * pVehicle = NULL;
						BYTE byteSeatId = 0;

						if(GetClosestVehicle(true, &pVehicle, byteSeatId))
						{
							if(IsLocalPlayer())
								CLogFile::Printf("HandleVehicleEntry(LocalPlayer, %d, %d)", pVehicle->GetVehicleId(), byteSeatId);
							else
								CLogFile::Printf("HandleVehicleEntry(%d, %d, %d)", m_playerId, pVehicle->GetVehicleId(), byteSeatId);

							// Enter the vehicle
							EnterVehicle(pVehicle, byteSeatId);

							// Is this a network vehicle?
							if(pVehicle->IsNetworkVehicle())
							{
								// Send the passenger entry to the server
								/*RakNet::BitStream bsSend;
								bsSend.Write(pVehicle->GetVehicleId());
								bsSend.Write(byteSeatId);
								g_pNetworkManager->GetRakClient()->RPC(RPC_VehicleEntry, &bsSend, HIGH_PRIORITY, RELIABLE);*/
							}
						}
					}
					else
					{
						// Key has not been held long enough to enter as a passenger, enter as driver
						// Do we have a close vehicle?
						CClientVehicle * pVehicle = NULL;
						BYTE byteSeatId = 0;

						if(GetClosestVehicle(false, &pVehicle, byteSeatId))
						{
							CLogFile::Printf("HandleVehicleEntry(LocalPlayer, %d, %d)", pVehicle->GetVehicleId(), byteSeatId);

							// Enter the vehicle
							EnterVehicle(pVehicle, byteSeatId);

							// Is this a network vehicle?
							if(pVehicle->IsNetworkVehicle())
							{
								// Send the vehicle entry to the server
								/*RakNet::BitStream bsSend;
								bsSend.Write(pVehicle->GetVehicleId());
								bsSend.Write(byteSeatId);
								g_pNetworkManager->GetRakClient()->RPC(RPC_VehicleEntry, &bsSend, HIGH_PRIORITY, RELIABLE);*/
							}
						}
					}

					// Reset the enter/exit vehicle key hold start time
					m_vehicleEnterExit.ulKeyHoldStartTime = 0;
				}
			}
		}
	}
}

void CClientPlayer::ProcessVehicleEntryExit()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Are we in a vehicle internally?
		if(InternalIsInVehicle())
		{
			// Are we flagged as entering a vehicle?
			if(m_vehicleEnterExit.bEntering)
			{
				// Have we finished our enter vehicle task?
				if(!IsGettingInToAVehicle())
				{
					// Vehicle entry is complete
					m_vehicleEnterExit.bEntering = false;
					m_pVehicle = m_vehicleEnterExit.pVehicle;
					m_pVehicle->SetOccupant(m_vehicleEnterExit.byteSeatId, this);
					m_vehicleEnterExit.pVehicle = NULL;

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleEntryComplete(LocalPlayer)");
					else
						CLogFile::Printf("VehicleEntryComplete(%d)", m_playerId);
				}
			}
		}
		else
		{
			// Are we flagged as entering a vehicle?
			if(m_vehicleEnterExit.bEntering)
			{
				// Do we no longer have our enter vehicle task?
				if(!IsGettingInToAVehicle())
				{
					// Are we the local player?
					if(IsLocalPlayer())
					{
						// NOTE: Isn't there some exit vehicle task abort event?
						// Is our enter/exit vehicle a network vehicle?
						if(m_vehicleEnterExit.pVehicle->IsNetworkVehicle())
						{
							// Vehicle entry has been canceled
							m_vehicleEnterExit.bEntering = false;
							m_vehicleEnterExit.pVehicle = NULL;

							// Get our position
							Vector3 vecPosition;
							GetPosition(vecPosition);

							// Send cancel vehicle entry notification to server
							/*BitStream bsSend;
							bsSend.Write((char *)&vecPosition, sizeof(Vector3));
							g_pNetworkManager->GetRakClient()->RPC(RPC_CancelVehicleEntry, &bsSend, HIGH_PRIORITY, RELIABLE);*/
							CLogFile::Printf("VehicleEntryCancelled(LocalPlayer)");
						}
					}
					else
					{
						// Force ourselves to enter the vehicle
						/*EnterVehicle(m_vehicleEnterExit.vehicleId, m_vehicleEnterExit.byteSeatId, false);*/
						CLogFile::Printf("VehicleEntryRestarted(%d)", m_playerId);
					}
				}
			}
			else
			{
				// Do we have an enter vehicle task?
				if(IsGettingInToAVehicle())
				{
					// Clear our primary task
					ClearPrimaryTask(true);

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleEntryRemoved(LocalPlayer)");
					else
						CLogFile::Printf("VehicleEntryRemoved(%d)", m_playerId);
				}
			}

			// Are we flagged as exiting a vehicle?
			if(m_vehicleEnterExit.bExiting)
			{
				// Have we finished our exit vehicle task?
				if(!IsGettingOutOfAVehicle())
				{
					// Vehicle exit is complete
					m_vehicleEnterExit.bExiting = false;
					m_pVehicle->SetOccupant(m_vehicleEnterExit.byteSeatId, NULL);
					m_pVehicle = NULL;
					m_vehicleEnterExit.byteSeatId = 0;

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleExitComplete(LocalPlayer)");
					else
						CLogFile::Printf("VehicleExitComplete(%d)", m_playerId);
				}
			}
		}
	}
}

void CClientPlayer::ResetVehicleEnterExit()
{
	// Reset the vehicle enter/exit flags
	m_vehicleEnterExit.bEntering = false;
	m_vehicleEnterExit.pVehicle = NULL;
	m_vehicleEnterExit.byteSeatId = 0;
	m_vehicleEnterExit.bExiting = false;
	m_vehicleEnterExit.ulKeyHoldStartTime = 0;
}

void CClientPlayer::UpdateTargetPosition()
{
	if(HasTargetPosition())
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime();

		// Get our position
		Vector3 vecCurrentPosition;
		GetPosition(vecCurrentPosition);

		// Get the factor of time spent from the interpolation start
		// to the current time.
		float fAlpha = Unlerp(m_interp.pos.ulStartTime, ulCurrentTime, m_interp.pos.ulFinishTime);

		// Don't let it overcompensate the error
		fAlpha = Clamp(0.0f, fAlpha, 1.0f);

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_interp.pos.fLastAlpha);
		m_interp.pos.fLastAlpha = fAlpha;

		// Apply the error compensation
		Vector3 vecCompensation = Lerp(Vector3(), fCurrentAlpha, m_interp.pos.vecError);

		// If we finished compensating the error, finish it for the next pulse
		if(fAlpha == 1.0f)
			m_interp.pos.ulFinishTime = 0;

		// Calculate the new position
		Vector3 vecNewPosition = (vecCurrentPosition + vecCompensation);

		// Check if the distance to interpolate is too far
		if((vecCurrentPosition - m_interp.pos.vecTarget).Length() > 5)
		{
			// Abort all interpolation
			m_interp.pos.ulFinishTime = 0;
			vecNewPosition = m_interp.pos.vecTarget;
		}

		// Set our new position
		SetPosition(vecNewPosition);
	}
}

void CClientPlayer::Interpolate()
{
	// Are we not getting in/out of a vehicle?
	if(true)
		UpdateTargetPosition();
}

void CClientPlayer::SetTargetPosition(const Vector3& vecPosition, unsigned long ulDelay)
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Update our target position
		UpdateTargetPosition();

		// Get our position
		Vector3 vecCurrentPosition;
		GetPosition(vecCurrentPosition);

		// Set the target position
		m_interp.pos.vecTarget = vecPosition;

		// Calculate the relative error
		m_interp.pos.vecError = (vecPosition - vecCurrentPosition);

		// Get the interpolation interval
		unsigned long ulTime = SharedUtility::GetTime();
		m_interp.pos.ulStartTime = ulTime;
		m_interp.pos.ulFinishTime = (ulTime + ulDelay);

		// Initialize the interpolation
		m_interp.pos.fLastAlpha = 0.0f;
	}
}

void CClientPlayer::RemoveTargetPosition()
{
	m_interp.pos.ulFinishTime = 0;
}

void CClientPlayer::ResetInterpolation()
{
	RemoveTargetPosition();
}

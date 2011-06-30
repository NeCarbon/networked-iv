//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPlayer.cpp
// Project: Client
// Author(s): jenksta
//            Multi Theft Team
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#define MODEL_PLAYER_INDEX 211
#define PASSENGER_KEY_HOLD_TIME 1000 // 1 Second

extern CClient * g_pClient;
extern CRootEntity * g_pRootEntity;

CClientPlayer::CClientPlayer(bool bIsLocalPlayer) : CStreamableEntity(ENTITY_TYPE_PLAYER, g_pRootEntity, "player", g_pClient->GetStreamer(), 200.0f)
{
	m_bIsLocalPlayer = bIsLocalPlayer;
	m_playerId = INVALID_ENTITY_ID;
	m_pContextData = NULL;
	m_pModelInfo = g_pClient->GetGame()->GetModelInfo(MODEL_PLAYER_INDEX);
	// TODO: Model parameter
	//SetModel(iModelIndex);
	memset(&m_previousNetPadState, 0, sizeof(CNetworkPadState));
	memset(&m_currentNetPadState, 0, sizeof(CNetworkPadState));
	m_interp.pos.ulFinishTime = 0;
	m_fCurrentHeading = 0.0f;
	m_fDesiredHeading = 0.0f;
	m_uiHealth = 200;
	m_dwWeaponType = 0;
	m_dwAmmo = 0;
	m_pVehicle = NULL;
	m_byteVehicleSeatId = 0;
	ResetVehicleEnterExit();
	m_bDucking = false;
	m_bSentDeath = false;

	if(bIsLocalPlayer)
	{
		// Set the player index to local player index
		m_byteInternalPlayerNumber = (BYTE)CPools::GetLocalPlayerIndex();

		// Create the player ped instance with the local player ped pointer
		m_pPlayerPed = new CIVPlayerPed(CPools::GetPlayerInfoFromIndex(0)->m_pPlayerPed);

		// Create the player info instance with the local player info pointer
		m_pPlayerInfo = new CIVPlayerInfo(CPools::GetPlayerInfoFromIndex(0));
	}
	else
	{
		// Invalidate the player number
		m_byteInternalPlayerNumber = INVALID_PLAYER_PED;

		// Set the player ped instance to NULL
		m_pPlayerPed = NULL;

		// Set the player info instance to NULL
		m_pPlayerInfo = NULL;
	}
}

CClientPlayer::~CClientPlayer()
{
	// Notify the streamer that we have been deleted
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

		CLogFile::Printf("PlayerNumber: %d", m_byteInternalPlayerNumber);

		// Invalid player number?
		if(m_byteInternalPlayerNumber == INVALID_PLAYER_PED)
			return false;

		// Load model
		m_pModelInfo->Load();

		// Get the model index
		int iModelIndex = m_pModelInfo->GetIndex();

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

		CLogFile::Printf("Player info for player ped 0x%p, our player info 0x%p", pPlayerPed->m_pPlayerInfo, m_pPlayerInfo->GetPlayerInfo());

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
		SetHealth(m_uiHealth);

		// Set initial position
		Teleport(m_vecPosition);

		// Set initial rotation
		SetCurrentHeading(m_fCurrentHeading);

		CLogFile::Printf("Done: PlayerNumber: %d, ScriptingHandle: %d", m_byteInternalPlayerNumber, GetScriptingHandle());
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
		//dwFunc = m_pPlayerPed->GetPlayerPed()->m_VFTable->ScalarDeletingDestructor;
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
			SetCurrentHeading(m_fCurrentHeading);

			// Set the move speed
			SetMoveSpeed(m_vecMoveSpeed);

			// Set the health
			SetHealth(m_uiHealth);

			// Set the current weapon
			GiveWeapon(m_dwWeaponType, m_dwAmmo);
			SetCurrentWeapon(m_dwWeaponType);

			// Are we supposed to be in a vehicle?
			if(m_pVehicle)
			{
				// Put ourselves into the vehicle
				PutInVehicle(m_pVehicle, m_byteVehicleSeatId);
			}

			// Set the duck state
			SetDucking(m_bDucking);
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
		m_fCurrentHeading = GetCurrentHeading();

		// Get the move speed
		GetMoveSpeed(m_vecMoveSpeed);

		// Get the health
		m_uiHealth = GetHealth();

		// Get the current weapon
		m_dwWeaponType = GetCurrentWeaponType();
		m_dwAmmo = GetWeaponAmmo(m_dwWeaponType);

		// Get the duck state
		m_bDucking = IsDucking();

		// Destroy the player
		Destroy();
	}
}

void CClientPlayer::Process()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Process vehicle entry/exit
		ProcessVehicleEntryExit();

		if(IsLocalPlayer())
		{
			// Check vehicle entry/exit key
			CheckVehicleEntryExitKey();

			// Copy the current net pad state to the previous net pad state
			memcpy(&m_previousNetPadState, &m_currentNetPadState, sizeof(CNetworkPadState));

			// Update the current net pad state
			CClientPadState currentPadState;
			g_pClient->GetGame()->GetPad()->GetCurrentClientPadState(currentPadState);
			currentPadState.ToNetPadState(m_currentNetPadState, IsOnFoot());
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
		return CPools::GetPedPool()->HandleOf(m_pPlayerPed->GetPed());

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
		return g_pClient->GetStreamer()->GetVehicleFromGameVehicle(m_pPlayerPed->GetCurrentVehicle());

	return NULL;
}

bool CClientPlayer::SetModel(int iModelIndex)
{
	// TODO: Fix (this doesn't work for local player)
	// Is the model index different from our current model index?
	if(iModelIndex != m_pModelInfo->GetIndex())
	{
		// Get the new model info
		CIVModelInfo * pNewModelInfo = g_pClient->GetGame()->GetModelInfo(iModelIndex);

		// Is the new model info valid?
		if(!pNewModelInfo || !pNewModelInfo->IsValid() || !pNewModelInfo->IsPed())
		{
			CLogFile::Printf("CClientPlayer::SetModel Failed (Invalid model)!");
			return false;
		}

		bool bSpawned = IsSpawned();

		// Are we spawned?
		if(bSpawned)
		{
			// Destroy the old player
			StreamOut();
		}

		// Set the new model info
		m_pModelInfo = pNewModelInfo;

		// Are we spawned?
		if(bSpawned)
		{
			// Ensure the new model is loaded
			m_pModelInfo->Load();

			// Create the new player
			StreamIn();
		}

		return true;
	}

	return false;
}

void CClientPlayer::SetNetPadState(const CNetworkPadState& netPadState)
{
	// Copy the current net pad state to the previous net pad state
	memcpy(&m_previousNetPadState, &m_currentNetPadState, sizeof(CNetworkPadState));

	// Copy the net pad state to the current net pad state
	memcpy(&m_currentNetPadState, &netPadState, sizeof(CNetworkPadState));

	// Are we spawned?
	if(IsSpawned())
	{
		// Get the game pad
		CIVPad * pPad = g_pClient->GetGame()->GetPad();

		// Are we not the local player?
		if(!IsLocalPlayer())
		{
			// Do we have a valid context data pointer?
			if(m_pContextData)
			{
				// Get the context data pad
				pPad = m_pContextData->GetPad();
			}
		}

		// Get the current pad state
		CClientPadState currentPadState;
		pPad->GetCurrentClientPadState(currentPadState);

		// Set the last pad state
		pPad->SetLastClientPadState(currentPadState);

		// Copy the net pad state to the pad state
		currentPadState.FromNetPadState(netPadState, IsOnFoot());

		// Set the current pad state
		pPad->SetCurrentClientPadState(currentPadState);
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

void CClientPlayer::Kill(bool bInstantly)
{
	// Are we spawned and not already dead?
	if(IsSpawned() && !IsDead())
	{
		if(bInstantly)
		{
			// Create the dead task
			// if this doesn't work vary last 2 params (1, 0 : 0, 1 : 1, 1 : 0, 0)
			CIVTaskSimpleDead * pTask = new CIVTaskSimpleDead(g_pClient->GetGame()->GetTime(), 1, 0);

			// Did the task create successfully?
			if(pTask)
			{
				// Set it as the ped task
				pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_DEFAULT);
			}
		}
		else
		{
			// Create the death task
			// guess from sa (thx mta)
			// wep type, body part, anim group, anim id, unknown?
			CIVTaskComplexDie * pTask = new CIVTaskComplexDie(0, 0, 44, 190, 4.0f, 0.0f, 1);

			// Did the task create successfully?
			if(pTask)
			{
				// Set it as the ped task
				pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);
			}
		}

		// Set the health and armour to 0
		SetHealth(0);
		//SetArmour(0);

		// Reset the pad state
		CNetworkPadState padState;
		SetNetPadState(padState);
	}
}

bool CClientPlayer::IsDead()
{
	bool bReturn = false;

	if(IsSpawned())
	{
		CIVTask * pTask = m_pPlayerPed->GetPedTaskManager()->GetTask(TASK_PRIORITY_PRIMARY);

		if(pTask)
		{
			if(pTask->GetType() == TASK_SIMPLE_DEAD)
				bReturn = true;
		}
	}

	return bReturn;
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

void CClientPlayer::Teleport(const CVector3& vecPosition)
{
	if(IsSpawned())
	{
		// Teleport the player ped
		// TODO: Use reversed code from this native
		InvokeNative<void *>(NATIVE_SET_CHAR_COORDINATES, GetScriptingHandle(), vecPosition.fX, vecPosition.fY, vecPosition.fZ);
	}

	m_vecPosition = vecPosition;
}

void CClientPlayer::SetPosition(const CVector3& vecPosition)
{
	// TODO: Use reversed code from SET_CHAR_COORDINATES native to fix this
	// (atm it doesn't work properly)
	if(IsSpawned())
	{
		// Remove the player ped from the world
		m_pPlayerPed->RemoveFromWorld();

		// Set the position in the player ped matrix
		m_pPlayerPed->SetPosition((CVector3 *)&vecPosition);

		// Re-add the player ped to the world
		m_pPlayerPed->AddToWorld();
	}

	m_vecPosition = vecPosition;
}

void CClientPlayer::GetPosition(CVector3& vecPosition)
{
	if(IsSpawned())
		m_pPlayerPed->GetPosition(&vecPosition);
	else
		vecPosition = m_vecPosition;
}

void CClientPlayer::SetCurrentHeading(float fCurrentHeading)
{
	if(IsSpawned())
	{
		m_pPlayerPed->SetCurrentHeading(fCurrentHeading);
		SetDesiredHeading(fCurrentHeading);
	}

	m_fCurrentHeading = fCurrentHeading;
}

float CClientPlayer::GetCurrentHeading()
{
	if(IsSpawned())
		return m_pPlayerPed->GetCurrentHeading();
	else
		return m_fCurrentHeading;
}

void CClientPlayer::SetDesiredHeading(float fDesiredHeading)
{
	if(IsSpawned())
		m_pPlayerPed->SetDesiredHeading(fDesiredHeading);

	m_fDesiredHeading = fDesiredHeading;
}

float CClientPlayer::GetDesiredHeading()
{
	if(IsSpawned())
		return m_pPlayerPed->GetDesiredHeading();
	else
		return m_fDesiredHeading;
}

void CClientPlayer::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->SetMoveSpeed((CVector3 *)&vecMoveSpeed);

	m_vecMoveSpeed = vecMoveSpeed;
}

void CClientPlayer::GetMoveSpeed(CVector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pPlayerPed->GetMoveSpeed(&vecMoveSpeed);
	else
		vecMoveSpeed = m_vecMoveSpeed;
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

void CClientPlayer::SetDucking(bool bDucking)
{
	if(IsSpawned())
		m_pPlayerPed->SetDucking(bDucking);

	m_bDucking = bDucking;
}

bool CClientPlayer::IsDucking()
{
	if(IsSpawned())
		return m_pPlayerPed->IsDucking();

	return m_bDucking;
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
		CVector3 vecVehiclePos;
		CClientVehicle * pClosestVehicle = NULL;

		// Get our position
		CVector3 vecPlayerPos;
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

// From 0x9C6DB0
// Looks like vehicles never have more than 4 doors?
BYTE GetDoorFromSeat(BYTE byteSeatId)
{
	BYTE byteDoorId;

	switch(byteSeatId)
	{
	case 0:
		byteDoorId = 2;
		break;
	case 1:
		byteDoorId = 1;
		break;
	case 2:
		byteDoorId = 3;
		break;
	default:
		byteDoorId = 0;
		break;
	}

	return byteDoorId;
}

int iProcessFrames = 0;

void CClientPlayer::EnterVehicle(CClientVehicle * pVehicle, BYTE byteSeatId)
{
	// Are we spawned?
	if(IsSpawned())
	{
		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d)", pVehicle, byteSeatId);

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

		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 2", pVehicle, byteSeatId);

		// Are we already in a vehicle?
		if(IsInVehicle())
			return;

		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 3", pVehicle, byteSeatId);

		/*BYTE byteDoorId = GetDoorFromSeat(byteSeatId);
		int iUnknown2 = 0;

		//if(byteSeatId != 0)
		//	iUnknown2 = 0x200000;

		// Create the enter vehicle task
		CIVTaskComplexNewGetInVehicle * pTask = new CIVTaskComplexNewGetInVehicle(m_pVehicle->GetGameVehicle(), byteDoorId, 27, iUnknown2, 0.0f);

		// Did the task create successfully?
		if(pTask)
		{
			// Set it as the ped task
			pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_PRIMARY);
		}*/

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

		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 4", pVehicle, byteSeatId);

		// Mark ourselves as entering a vehicle and store our vehicle and seat
		m_vehicleEnterExit.bEntering = true;
		m_vehicleEnterExit.pVehicle = pVehicle;
		m_vehicleEnterExit.byteSeatId = byteSeatId;

		CLogFile::Printf("CClientPlayer::EnterVehicle(0x%p, %d) 5", pVehicle, byteSeatId);

		// Reset interpolation
		ResetInterpolation();

		// see ProcessVehicleEntryExit
		if(IsLocalPlayer())
			iProcessFrames = 1;
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
				CVector3 vecPos;
				m_pVehicle->GetPosition(vecPos);
				InvokeNative<void *>(WARP_CHAR_FROM_CAR_TO_COORD, GetScriptingHandle(), vecPos.fX, vecPos.fY, (vecPos.fZ + 1.0f));

				// Reset the vehicle occupant for our seat
				m_pVehicle->SetOccupant(m_vehicleEnterExit.byteSeatId, NULL);

				// Reset our current vehicle pointer
				m_pVehicle = NULL;

				// Reset our vehicle seat id
				m_byteVehicleSeatId = 0;

				// Reset vehicle entry/exit flags
				ResetVehicleEnterExit();
			}
			else
			{
				// Create the exit vehicle task
				CIVTaskComplexNewExitVehicle * pTask = new CIVTaskComplexNewExitVehicle(m_pVehicle->GetGameVehicle(), 0xF, 0, 0);

				// Did the task create successfully?
				if(pTask)
				{
					// Set it as the ped task
					pTask->SetAsPedTask(m_pPlayerPed, TASK_PRIORITY_PRIMARY);
				}

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

		/*
		v3 = CPool__AtHandle(g_pPedPool, a1);
		v2 = CPool__AtHandle(g_pVehiclePool, a2);
		if ( *(_DWORD *)(v3 + 0x24) & 0x8000000 )
		{
		CWorld__RemoveEntity(v3, 0);
		sub_9E6830(v3, v2, 0);
		CWorld__AddEntity(v3, 0);
		}
		result = *(_DWORD *)(v3 + 0x6C);
		if ( !result || !*(_BYTE *)(result + 0xE) )
		{
		ShutdownPedIntelligence(*(void **)(v3 + 0x224), 0);
		v5 = GetDoorFromSeat(-1);
		CTaskSimpleCarSetPedInVehicle__CTaskSimpleCarSetPedInVehicle(&v6, v2, v5, 0, 0);
		sub_AA07C0(v3);
		result = CTaskSimpleCarSetPedInVehicle___CTaskSimpleCarSetPedInVehicle(&v6);
		}
		return result;
		*/

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
		m_byteVehicleSeatId = byteSeatId;
		pVehicle->SetOccupant(byteSeatId, this);
	}
}

void CClientPlayer::CheckVehicleEntryExitKey()
{
	// Are we spawned and is input enabled?
	if(IsSpawned() && g_pClient->GetInputState())
	{
		// Has the enter/exit vehicle key just been pressed?
		if(!m_previousNetPadState.IsUsingEnterExitVehicle() && m_currentNetPadState.IsUsingEnterExitVehicle())
		{
			if(!m_vehicleEnterExit.bRequesting && IsInVehicle() && !m_vehicleEnterExit.bExiting)
			{
				if(IsLocalPlayer())
					CLogFile::Printf("HandleVehicleExit(LocalPlayer)");
				else
					CLogFile::Printf("HandleVehicleExit(%d)", m_playerId);

				// Is this a network vehicle?
				if(m_pVehicle->IsNetworkVehicle())
				{
					// Request the vehicle exit
					CBitStream bitStream;
					bitStream.Write((BYTE)VEHICLE_EXIT_REQUEST);
					bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
					g_pClient->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					m_vehicleEnterExit.bRequesting = true;
				}
				else
				{
					// Exit the vehicle
					ExitVehicle(false);
				}
			}
			else
			{
				// Save the current time into the enter/exit vehicle key hold start time
				m_vehicleEnterExit.ulKeyHoldStartTime = SharedUtility::GetTime();

				if(IsLocalPlayer())
					CLogFile::Printf("EnterExitVehicleKeyHoldStart(LocalPlayer)");
				else
					CLogFile::Printf("EnterExitVehicleKeyHoldStart(%d)", m_playerId);
			}
		}
		else
		{
			// Has the enter/exit vehicle key just been released?
			if(m_previousNetPadState.IsUsingEnterExitVehicle() && !m_currentNetPadState.IsUsingEnterExitVehicle() && 
				m_vehicleEnterExit.ulKeyHoldStartTime)
			{
				if(IsLocalPlayer())
					CLogFile::Printf("EnterExitVehicleKeyHoldEnd(LocalPlayer)");
				else
					CLogFile::Printf("EnterExitVehicleKeyHoldEnd(%d)", m_playerId);

				if(!m_vehicleEnterExit.bRequesting)
				{
					if(!IsInVehicle() && !m_vehicleEnterExit.bEntering)
					{
						// Get the time the enter/exit vehicle key was held for
						unsigned long ulKeyHoldTime = (SharedUtility::GetTime() - m_vehicleEnterExit.ulKeyHoldStartTime);

						CClientVehicle * pVehicle = NULL;
						BYTE byteSeatId = 0;
						bool bFound = false;

						// Has the key been held long enough to enter as a passenger?
						if(ulKeyHoldTime >= PASSENGER_KEY_HOLD_TIME)
						{
							// Do we have a close vehicle?
							bFound = GetClosestVehicle(true, &pVehicle, byteSeatId);
						}
						else
						{
							// Key has not been held long enough to enter as a passenger, enter as driver
							// Do we have a close vehicle?
							bFound = GetClosestVehicle(false, &pVehicle, byteSeatId);
						}

						// Have we found a close vehicle?
						if(bFound)
						{
							if(IsLocalPlayer())
								CLogFile::Printf("HandleVehicleEntry(LocalPlayer, %d, %d)", pVehicle->GetVehicleId(), byteSeatId);
							else
								CLogFile::Printf("HandleVehicleEntry(%d, %d, %d)", m_playerId, pVehicle->GetVehicleId(), byteSeatId);

							// Is this a network vehicle?
							if(pVehicle->IsNetworkVehicle())
							{
								// Request the vehicle entry
								CBitStream bitStream;
								bitStream.Write((BYTE)VEHICLE_ENTRY_REQUEST);
								bitStream.WriteCompressed(pVehicle->GetVehicleId());
								bitStream.Write(byteSeatId);
								g_pClient->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
								m_vehicleEnterExit.bRequesting = true;
							}
							else
							{
								// Enter the vehicle
								EnterVehicle(pVehicle, byteSeatId);
							}
						}

						// Reset the enter/exit vehicle key hold start time
						m_vehicleEnterExit.ulKeyHoldStartTime = 0;
					}
				}
				else
				{
					CLogFile::Printf("Already requesting vehicle entry/exit!");
				}
			}
		}
	}
}

void CClientPlayer::ProcessVehicleEntryExit()
{
	// this hack is needed to make vehicle entry work for network vehicles
	// (due to the fact when you call task natives the task doesnt apply till next frame)
	// once we apply vehicle entry/exit tasks properly, this can be removed
	// note: not safe for more than one player at a time
	if(IsLocalPlayer())
	{
		if(iProcessFrames > 0)
		{
			iProcessFrames++;
			if(iProcessFrames == 4)
				iProcessFrames = 0;
			else
				return;
		}
	}

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
					m_byteVehicleSeatId = m_vehicleEnterExit.byteSeatId;
					m_pVehicle->SetOccupant(m_vehicleEnterExit.byteSeatId, this);
					m_vehicleEnterExit.pVehicle = NULL;

					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send the network rpc
						CBitStream bitStream;
						bitStream.Write((BYTE)VEHICLE_ENTRY_COMPLETE);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						bitStream.Write(m_byteVehicleSeatId);
						g_pClient->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

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
							// Get our position
							CVector3 vecPosition;
							GetPosition(vecPosition);

							// Send the network rpc
							CBitStream bitStream;
							bitStream.Write((BYTE)VEHICLE_ENTRY_CANCELLED);
							bitStream.WriteCompressed(m_vehicleEnterExit.pVehicle->GetVehicleId());
							bitStream.Write(m_byteVehicleSeatId);
							g_pClient->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);

							CLogFile::Printf("VehicleEntryCancelled(LocalPlayer)");
						}

						// Vehicle entry has been canceled
						m_vehicleEnterExit.bEntering = false;
						m_vehicleEnterExit.pVehicle = NULL;
					}
					else
					{
						// Force ourselves to enter the vehicle
						EnterVehicle(m_vehicleEnterExit.pVehicle, m_vehicleEnterExit.byteSeatId);
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
					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send the network rpc
						CBitStream bitStream;
						bitStream.Write((BYTE)VEHICLE_EXIT_COMPLETE);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pClient->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					// Vehicle exit is complete
					m_vehicleEnterExit.bExiting = false;
					m_pVehicle->SetOccupant(m_byteVehicleSeatId, NULL);
					m_pVehicle = NULL;
					m_byteVehicleSeatId = 0;

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleExitComplete(LocalPlayer)");
					else
						CLogFile::Printf("VehicleExitComplete(%d)", m_playerId);
				}
			}
			else
			{
				// Do we have an exit vehicle task?
				if(IsGettingOutOfAVehicle())
				{
					// Clear our primary task
					ClearPrimaryTask(true);

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleExitRemoved(LocalPlayer)");
					else
						CLogFile::Printf("VehicleExitRemoved(%d)", m_playerId);
				}

				// Are we flagged as in a vehicle?
				if(m_pVehicle)
				{
					// Is this a network vehicle?
					if(m_pVehicle->IsNetworkVehicle())
					{
						// Send the network rpc
						CBitStream bitStream;
						bitStream.Write((BYTE)VEHICLE_EXIT_FORCEFUL);
						bitStream.WriteCompressed(m_pVehicle->GetVehicleId());
						g_pClient->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE);
					}

					// Player has forcefully exited the vehicle (out of windscreen, e.t.c.)
					m_pVehicle->SetOccupant(m_byteVehicleSeatId, NULL);
					m_pVehicle = NULL;
					m_byteVehicleSeatId = 0;

					if(IsLocalPlayer())
						CLogFile::Printf("VehicleForcefulExit(LocalPlayer)");
					else
						CLogFile::Printf("VehicleForcefulExit(%d)", m_playerId);
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
	m_vehicleEnterExit.bRequesting = false;
}

void CClientPlayer::UpdateTargetPosition()
{
	if(HasTargetPosition())
	{
		unsigned long ulCurrentTime = SharedUtility::GetTime();

		// Get our position
		CVector3 vecCurrentPosition;
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
		CVector3 vecCompensation = Lerp(CVector3(), fCurrentAlpha, m_interp.pos.vecError);

		// If we finished compensating the error, finish it for the next pulse
		if(fAlpha == 1.0f)
			m_interp.pos.ulFinishTime = 0;

		// Calculate the new position
		CVector3 vecNewPosition = (vecCurrentPosition + vecCompensation);

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

void CClientPlayer::SetTargetPosition(const CVector3& vecPosition, unsigned long ulDelay)
{
	// Update our target position
	UpdateTargetPosition();

	// Are we spawned?
	if(IsSpawned())
	{
		// Get our position
		CVector3 vecCurrentPosition;
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
	else
	{
		// Set the position
		SetPosition(vecPosition);
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

void CClientPlayer::Serialize(CBitStream * pBitStream)
{
	// Write the player net pad state
	CNetworkPadState netPadState;
	GetNetPadState(netPadState);
	pBitStream->Write(netPadState);

	// Write the player health
	pBitStream->Write(GetHealth());

	// Write if we are on foot
	pBitStream->WriteBit(IsOnFoot());

	// Are we on foot?
	if(IsOnFoot())
	{
		// Write the player position
		CVector3 vecPosition;
		GetPosition(vecPosition);
		pBitStream->Write(vecPosition);

		// Write the player heading
		pBitStream->Write(GetCurrentHeading());

		// Write the player move speed
		CVector3 vecMoveSpeed;
		GetMoveSpeed(vecMoveSpeed);
		pBitStream->Write(vecMoveSpeed);
		
		// Write the player duck state
		pBitStream->WriteBit(IsDucking());

		// Write the player current weapon
		pBitStream->Write((unsigned int)GetCurrentWeaponType());
	}
	else
	{
		// Write our vehicle id
		pBitStream->WriteCompressed(m_pVehicle->GetVehicleId());

		// Write our vehicle seat id
		pBitStream->Write(m_byteVehicleSeatId);

		// Are we the driver?
		if(m_byteVehicleSeatId == 0)
		{
			// Serialize the vehicle to the bit stream
			m_pVehicle->Serialize(pBitStream);
		}
	}
}

bool CClientPlayer::Deserialize(CBitStream * pBitStream)
{
	// Read the player net pad state
	CNetworkPadState netPadState;

	if(!pBitStream->Read(netPadState))
	{
		CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 1)");
		return false;
	}

	SetNetPadState(netPadState);

	// Read the player health
	unsigned int uiHealth;
	if(!pBitStream->Read(uiHealth))
	{
		CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 10)");
		return false;
	}

	SetHealth(uiHealth);

	// Read if we are on foot
	bool bIsOnFoot = pBitStream->ReadBit();

	// Are we on foot or in a vehicle when we are not meant to be?
	if(bIsOnFoot != IsOnFoot())
	{
		CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 2)");
		return false;
	}

	// Are we on foot?
	if(bIsOnFoot)
	{
		// Read the player position
		CVector3 vecPosition;

		if(!pBitStream->Read(vecPosition))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 3)");
			return false;
		}

		SetTargetPosition(vecPosition, NETWORK_TICK_RATE);

		// Read the player heading
		// TODO: Interpolate rotation too
		float fHeading;

		if(!pBitStream->Read(fHeading))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 4)");
			return false;
		}

		SetCurrentHeading(fHeading);

		// Read the player move speed
		CVector3 vecMoveSpeed;

		if(!pBitStream->Read(vecMoveSpeed))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 5)");
			return false;
		}

		SetMoveSpeed(vecMoveSpeed);

		// Read the player duck state
		SetDucking(pBitStream->ReadBit());

		// Read the current weapon type
		unsigned int uiCurrentWeapon;
		if(!pBitStream->Read(uiCurrentWeapon))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (error code: current weapon)");
			return false;
		}

		SetCurrentWeapon(uiCurrentWeapon);
	}
	else
	{
		// Read the vehicle id
		EntityId vehicleId;

		if(!pBitStream->ReadCompressed(vehicleId))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 6)");
			return false;
		}

		// Read the vehicle seat id
		BYTE byteSeatId;

		if(!pBitStream->Read(byteSeatId))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 7)");
			return false;
		}

		// TODO: Check against vehicle id for validity?

		// Get the vehicle pointer
		CClientVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

		// Is the vehicle pointer valid?
		if(!pVehicle)
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 8)");
			return false;
		}

		// Are we not already in the vehicle?
		if(m_pVehicle != pVehicle)
		{
			// Force us in to the vehicle
			PutInVehicle(pVehicle, byteSeatId);
		}

		// Are we the driver?
		if(byteSeatId == 0)
		{
			// Deserialize the vehicle from the bit stream
			if(!pVehicle->Deserialize(pBitStream))
			{
				CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 9)");
				return false;
			}
		}
	}

	return true;
}

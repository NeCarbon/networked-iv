//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientVehicle.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#define MODEL_ADMIRAL_INDEX 84

extern CClient * g_pClient;
extern CRootEntity * g_pRootEntity;

CClientVehicle::CClientVehicle(int iModelIndex) : CStreamableEntity(ENTITY_TYPE_VEHICLE, g_pRootEntity, "vehicle", g_pClient->GetStreamer(), 200.0f)
{
	m_vehicleId = INVALID_ENTITY_ID;
	m_pVehicle = NULL;
	m_pModelInfo = g_pClient->GetGame()->GetModelInfo(MODEL_ADMIRAL_INDEX);
	SetModel(iModelIndex);
	memset(m_byteColors, 0, sizeof(m_byteColors));
	m_pDriver = NULL;
	memset(m_pPassengers, 0, sizeof(m_pPassengers));
	//m_interp.pos.ulFinishTime = 0;
	m_fHealth = 1000.0f;
}

CClientVehicle::~CClientVehicle()
{
	// Notify the streamer that we have been deleted
	OnDelete();
}

void CClientVehicle::SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4)
{
	// Are we spawned?
	if(IsSpawned())
		m_pVehicle->SetColors(byteColor1, byteColor2, byteColor3, byteColor4);

	m_byteColors[0] = byteColor1;
	m_byteColors[1] = byteColor2;
	m_byteColors[2] = byteColor3;
	m_byteColors[3] = byteColor4;
}

void CClientVehicle::GetColors(BYTE& byteColor1, BYTE& byteColor2, BYTE& byteColor3, BYTE& byteColor4)
{
	byteColor1 = m_byteColors[0];
	byteColor2 = m_byteColors[1];
	byteColor3 = m_byteColors[2];
	byteColor4 = m_byteColors[3];
}

void CClientVehicle::SetPassenger(BYTE bytePassengerId, CClientPlayer * pPassenger)
{
	if(bytePassengerId < MAX_VEHICLE_PASSENGERS)
		return;

	m_pPassengers[bytePassengerId] = pPassenger;
}

CClientPlayer * CClientVehicle::GetPassenger(BYTE bytePassengerId)
{
	if(bytePassengerId >= MAX_VEHICLE_PASSENGERS)
		return NULL;

	return m_pPassengers[bytePassengerId];
}

void CClientVehicle::SetOccupant(BYTE byteSeatId, CClientPlayer * pOccupant)
{
	if(byteSeatId == 0)
		SetDriver(pOccupant);
	else
		SetPassenger((byteSeatId - 1), pOccupant);
}

CClientPlayer * CClientVehicle::GetOccupant(BYTE byteSeatId)
{
	if(byteSeatId == 0)
		return GetDriver();

	return GetPassenger(byteSeatId - 1);
}

CClientPlayer * CClientVehicle::GetSyncer()
{
	// Loop through all seats
	for(BYTE byteSeatID = 0; byteSeatID < MAX_VEHICLE_PASSENGERS; ++byteSeatID)
	{
		// Check for the player occupying that seat
		CClientPlayer * pPlayer = GetOccupant(byteSeatID);

		// Any player in that seat?
		if(pPlayer)
			return pPlayer;
	}

	return NULL;
}

bool CClientVehicle::Create()
{
	// Are we not already created?
	if(!IsSpawned())
	{
		// Load the model
		m_pModelInfo->Load();

		// Get the model hash
		DWORD dwModelHash = m_pModelInfo->GetHash();

		// Create vehicle (TODO: Don't use natives for this)
		unsigned int uiVehicleHandle;
		InvokeNative<void *>(NATIVE_CREATE_CAR, dwModelHash, 0.0f, 0.0f, 0.0f, &uiVehicleHandle, true);

		// Create vehicle instance
		m_pVehicle = new CIVVehicle(CPools::GetVehiclePool()->AtHandle(uiVehicleHandle));

		// Set initial colors
		SetColors(m_byteColors[0], m_byteColors[1], m_byteColors[2], m_byteColors[3]);

		// Add to world
		m_pVehicle->AddToWorld();

		// Set initial health
		SetHealth(m_fHealth);

		// Set initial position
		Teleport(m_vecPosition);

		// Set initial rotation
		SetRotation(m_vecRotation);
		return true;
	}

	// Already created
	return false;
}

void CClientVehicle::Destroy()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Remove the driver
		if(m_pDriver)
			m_pDriver->ExitVehicle(true);

		// Remove all passengers
		for(BYTE i = 0; i < MAX_VEHICLE_PASSENGERS; i++)
		{
			if(m_pPassengers[i])
				m_pPassengers[i]->ExitVehicle(true);
		}

		// Get the vehicle pointer
		IVVehicle * pVehicle = m_pVehicle->GetVehicle();

		*(BYTE *)(pVehicle + 0xF6B) &= 0xDFu;

		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();

		// Delete the vehicle
		DWORD dwFunc = pVehicle->m_VFTable->ScalarDeletingDestructor;
		_asm
		{
			push 1
			mov ecx, pVehicle
			call dwFunc
		}

		// Delete the vehicle instance
		SAFE_DELETE(m_pVehicle);
	}
}

void CClientVehicle::StreamIn()
{
	// Attempt to create the vehicle
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

		// Set the colors
		SetColors(m_byteColors[0], m_byteColors[1], m_byteColors[2], m_byteColors[3]);

		// Set the health
		SetHealth(m_fHealth);
	}
}

void CClientVehicle::StreamOut()
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
	m_fHealth = GetHealth();

	// Destroy the vehicle
	Destroy();
}

void CClientVehicle::Process()
{

}

bool CClientVehicle::SetModel(int iModelIndex)
{
	// Is the model index different from our current model index?
	if(iModelIndex != m_pModelInfo->GetIndex())
	{
		// Get the new model info
		CIVModelInfo * pNewModelInfo = g_pClient->GetGame()->GetModelInfo(iModelIndex);

		// Is the new model info valid?
		if(!pNewModelInfo || !pNewModelInfo->IsValid() || !pNewModelInfo->IsVehicle())
		{
			CLogFile::Printf("CClientVehicle::SetModel Failed (Invalid model)!");
			return false;
		}

		bool bSpawned = IsSpawned();

		// Are we spawned?
		if(bSpawned)
		{
			// Destroy the old vehicle
			StreamOut();
		}

		// Set the new model info
		m_pModelInfo = pNewModelInfo;

		// Are we spawned?
		if(bSpawned)
		{
			// Ensure the new model is loaded
			m_pModelInfo->Load();

			// Create the new vehicle
			StreamIn();
		}

		return true;
	}

	return false;
}

unsigned int CClientVehicle::GetScriptingHandle()
{
	if(IsSpawned())
		return CPools::GetVehiclePool()->HandleOf(m_pVehicle->GetVehicle());

	return 0;
}

void CClientVehicle::SetHealth(float fHealth)
{
	if(IsSpawned())
	{
		// Set the vehicle engine health
		m_pVehicle->SetEngineHealth(fHealth);
	}

	m_fHealth = fHealth;
}

float CClientVehicle::GetHealth()
{
	if(IsSpawned())
	{
		// Get the vehicle engine health
		return m_pVehicle->GetEngineHealth();
	}

	return m_fHealth;
}

void CClientVehicle::Teleport(const CVector3& vecPosition)
{
	if(IsSpawned())
	{
		// Teleport the vehicle (TODO: Don't use natives for this)
		// TODO: Use reversed code from this native
		InvokeNative<void *>(NATIVE_SET_CAR_COORDINATES_NO_OFFSET, GetScriptingHandle(), vecPosition.fX, vecPosition.fY, vecPosition.fZ);
	}

	m_vecPosition = vecPosition;
}

void CClientVehicle::SetPosition(const CVector3& vecPosition)
{
	// TODO: Use reversed code from SET_CAR_COORDINATES_NO_OFFSET native to fix this
	// (atm it cannot be used for long distances)
	if(IsSpawned())
	{
		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();

		// Set the position in the vehicle matrix
		m_pVehicle->SetPosition((CVector3 *)&vecPosition);

		// Re-add the vehicle to the world
		m_pVehicle->AddToWorld();
	}

	m_vecPosition = vecPosition;
}

void CClientVehicle::GetPosition(CVector3& vecPosition)
{
	if(IsSpawned())
		m_pVehicle->GetPosition(&vecPosition);
	else
		vecPosition = m_vecPosition;
}

void CClientVehicle::SetRotation(const CVector3& vecRotation)
{
	if(IsSpawned())
	{
		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();

		// Get the vehicle matrix
		Matrix matMatrix;
		m_pVehicle->GetMatrix(&matMatrix);

		// Convert the rotation to radians and apply it to the vehicle matrix
		CVector3 vecNewRotation = vecRotation;
		ConvertDegreesToRadians(vecNewRotation);
		g_pClient->GetGame()->ConvertEulerAnglesToRotationMatrix(vecNewRotation, matMatrix);

		// Set the new vehicle matrix
		m_pVehicle->SetMatrix(&matMatrix);

		// Re-add the vehicle to the world
		m_pVehicle->AddToWorld();
	}

	m_vecRotation = vecRotation;
}

void CClientVehicle::GetRotation(CVector3& vecRotation)
{
	if(IsSpawned())
	{
		// Get the vehicle matrix
		Matrix matMatrix;
		m_pVehicle->GetMatrix(&matMatrix);

		// Convert the matrix to euler angles
		g_pClient->GetGame()->ConvertRotationMatrixToEulerAngles(matMatrix, vecRotation);

		// Flip the rotation
		vecRotation.fX = (2 * PI) - vecRotation.fX;
		vecRotation.fY = (2 * PI) - vecRotation.fY;
		vecRotation.fZ = (2 * PI) - vecRotation.fZ;

		// Convert the rotation from radians to degrees
		ConvertRadiansToDegrees(vecRotation);
	}
	else
		vecRotation = m_vecRotation;
}

void CClientVehicle::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pVehicle->SetMoveSpeed((CVector3 *)&vecMoveSpeed);

	m_vecMoveSpeed = vecMoveSpeed;
}

void CClientVehicle::GetMoveSpeed(CVector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pVehicle->GetMoveSpeed(&vecMoveSpeed);
	else
		vecMoveSpeed = m_vecMoveSpeed;
}

void CClientVehicle::SetTurnSpeed(const CVector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pVehicle->SetTurnSpeed((CVector3 *)&vecTurnSpeed);

	m_vecTurnSpeed = vecTurnSpeed;
}

void CClientVehicle::GetTurnSpeed(CVector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pVehicle->GetTurnSpeed(&vecTurnSpeed);
	else
		vecTurnSpeed = m_vecTurnSpeed;
}

BYTE CClientVehicle::GetMaxPassengers()
{
	if(IsSpawned())
		return m_pVehicle->GetMaxPasssengers();

	return 0;
}

void CClientVehicle::Serialize(CBitStream * pBitStream)
{
	// Write the vehicle position
	CVector3 vecPosition;
	GetPosition(vecPosition);
	pBitStream->Write(vecPosition);

	// Write the vehicle rotation
	CVector3 vecRotation;
	GetRotation(vecRotation);
	pBitStream->Write(vecRotation);

	// Write the vehicle move speed
	CVector3 vecMoveSpeed;
	GetMoveSpeed(vecMoveSpeed);
	pBitStream->Write(vecMoveSpeed);

	// Write the vehicle turn speed
	CVector3 vecTurnSpeed;
	GetTurnSpeed(vecTurnSpeed);
	pBitStream->Write(vecTurnSpeed);

	// Write the vehicle health
	pBitStream->Write(GetHealth());
}

bool CClientVehicle::Deserialize(CBitStream * pBitStream)
{
	// Read the vehicle position
	CVector3 vecPosition;

	if(!pBitStream->Read(vecPosition))
		return false;

	SetPosition(vecPosition);

	// Read the vehicle rotation
	CVector3 vecRotation;

	if(!pBitStream->Read(vecRotation))
		return false;

	SetRotation(vecRotation);

	// Read the vehicle move speed
	CVector3 vecMoveSpeed;

	if(!pBitStream->Read(vecMoveSpeed))
		return false;

	SetMoveSpeed(vecMoveSpeed);

	// Read the vehicle turn speed
	CVector3 vecTurnSpeed;

	if(!pBitStream->Read(vecTurnSpeed))
		return false;

	SetTurnSpeed(vecTurnSpeed);

	// Read the vehicle health
	float fHealth;

	if(!pBitStream->Read(fHealth))
		return false;

	SetHealth(fHealth);
	return true;
}

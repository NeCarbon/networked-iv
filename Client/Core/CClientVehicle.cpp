//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientVehicle.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CClientVehicle::CClientVehicle(DWORD dwModelHash)/* : CStreamableEntity(g_pClient->GetStreamer(), ENTITY_TYPE_VEHICLE, 200.0f)*/
{
	m_vehicleId = INVALID_ENTITY_ID;
	m_pVehicle = NULL;
	m_dwModelHash = dwModelHash;
	m_pDriver = NULL;
	memset(m_pPassengers, 0, sizeof(m_pPassengers));
}

CClientVehicle::~CClientVehicle()
{
	// Are we spawned?
	if(IsSpawned())
	{
		// Destroy ourselves
		Destroy();
	}

	// Delete the vehicle instance
	SAFE_DELETE(m_pVehicle);
}

void CClientVehicle::SetPassenger(BYTE bytePassengerId, CClientPlayer * pPassenger)
{
	if(bytePassengerId < MAX_PASSENGERS)
		return;

	m_pPassengers[bytePassengerId] = pPassenger;
}

CClientPlayer * CClientVehicle::GetPassenger(BYTE bytePassengerId)
{
	if(bytePassengerId >= MAX_PASSENGERS)
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

bool CClientVehicle::Create()
{
	// Are we not already created?
	if(!IsSpawned())
	{
		// Load model (if needed) and get model index
		int iModelIndex = g_pClient->GetGame()->LoadModel(m_dwModelHash);

		// Invalid model hash?
		if(iModelIndex == -1)
			return false;

		// Create vehicle (TODO: Don't use natives for this)
		unsigned int uiVehicleHandle;
		InvokeNative<void *>(NATIVE_CREATE_CAR, m_dwModelHash, 0.0f, 0.0f, 0.0f, &uiVehicleHandle, true);

		// Create vehicle instance
		m_pVehicle = new CIVVehicle(CPools::GetVehicleFromHandle(uiVehicleHandle));

		// Use the network object pointer to store out player ped instance pointer
		//m_pVehicle->GetVehicle()->m_dwNetObject = (DWORD)m_pVehicle;

		// Add to world
		m_pVehicle->AddToWorld();

		// Set initial health
		SetHealth(1000.0f);

		// Set initial position
		Teleport(Vector3());

		// Set initial rotation
		SetRotation(Vector3());
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

unsigned int CClientVehicle::GetScriptingHandle()
{
	if(IsSpawned())
		return CPools::GetHandleFromVehicle(m_pVehicle->GetVehicle());

	return 0;
}

void CClientVehicle::SetHealth(float fHealth)
{
	if(IsSpawned())
	{
		// Set the vehicle engine health
		m_pVehicle->SetEngineHealth(fHealth);
	}
}

float CClientVehicle::GetHealth()
{
	if(IsSpawned())
	{
		// Get the vehicle engine health
		return m_pVehicle->GetEngineHealth();
	}

	return 0.0f;
}

void CClientVehicle::Teleport(const Vector3& vecPosition)
{
	if(IsSpawned())
	{
		// Teleport the vehicle (TODO: Don't use natives for this)
		// TODO: Use reversed code from this native
		InvokeNative<void *>(NATIVE_SET_CAR_COORDINATES_NO_OFFSET, GetScriptingHandle(), vecPosition.fX, vecPosition.fY, vecPosition.fZ);
	}
}

void CClientVehicle::SetPosition(const Vector3& vecPosition)
{
	// TODO: Use reversed code from SET_CAR_COORDINATES_NO_OFFSET native to fix this
	// (atm it cannot be used for long distances)
	if(IsSpawned())
	{
		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();

		// Set the position in the vehicle matrix
		m_pVehicle->SetPosition((Vector3 *)&vecPosition);

		// Re-add the vehicle to the world
		m_pVehicle->AddToWorld();
	}
}

void CClientVehicle::GetPosition(Vector3& vecPosition)
{
	if(IsSpawned())
		m_pVehicle->GetPosition(&vecPosition);
	else
		memset(&vecPosition, 0, sizeof(Vector3));
}

void CClientVehicle::SetRotation(const Vector3& vecRotation)
{
	if(IsSpawned())
	{
		// Remove the vehicle from the world
		m_pVehicle->RemoveFromWorld();

		// Get the vehicle matrix
		Matrix matMatrix;
		m_pVehicle->GetMatrix(&matMatrix);

		// Convert the rotation to radians and apply it to the vehicle matrix
		Vector3 vecNewRotation = ((Vector3)vecRotation).ToRadians();
		g_pClient->GetGame()->ConvertEulerAnglesToRotationMatrix(&vecNewRotation, &matMatrix);

		// Set the new vehicle matrix
		m_pVehicle->SetMatrix(&matMatrix);

		// Re-add the vehicle to the world
		m_pVehicle->AddToWorld();
	}
}

void CClientVehicle::GetRotation(Vector3& vecRotation)
{
	if(IsSpawned())
	{
		// Get the vehicle matrix
		Matrix matMatrix;
		m_pVehicle->GetMatrix(&matMatrix);

		// Convert the matrix to euler angles
		g_pClient->GetGame()->ConvertRotationMatrixToEulerAngles(&matMatrix, &vecRotation);

		// Convert the rotation from radians to degrees
		vecRotation.ConvertToDegrees();
	}
	else
		memset(&vecRotation, 0, sizeof(Vector3));
}

void CClientVehicle::SetMoveSpeed(const Vector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pVehicle->SetMoveSpeed((Vector3 *)&vecMoveSpeed);
}

void CClientVehicle::GetMoveSpeed(Vector3& vecMoveSpeed)
{
	if(IsSpawned())
		m_pVehicle->GetMoveSpeed(&vecMoveSpeed);
	else
		memset(&vecMoveSpeed, 0, sizeof(Vector3));
}

void CClientVehicle::SetTurnSpeed(const Vector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pVehicle->SetTurnSpeed((Vector3 *)&vecTurnSpeed);
}

void CClientVehicle::GetTurnSpeed(Vector3& vecTurnSpeed)
{
	if(IsSpawned())
		m_pVehicle->GetTurnSpeed(&vecTurnSpeed);
	else
		memset(&vecTurnSpeed, 0, sizeof(Vector3));
}

BYTE CClientVehicle::GetMaxPassengers()
{
	if(IsSpawned())
		return m_pVehicle->GetMaxPasssengers();

	return 0;
}

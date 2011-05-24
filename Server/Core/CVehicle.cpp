//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CVehicle.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CServer * g_pServer;

CVehicle::CVehicle(EntityId vehicleId, int iModelIndex) : CEntity(ENTITY_TYPE_VEHICLE, g_pServer->GetRootEntity(), "vehicle")
{
	m_vehicleId = vehicleId;
	m_iModelIndex = iModelIndex;
	m_bSpawned = false;
	Reset();
}

CVehicle::~CVehicle()
{

}

void CVehicle::Reset()
{
	memset(m_byteColors, 0, sizeof(m_byteColors));
	m_pDriver = NULL;
	memset(m_pPassengers, 0, sizeof(m_pPassengers));
	memset(&m_vecPosition, 0, sizeof(CVector3));
	memset(&m_vecRotation, 0, sizeof(CVector3));
	memset(&m_vecMoveSpeed, 0, sizeof(CVector3));
	memset(&m_vecTurnSpeed, 0, sizeof(CVector3));
	m_fHealth = 1000.0f;
}

void CVehicle::SpawnForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_vehicleId);
	bitStream.Write(m_iModelIndex);
	bitStream.Write((char *)m_byteColors, sizeof(m_byteColors));
	bitStream.Write(m_vecPosition);
	bitStream.Write(m_vecRotation);
	bitStream.Write(m_fHealth);

	// Send it to the player
	g_pServer->GetNetworkManager()->RPC(RPC_SPAWN_VEHICLE, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CVehicle::SpawnForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player active?
		if(g_pServer->GetPlayerManager()->IsActive(i))
		{
			// Spawn this vehicle for the current player
			SpawnForPlayer(i);
		}
	}

	// Mark us as spawned
	m_bSpawned = true;
}

void CVehicle::DestroyForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_vehicleId);

	// Send it to the player
	g_pServer->GetNetworkManager()->RPC(RPC_DESTROY_VEHICLE, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CVehicle::DestroyForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player active?
		if(g_pServer->GetPlayerManager()->IsActive(i))
		{
			// Destroy this vehicle for the current player
			DestroyForPlayer(i);
		}
	}

	// Mark us as not spawned
	m_bSpawned = false;
}

void CVehicle::SetDriver(CPlayer * pDriver)
{
	m_pDriver = pDriver;
}

CPlayer * CVehicle::GetDriver()
{
	return m_pDriver;
}

void CVehicle::SetPassenger(BYTE bytePassengerId, CPlayer * pPassenger)
{
	if(bytePassengerId < MAX_VEHICLE_PASSENGERS)
		return;

	m_pPassengers[bytePassengerId] = pPassenger;
}

CPlayer * CVehicle::GetPassenger(BYTE bytePassengerId)
{
	if(bytePassengerId >= MAX_VEHICLE_PASSENGERS)
		return NULL;

	return m_pPassengers[bytePassengerId];
}

void CVehicle::SetOccupant(BYTE byteSeatId, CPlayer * pOccupant)
{
	if(byteSeatId == 0)
		SetDriver(pOccupant);
	else
		SetPassenger((byteSeatId - 1), pOccupant);
}

CPlayer * CVehicle::GetOccupant(BYTE byteSeatId)
{
	if(byteSeatId == 0)
		return GetDriver();

	return GetPassenger(byteSeatId - 1);
}

void CVehicle::SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4)
{
	m_byteColors[0] = byteColor1;
	m_byteColors[1] = byteColor2;
	m_byteColors[2] = byteColor3;
	m_byteColors[3] = byteColor4;
}

void CVehicle::GetColors(BYTE& byteColor1, BYTE& byteColor2, BYTE& byteColor3, BYTE& byteColor4)
{
	byteColor1 = m_byteColors[0];
	byteColor2 = m_byteColors[1];
	byteColor3 = m_byteColors[2];
	byteColor4 = m_byteColors[3];
}

void CVehicle::SetPosition(const CVector3& vecPosition)
{
	m_vecPosition = vecPosition;
}

void CVehicle::GetPosition(CVector3& vecPosition)
{
	vecPosition = m_vecPosition;
}

void CVehicle::SetRotation(const CVector3& vecRotation)
{
	m_vecRotation = vecRotation;
}

void CVehicle::GetRotation(CVector3& vecRotation)
{
	vecRotation = m_vecRotation;
}

void CVehicle::SetMoveSpeed(const CVector3& vecMoveSpeed)
{
	m_vecMoveSpeed = vecMoveSpeed;
}

void CVehicle::GetMoveSpeed(CVector3& vecMoveSpeed)
{
	vecMoveSpeed = m_vecMoveSpeed;
}

void CVehicle::SetTurnSpeed(const CVector3& vecTurnSpeed)
{
	m_vecTurnSpeed = vecTurnSpeed;
}

void CVehicle::GetTurnSpeed(CVector3& vecTurnSpeed)
{
	vecTurnSpeed = m_vecTurnSpeed;
}

void CVehicle::SetHealth(float fHealth)
{
	m_fHealth = fHealth;
}

float CVehicle::GetHealth()
{
	return m_fHealth;
}

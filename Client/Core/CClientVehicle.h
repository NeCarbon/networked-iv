//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientVehicle.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// This is also a GTA limit
#define MAX_PASSENGERS 8

class CClientVehicle
{
private:
	EntityId         m_vehicleId;
	CIVVehicle     * m_pVehicle;
	DWORD            m_dwModelHash;
	CClientPlayer  * m_pDriver;
	CClientPlayer  * m_pPassengers[MAX_PASSENGERS];

public:
	CClientVehicle(DWORD dwModelHash);
	~CClientVehicle();

	bool             IsNetworkVehicle() { return (m_vehicleId != INVALID_ENTITY_ID); }
	void             SetVehicleId(EntityId vehicleId) { m_vehicleId = vehicleId; }
	EntityId         GetVehicleId() { return m_vehicleId; }
	CIVVehicle     * GetGameVehicle() { return m_pVehicle; }
	bool             IsSpawned() { return (m_pVehicle != NULL); }
	DWORD            GetModelHash() { return m_dwModelHash; }
	void             SetDriver(CClientPlayer * pDriver) { m_pDriver = pDriver; }
	CClientPlayer  * GetDriver() { return m_pDriver; }
	void             SetPassenger(BYTE bytePassengerId, CClientPlayer * pPassenger);
	CClientPlayer  * GetPassenger(BYTE bytePassengerId);
	void             SetOccupant(BYTE byteSeatId, CClientPlayer * pOccupant);
	CClientPlayer  * GetOccupant(BYTE byteSeatId);
	bool             Create();
	void             Destroy();
	unsigned int     GetScriptingHandle();
	void             SetHealth(float fHealth);
	float            GetHealth();
	void             Teleport(const Vector3& vecPosition);
	void             SetPosition(const Vector3& vecPosition);
	void             GetPosition(Vector3& vecPosition);
	void             SetRotation(const Vector3& vecRotation);
	void             GetRotation(Vector3& vecRotation);
	void             SetMoveSpeed(const Vector3& vecMoveSpeed);
	void             GetMoveSpeed(Vector3& vecMoveSpeed);
	void             SetTurnSpeed(const Vector3& vecTurnSpeed);
	void             GetTurnSpeed(Vector3& vecTurnSpeed);
	BYTE             GetMaxPassengers();
};

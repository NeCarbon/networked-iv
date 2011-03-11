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

class CClientVehicle : public CStreamableEntity
{
private:
	EntityId         m_vehicleId;
	CIVVehicle     * m_pVehicle;
	CIVModelInfo   * m_pModelInfo;
	BYTE             m_byteColors[4];
	CClientPlayer  * m_pDriver;
	CClientPlayer  * m_pPassengers[MAX_PASSENGERS];
	Vector3          m_vecPosition;
	Vector3          m_vecRotation;
	Vector3          m_vecMoveSpeed;
	Vector3          m_vecTurnSpeed;
	float            m_fHealth;

	bool             Create();
	void             Destroy();

public:
	CClientVehicle(DWORD dwModelHash);
	~CClientVehicle();

	bool             IsNetworkVehicle() { return (m_vehicleId != INVALID_ENTITY_ID); }
	void             SetVehicleId(EntityId vehicleId) { m_vehicleId = vehicleId; }
	EntityId         GetVehicleId() { return m_vehicleId; }
	CIVVehicle     * GetGameVehicle() { return m_pVehicle; }
	bool             IsSpawned() { return (m_pVehicle != NULL); }
	CIVModelInfo   * GetModelInfo() { return m_pModelInfo; }
	void             SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4);
	void             GetColors(BYTE& byteColor1, BYTE& byteColor2, BYTE& byteColor3, BYTE& byteColor4);
	void             SetDriver(CClientPlayer * pDriver) { m_pDriver = pDriver; }
	CClientPlayer  * GetDriver() { return m_pDriver; }
	void             SetPassenger(BYTE bytePassengerId, CClientPlayer * pPassenger);
	CClientPlayer  * GetPassenger(BYTE bytePassengerId);
	void             SetOccupant(BYTE byteSeatId, CClientPlayer * pOccupant);
	CClientPlayer  * GetOccupant(BYTE byteSeatId);
	void             StreamIn();
	void             StreamOut();
	unsigned int     GetScriptingHandle();
	void             SetHealth(float fHealth);
	float            GetHealth();

	void             Teleport(const Vector3& vecPosition);
	void             SetPosition(const Vector3& vecPosition);
	void             GetPosition(Vector3& vecPosition);
	void             GetStreamPosition(Vector3& vecPosition) { GetPosition(vecPosition); }
	void             SetRotation(const Vector3& vecRotation);
	void             GetRotation(Vector3& vecRotation);
	void             SetMoveSpeed(const Vector3& vecMoveSpeed);
	void             GetMoveSpeed(Vector3& vecMoveSpeed);
	void             SetTurnSpeed(const Vector3& vecTurnSpeed);
	void             GetTurnSpeed(Vector3& vecTurnSpeed);

	// Max passengers
	BYTE             GetMaxPassengers();
};

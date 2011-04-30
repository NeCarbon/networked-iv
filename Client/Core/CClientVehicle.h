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

class CClientVehicle : public CStreamableEntity, public CEntity
{
private:
	EntityId        m_vehicleId;
	CIVVehicle    * m_pVehicle;
	CIVModelInfo  * m_pModelInfo;
	BYTE            m_byteColors[4];
	CClientPlayer * m_pDriver;
	CClientPlayer * m_pPassengers[MAX_VEHICLE_PASSENGERS];
	CVector3         m_vecPosition;
	CVector3         m_vecRotation;
	CVector3         m_vecMoveSpeed;
	CVector3         m_vecTurnSpeed;
	float           m_fHealth;

	bool             Create();
	void             Destroy();

public:
	CClientVehicle(int iModelIndex);
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
	void             Process();
	bool             SetModel(int iModelIndex);
	unsigned int     GetScriptingHandle();
	void             SetHealth(float fHealth);
	float            GetHealth();

	void             Teleport(const CVector3& vecPosition);
	void             SetPosition(const CVector3& vecPosition);
	void             GetPosition(CVector3& vecPosition);
	void             GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }
	void             SetRotation(const CVector3& vecRotation);
	void             GetRotation(CVector3& vecRotation);
	void             SetMoveSpeed(const CVector3& vecMoveSpeed);
	void             GetMoveSpeed(CVector3& vecMoveSpeed);
	void             SetTurnSpeed(const CVector3& vecTurnSpeed);
	void             GetTurnSpeed(CVector3& vecTurnSpeed);

	// Max passengers
	BYTE             GetMaxPassengers();

	// Information Serialization/Deserialization
	void             Serialize(CBitStreamInterface * pBitStream);
	bool             Deserialize(CBitStreamInterface * pBitStream);
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CVehicle.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CVehicle : public CEntity
{
private:
	EntityId  m_vehicleId;
	int       m_iModelIndex;
	bool      m_bSpawned;
	BYTE      m_byteColors[4];
	CPlayer * m_pDriver;
	CPlayer * m_pPassengers[MAX_VEHICLE_PASSENGERS];
	CVector3   m_vecPosition;
	CVector3   m_vecRotation;
	CVector3   m_vecMoveSpeed;
	CVector3   m_vecTurnSpeed;
	float     m_fHealth;

public:
	CVehicle(EntityId vehicleId, int iModelIndex);
	~CVehicle();

	bool      IsSpawned() { return m_bSpawned; }
	void      Reset();
	void      SpawnForPlayer(EntityId playerId);
	void      SpawnForWorld();
	void      DestroyForPlayer(EntityId playerId);
	void      DestroyForWorld();
	void      SetDriver(CPlayer * pDriver);
	CPlayer * GetDriver();
	void      SetPassenger(BYTE bytePassengerId, CPlayer * pPassenger);
	CPlayer * GetPassenger(BYTE bytePassengerId);
	void      SetOccupant(BYTE byteSeatId, CPlayer * pOccupant);
	CPlayer * GetOccupant(BYTE byteSeatId);
	void      SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4);
	void      GetColors(BYTE& byteColor1, BYTE& byteColor2, BYTE& byteColor3, BYTE& byteColor4);
	void      SetPosition(const CVector3& vecPosition);
	void      GetPosition(CVector3& vecPosition);
	void      SetRotation(const CVector3& vecRotation);
	void      GetRotation(CVector3& vecRotation);
	void      SetMoveSpeed(const CVector3& vecMoveSpeed);
	void      GetMoveSpeed(CVector3& vecMoveSpeed);
	void      SetTurnSpeed(const CVector3& vecTurnSpeed);
	void      GetTurnSpeed(CVector3& vecTurnSpeed);
	void      SetHealth(float fHealth);
	float     GetHealth();
};

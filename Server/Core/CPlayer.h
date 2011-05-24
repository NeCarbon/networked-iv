//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayer.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

enum eState
{
	STATE_CONNECTED,
	STATE_INGAME
};

class CVehicle;

class CPlayer : public CEntity
{
private:
	EntityId         m_playerId;
	String           m_strName;
	String           m_strSerial;
	bool             m_bSpawned;
	eState           m_state;
	CNetworkPadState m_previousNetPadState;
	CNetworkPadState m_currentNetPadState;
	CVehicle       * m_pVehicle;
	BYTE             m_byteVehicleSeatId;
	CVector3         m_vecPosition;
	float            m_fHeading;
	CVector3         m_vecMoveSpeed;
	bool             m_bIsDucking;

public:
	CPlayer(EntityId playerId, String strName, String strSerial);
	~CPlayer();

	bool       IsOnFoot() { return (m_pVehicle == NULL); }
	bool       IsInVehicle() { return (m_pVehicle != NULL); }
	EntityId   GetPlayerId();
	String     GetName();
	String     GetIp();
	bool       IsSpawned();
	void       SetState(eState state) { m_state = state; }
	eState     GetState() { return m_state; }
	void       AddForPlayer(EntityId playerId);
	void       AddForWorld();
	void       DeleteForPlayer(EntityId playerId);
	void       DeleteForWorld();
	void       SpawnForPlayer(EntityId playerId);
	void       SpawnForWorld();
	void       DestroyForPlayer(EntityId playerId);
	void       DestroyForWorld();
	void       SetVehicle(CVehicle * pVehicle) { m_pVehicle = pVehicle; }
	CVehicle * GetVehicle() { return m_pVehicle; }
	void       SetVehicleSeatId(BYTE byteSeatId) { m_byteVehicleSeatId = byteSeatId; }
	BYTE       GetVehicleSeatId() { return m_byteVehicleSeatId; }
	void       SetDucking(bool bIsDucking) { m_bIsDucking = bIsDucking; }
	bool       IsDucking() { return m_bIsDucking; }
	String     GetSerial() { return m_strSerial; }

	// Information Serialization/Deserialization
	void       Serialize(CBitStream * pBitStream);
	bool       Deserialize(CBitStream * pBitStream);
};

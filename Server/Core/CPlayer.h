//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPlayer.h
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

class CClientPlayer : public CEntity
{
private:
	EntityId m_playerId;
	String m_strName;
	bool m_bSpawned;
	eState m_state;

public:
	CClientPlayer(EntityId playerId, String strName);
	~CClientPlayer();

	EntityId GetPlayerId();
	String   GetName();
	String   GetIp();
	bool     IsSpawned();
	void     SetState(eState state) { m_state = state; }
	eState   GetState() { return m_state; }
	void     AddForPlayer(EntityId playerId);
	void     AddForWorld();
	void     DeleteForPlayer(EntityId playerId);
	void     DeleteForWorld();
	void     SpawnForPlayer(EntityId playerId);
	void     SpawnForWorld();
	void     DestroyForPlayer(EntityId playerId);
	void     DestroyForWorld();
};

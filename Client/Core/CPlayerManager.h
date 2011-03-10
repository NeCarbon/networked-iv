//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayerManager.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CPlayerManager
{
private:
	CClientPlayer * m_pLocalPlayer;
	EntityId         m_localPlayerId;
	CClientPlayer * m_pNetworkPlayers[PLAYER_MAX];

public:
	CPlayerManager();
	~CPlayerManager();

	bool             Add(EntityId playerId, String strName);
	bool             Delete(EntityId playerId);
	void             Process();
	bool             IsActive(EntityId playerId);
	CClientPlayer * Get(EntityId playerId);
	EntityId         GetCount();
	CClientPlayer * GetLocalPlayer() { return m_pLocalPlayer; }
	void             SetLocalPlayerId(EntityId localPlayerId);
	EntityId         GetLocalPlayerId() { return m_localPlayerId; }
};

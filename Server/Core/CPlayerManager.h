//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayerManager.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CPlayerManager
{
private:
	CPlayer * m_pPlayers[PLAYER_MAX];

public:
	CPlayerManager();
	~CPlayerManager();

	bool      Add(EntityId playerId, String strName);
	bool      Delete(EntityId playerId);
	bool      IsActive(EntityId playerId);
	CPlayer * Get(EntityId playerId);
	EntityId  GetCount();
	void      HandlePlayerJoin(EntityId playerId);
};

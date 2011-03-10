//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPlayer.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager *  g_pPlayerManager;
extern CRootEntity *     g_pRootEntity;

CClientPlayer::CClientPlayer(EntityId playerId, String strName) : CEntity(ENTITY_TYPE_PLAYER, g_pRootEntity, "player")
{
	m_playerId = playerId;
	m_strName.SetLimit(NICK_MAX);
	m_strName = strName;
	m_bSpawned = false;
	m_state = STATE_CONNECTED;
}

CClientPlayer::~CClientPlayer()
{

}

EntityId CClientPlayer::GetPlayerId()
{
	return m_playerId;
}

String CClientPlayer::GetName()
{
	return m_strName;
}

String CClientPlayer::GetIp()
{
	return g_pNetworkManager->GetPlayerIp(m_playerId);
}

bool CClientPlayer::IsSpawned()
{
	return m_bSpawned;
}

void CClientPlayer::AddForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);
	bitStream.Write(m_strName);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_ADD_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CClientPlayer::AddForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pPlayerManager->IsActive(i))
		{
			// Add this player for the current player
			AddForPlayer(i);
		}
	}
}

void CClientPlayer::DeleteForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_DELETE_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

}
void CClientPlayer::DeleteForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pPlayerManager->IsActive(i))
		{
			// Delete this player for the current player
			DeleteForPlayer(i);
		}
	}
}

void CClientPlayer::SpawnForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_SPAWN_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CClientPlayer::SpawnForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pPlayerManager->IsActive(i))
		{
			// Spawn this player for the current player
			SpawnForPlayer(i);
		}
	}

	// Mark us as spawned
	m_bSpawned = true;
}

void CClientPlayer::DestroyForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_DESTROY_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CClientPlayer::DestroyForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pPlayerManager->IsActive(i))
		{
			// Destroy this player for the current player
			DestroyForPlayer(i);
		}
	}

	// Mark us as not spawned
	m_bSpawned = false;
}

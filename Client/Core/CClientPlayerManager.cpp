//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPlayerManager.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CClientPlayerManager::CClientPlayerManager()
{
	// Create the local player instance
	m_pLocalPlayer = new CClientPlayer(true);

	// Invalidate the local player id
	m_localPlayerId = INVALID_ENTITY_ID;

	// Loop through all network player pointers
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Set this network player pointer to NULL
		m_pNetworkPlayers[i] = NULL;
	}
}

CClientPlayerManager::~CClientPlayerManager()
{
	// Loop through all network player pointers
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is this network player pointer valid?
		if(m_pNetworkPlayers[i] && !m_pNetworkPlayers[i]->IsLocalPlayer())
		{
			// Delete this network player
			SAFE_DELETE(m_pNetworkPlayers[i]);
		}
	}

	// Delete the local player instance
	SAFE_DELETE(m_pLocalPlayer);
}

bool CClientPlayerManager::Add(EntityId playerId, String strName)
{
	// Is this player already added?
	if(IsActive(playerId))
		return false;

	// Create the network player instance
	m_pNetworkPlayers[playerId] = new CClientPlayer(false);

	// Was the network player instance created?
	if(m_pNetworkPlayers[playerId])
	{
		// Set the players id
		m_pNetworkPlayers[playerId]->SetPlayerId(playerId);

		// Set the players name
		m_pNetworkPlayers[playerId]->SetPlayerName(strName);
		return true;
	}

	return false;
}

bool CClientPlayerManager::Delete(EntityId playerId)
{
	// Is this player not active?
	if(!IsActive(playerId))
		return false;

	// Is this the local player?
	if(m_pNetworkPlayers[playerId]->IsLocalPlayer())
		return false;

	// Delete the network player instance
	SAFE_DELETE(m_pNetworkPlayers[playerId]);
	return true;
}

void CClientPlayerManager::Process()
{
	// Process the local player
	m_pLocalPlayer->Process();

	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player active?
		if(IsActive(i) && !m_pNetworkPlayers[i]->IsLocalPlayer())
		{
			// Process the current player
			m_pNetworkPlayers[i]->Process();
		}
	}
}

bool CClientPlayerManager::IsActive(EntityId playerId)
{
	// Is the player id invalid?
	if(playerId >= PLAYER_MAX)
		return false;

	return (m_pNetworkPlayers[playerId] != NULL);
}

CClientPlayer * CClientPlayerManager::Get(EntityId playerId)
{
	// Is this player not active?
	if(!IsActive(playerId))
		return NULL;

	return m_pNetworkPlayers[playerId];
}

EntityId CClientPlayerManager::GetCount()
{
	EntityId count = 0;

	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player active?
		if(IsActive(i))
		{
			// Increment the player count
			count++;
		}
	}

	return count;
}

void CClientPlayerManager::SetLocalPlayerId(EntityId localPlayerId)
{
	// Clear the current local player (if any)
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player active?
		if(IsActive(i))
		{
			// Is the current player the local player?
			if(m_pNetworkPlayers[i]->IsLocalPlayer())
			{
				// Clear the current player
				m_pNetworkPlayers[i] = NULL;
				break;
			}
			else
			{
				// Is the current players id the same as the new local player id?
				if(i == localPlayerId)
				{
					// Delete the current player
					SAFE_DELETE(m_pNetworkPlayers[i]);
					break;
				}
			}
		}
	}

	// Set the local player
	m_pNetworkPlayers[localPlayerId] = m_pLocalPlayer;

	// Set the local player id
	m_localPlayerId = localPlayerId;
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayer.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager *  g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CRootEntity *     g_pRootEntity;

CPlayer::CPlayer(EntityId playerId, String strName) : CEntity(ENTITY_TYPE_PLAYER, g_pRootEntity, "player")
{
	m_playerId = playerId;
	m_strName.SetLimit(NICK_MAX);
	m_strName = strName;
	m_bSpawned = false;
	m_state = STATE_CONNECTED;
	m_pVehicle = NULL;
	m_byteVehicleSeatId = 0;
}

CPlayer::~CPlayer()
{

}

EntityId CPlayer::GetPlayerId()
{
	return m_playerId;
}

String CPlayer::GetName()
{
	return m_strName;
}

String CPlayer::GetIp()
{
	return g_pNetworkManager->GetPlayerIp(m_playerId);
}

bool CPlayer::IsSpawned()
{
	return m_bSpawned;
}

void CPlayer::AddForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);
	bitStream.Write(m_strName);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_ADD_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CPlayer::AddForWorld()
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

void CPlayer::DeleteForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_DELETE_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

}
void CPlayer::DeleteForWorld()
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

void CPlayer::SpawnForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_SPAWN_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CPlayer::SpawnForWorld()
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

void CPlayer::DestroyForPlayer(EntityId playerId)
{
	// Construct the BitStream
	CBitStream bitStream;
	bitStream.WriteCompressed(m_playerId);

	// Send it to the player
	g_pNetworkManager->RPC(RPC_DESTROY_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CPlayer::DestroyForWorld()
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

void CPlayer::Serialize(CBitStream * pBitStream)
{
	// Write the player net pad state
	pBitStream->Write(m_currentNetPadState);

	// Write if we are on foot
	pBitStream->WriteBit(IsOnFoot());

	// Are we on foot?
	if(IsOnFoot())
	{
		// Write the player position
		pBitStream->Write(m_vecPosition);

		// Write the player heading
		pBitStream->Write(m_fHeading);

		// Write the player move speed
		pBitStream->Write(m_vecMoveSpeed);

		// Write the player duck state
		pBitStream->WriteBit(m_bIsDucking);
	}
	else
	{
		return;
		// Write our vehicle id
		pBitStream->WriteCompressed(m_pVehicle->GetVehicleId());

		// Write our vehicle seat id
		pBitStream->Write(m_byteVehicleSeatId);

		// Are we the driver?
		if(m_byteVehicleSeatId == 0)
		{
			// TODO
			// Serialize the vehicle to the bit stream
			//m_pVehicle->Serialize(pBitStream);
		}
	}
}

bool CPlayer::Deserialize(CBitStream * pBitStream)
{
	// Copy the current net pad state to the previous net pad state
	memcpy(&m_previousNetPadState, &m_currentNetPadState, sizeof(CNetworkPadState));

	// Read the net pad state
	if(!pBitStream->Read(m_currentNetPadState))
	{
		CLogFile::Printf("CPlayer::Deserialize fail (Error code 1)\n");
		return false;
	}

	// Read if we are on foot
	bool bIsOnFoot = pBitStream->ReadBit();

	// Are we on foot or in a vehicle when we are not meant to be?
	if(bIsOnFoot != IsOnFoot())
	{
		CLogFile::Printf("CPlayer::Deserialize fail (Error code 2)\n");
		return false;
	}

	// Are we on foot?
	if(bIsOnFoot)
	{
		// Read the player position
		if(!pBitStream->Read(m_vecPosition))
		{
			CLogFile::Printf("CPlayer::Deserialize fail (Error code 3)\n");
			return false;
		}

		// Read the player heading
		if(!pBitStream->Read(m_fHeading))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 4)\n");
			return false;
		}

		// Read the player move speed
		if(!pBitStream->Read(m_vecMoveSpeed))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 5)\n");
			return false;
		}

		// Read the player duck state
		m_bIsDucking = pBitStream->ReadBit();
	}
	else
	{
		return false;
		// Read the vehicle id
		EntityId vehicleId;

		if(!pBitStream->ReadCompressed(vehicleId))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 6)\n");
			return false;
		}

		// TODO: Check against vehicle id for validity?

		// Read the vehicle seat id
		BYTE byteSeatId;

		if(!pBitStream->Read(byteSeatId))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 7)\n");
			return false;
		}

		// TODO: Check against vehicle seat id for validity?

		// Get the vehicle pointer
		CVehicle * pVehicle = g_pVehicleManager->Get(vehicleId);

		// Is the vehicle pointer valid?
		if(!pVehicle)
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 8)\n");
			return false;
		}

		// Are we not already in the vehicle?
		/*if(m_pVehicle != pVehicle)
		{
			// Force us in to the vehicle
			PutInVehicle(pVehicle, byteSeatId);
		}*/

		// Are we the driver?
		/*if(byteSeatId == 0)
		{
			// Deserialize the vehicle from the bit stream
			if(!pVehicle->Deserialize(pBitStream))
			{
				CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 9)\n");
				return false;
			}
		}*/
	}

	return true;
}

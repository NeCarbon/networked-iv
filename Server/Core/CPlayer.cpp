//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayer.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CServer * g_pServer;

CPlayer::CPlayer(EntityId playerId, String strName, String strSerial) : CEntity(ENTITY_TYPE_PLAYER, g_pServer->GetRootEntity(), "player")
{
	m_playerId = playerId;
	m_strName.SetLimit(NICK_MAX);
	m_strName = strName;
	m_strSerial = strSerial;
	m_bSpawned = false;
	m_state = STATE_CONNECTED;
	m_pVehicle = NULL;
	m_byteVehicleSeatId = 0;
	m_uiHealth = 200;
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
	return g_pServer->GetNetworkManager()->GetPlayerIp(m_playerId);
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
	g_pServer->GetNetworkManager()->RPC(RPC_ADD_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CPlayer::AddForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pServer->GetPlayerManager()->IsActive(i))
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
	g_pServer->GetNetworkManager()->RPC(RPC_DELETE_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);

}
void CPlayer::DeleteForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pServer->GetPlayerManager()->IsActive(i))
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
	g_pServer->GetNetworkManager()->RPC(RPC_SPAWN_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CPlayer::SpawnForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pServer->GetPlayerManager()->IsActive(i))
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
	g_pServer->GetNetworkManager()->RPC(RPC_DESTROY_PLAYER, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

void CPlayer::DestroyForWorld()
{
	// Loop through all players
	for(EntityId i = 0; i < PLAYER_MAX; i++)
	{
		// Is the current player not this player and active?
		if(i != m_playerId && g_pServer->GetPlayerManager()->IsActive(i))
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

	// Write the player health
	pBitStream->Write(m_uiHealth);

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

		// Write the player current weapon
		pBitStream->Write(m_uiCurrentWeapon);
	}
	else
	{
		return;
		// Write our vehicle id
		pBitStream->WriteCompressed(m_pVehicle->GetVehicleId());

		// Write our vehicle seat id
		pBitStream->Write(m_byteVehicleSeatId);

		// Are we the driver?
		if(m_pVehicle->GetSyncer() == this)
		{
			// Serialize the vehicle
			m_pVehicle->Serialize(pBitStream);
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
		CLogFile::Printf("CPlayer::Deserialize fail (Error code 1)");
		return false;
	}

	// Read the health
	if(!pBitStream->Read(m_uiHealth))
	{
		CLogFile::Printf("CPlayer::Deserialize fail (Error code 10)");
		return false;
	}

	// Read if we are on foot
	bool bIsOnFoot = pBitStream->ReadBit();

	// Are we on foot or in a vehicle when we are not meant to be?
	if(bIsOnFoot != IsOnFoot())
	{
		CLogFile::Printf("CPlayer::Deserialize fail (Error code 2)");
		return false;
	}

	// Are we on foot?
	if(bIsOnFoot)
	{
		// Read the player position
		if(!pBitStream->Read(m_vecPosition))
		{
			CLogFile::Printf("CPlayer::Deserialize fail (Error code 3)");
			return false;
		}

		// Read the player heading
		if(!pBitStream->Read(m_fHeading))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 4)");
			return false;
		}

		// Read the player move speed
		if(!pBitStream->Read(m_vecMoveSpeed))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 5)");
			return false;
		}

		// Read the player duck state
		m_bIsDucking = pBitStream->ReadBit();

		// Read the player current weapon
		pBitStream->Read(m_uiCurrentWeapon);
	}
	else
	{
		// Read the vehicle id
		EntityId vehicleId;

		if(!pBitStream->ReadCompressed(vehicleId))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 6)");
			return false;
		}

		// Read the vehicle seat id
		BYTE byteSeatId;

		if(!pBitStream->Read(byteSeatId))
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 7)");
			return false;
		}

		// TODO: Check against vehicle seat id for validity?

		// Get the vehicle pointer
		CVehicle * pVehicle = g_pServer->GetVehicleManager()->Get(vehicleId);

		// Is the vehicle pointer valid?
		if(!pVehicle)
		{
			CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 8)");
			return false;
		}

		// Are we not already in the vehicle?
		/*if(m_pVehicle != pVehicle)
		{
			// Force us in to the vehicle
			PutInVehicle(pVehicle, byteSeatId);
		}*/

		// Are we responsible for syncing the vehicle?
		if(pVehicle->GetSyncer() == this)
		{
			// Check if there is any sync data existing
			if(pBitStream->GetNumberOfUnreadBits() > 0)
			{
				// Deserialize the vehicle from the bit stream
				if(!pVehicle->Deserialize(pBitStream))
				{
					CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 9)");
					return false;
				}
			}
			else
			{
				CLogFile::Printf("CClientPlayer::Deserialize fail (Error code 10)");
				return false;
			}
		}
	}

	return true;
}

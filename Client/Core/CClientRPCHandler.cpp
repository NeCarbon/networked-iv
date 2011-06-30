//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientRPCHandler.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

void CClientRPCHandler::InitialData(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got InitialData RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for InitialData RPC");
		return;
	}

	// Read the data the server sent us
	EntityId ourPlayerId;

	// Read our player id
	if(!pBitStream->ReadCompressed(ourPlayerId))
		return;

	// Set local player id
	g_pClient->GetPlayerManager()->SetLocalPlayerId(ourPlayerId);

	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connected to the server with player id %d", ourPlayerId);

	// Fade in the game screen
	g_pClient->GetGame()->FadeScreen(FADE_TYPE_IN, 0);
}

void CClientRPCHandler::AddPlayer(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got AddPlayer RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for AddPlayer RPC");
		return;
	}

	// Read the data the server sent us
	EntityId playerId;
	String strName;

	// Read the new player id
	if(!pBitStream->ReadCompressed(playerId))
		return;

	// Read the player name
	if(!pBitStream->Read(strName))
		return;

	// Add this player to the player manager
	if(g_pClient->GetPlayerManager()->Add(playerId, strName) == NULL)
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "CPlayerManager::Add failed for player %d (Name %s)!", playerId, strName.Get());

	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "%s (Id: %d) has connected to the server", strName.C_String(), playerId);
}

void CClientRPCHandler::DeletePlayer(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got DeletePlayer RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for DeletePlayer RPC");
		return;
	}

	// Read the data the server sent us
	EntityId playerId;

	// Read the new player id
	if(!pBitStream->ReadCompressed(playerId))
		return;

	// Get the network player pointer
	CClientPlayer * pNetworkPlayer = g_pClient->GetPlayerManager()->Get(playerId);

	// Is the network player pointer valid?
	if(pNetworkPlayer)
	{
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "%s (Id: %d) has disconnected from the server", pNetworkPlayer->GetPlayerName().Get(), playerId);

		// Delete this player from the player manager
		if(!g_pClient->GetPlayerManager()->Delete(playerId))
			g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "CPlayerManager::Delete failed for player %d!", playerId);
	}
	else
	{
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Invalid player (Id: %d) has disconnected to the server", playerId);
	}
}

void CClientRPCHandler::SpawnPlayer(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got SpawnPlayer RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for SpawnPlayer RPC");
		return;
	}

	// Read the data the server sent us
	EntityId inputPlayerId;

	// Read the input player id
	if(!pBitStream->ReadCompressed(inputPlayerId))
		return;

	// Get the network player pointer
	CClientPlayer * pPlayer = g_pClient->GetPlayerManager()->Get(inputPlayerId);

	// Is the network player pointer valid?
	if(pPlayer)
	{
		// Set the player can be streamed in flag
		pPlayer->SetCanBeStreamedIn(true);

		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Spawned player %d (%s)", inputPlayerId, pPlayer->GetPlayerName().Get());
	}
}

void CClientRPCHandler::DestroyPlayer(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got DestroyPlayer RPC from server");
}

void CClientRPCHandler::SpawnVehicle(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got SpawnVehicle RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for SpawnVehicle RPC");
		return;
	}

	// Read the vehicle id
	EntityId vehicleId;

	if(!pBitStream->ReadCompressed(vehicleId))
		return;

	// Read the vehicle model
	int iModelIndex;

	if(!pBitStream->Read(iModelIndex))
		return;

	// Read the vehicle colors
	BYTE byteColors[4];

	if(!pBitStream->Read((char *)byteColors, sizeof(byteColors)))
		return;

	// Read the vehicle position
	CVector3 vecPosition;

	if(!pBitStream->Read(vecPosition))
		return;

	// Read the vehicle rotation
	CVector3 vecRotation;

	if(!pBitStream->Read(vecRotation))
		return;

	// Read the vehicle health
	float fHealth;

	if(!pBitStream->Read(fHealth))
		return;

	// Add this vehicle to the vehicle manager
	CClientVehicle * pVehicle = g_pClient->GetVehicleManager()->Add(vehicleId, iModelIndex);

	if(pVehicle == NULL)
	{
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "CVehicleManager::Add failed for vehicle %d (Model %d)!", vehicleId, iModelIndex);
		return;
	}

	// Set the vehicle colors
	pVehicle->SetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);

	// Set the vehicle position
	pVehicle->SetPosition(vecPosition);

	// Set the vehicle rotation
	pVehicle->SetRotation(vecRotation);

	// Set the vehicle health
	pVehicle->SetHealth(fHealth);

	// Set the vehicle can be streamed in flag
	pVehicle->SetCanBeStreamedIn(true);
	
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Spawned vehicle %d (Model %d)", vehicleId, iModelIndex);
}

void CClientRPCHandler::DestroyVehicle(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got DestroyVehicle RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for DestroyVehicle RPC");
		return;
	}

	// Read the vehicle id
	EntityId vehicleId;

	if(!pBitStream->ReadCompressed(vehicleId))
		return;

	// Delete this vehicle from the vehicle manager
	if(!g_pClient->GetVehicleManager()->Delete(vehicleId))
	{
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "CVehicleManager::Delete failed for vehicle %d!", vehicleId);
		return;
	}

	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Destroyed vehicle %d", vehicleId);
}

void CClientRPCHandler::ChatInput(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for ChatInput RPC");
		return;
	}

	// Read the data the server sent us
	EntityId inputPlayerId;
	String strInput;

	// Read the input player id
	if(!pBitStream->ReadCompressed(inputPlayerId))
		return;

	// Read the input
	if(!pBitStream->Read(strInput))
		return;

	// Get the network player pointer
	CClientPlayer * pPlayer = g_pClient->GetPlayerManager()->Get(inputPlayerId);

	// Is the network player pointer valid?
	if(pPlayer)
	{
		// TODO: MESSAGE_CHAT_COLOR
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "%s (Id: %d): %s (Length: %d)", pPlayer->GetPlayerName().Get(), inputPlayerId, strInput.Get(), strInput.GetLength());
	}
}

void CClientRPCHandler::VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got VehicleEnterExit RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for VehicleEnterExit RPC");
		return;
	}

	// Read the player id
	EntityId playerId = INVALID_ENTITY_ID;

	if(!pBitStream->ReadCompressed(playerId))
		return;

	// Read the reply
	bool bReply = pBitStream->ReadBit();

	// If the reply is ok read the reply type
	BYTE byteEnterExitVehicleType = VEHICLE_ENTRY_EXIT_INVALID;

	if(bReply && !pBitStream->Read(byteEnterExitVehicleType))
		return;

	// If the reply is ok read the vehicle id
	EntityId vehicleId = INVALID_ENTITY_ID;

	if(bReply && !pBitStream->Read(vehicleId))
		return;

	// If the reply is ok and the reply type is enter return read the seat id
	BYTE byteSeatId = 0;

	if(bReply && (byteEnterExitVehicleType == VEHICLE_ENTRY_RETURN) && !pBitStream->Read(byteSeatId))
		return;

	CLogFile::Printf("VehicleEntryExit(Player %d, Reply %d, Type %d, Vehicle %d, Seat %d)", playerId, bReply, byteEnterExitVehicleType, vehicleId, byteSeatId);

	// Get the player pointer
	CClientPlayer * pPlayer = NULL;

	// Is it for us?
	if(g_pClient->GetPlayerManager()->GetLocalPlayerId() == playerId)
	{
		// Get the local player pointer
		pPlayer = g_pClient->GetPlayerManager()->GetLocalPlayer();
	}
	else
	{
		// Get the remote player pointer
		pPlayer = g_pClient->GetPlayerManager()->Get(playerId);
	}

	// Set no longer requesting a vehicle entry/exit
	pPlayer->SetRequestingVehicleEnterExit(false);

	// Get the vehicle pointer
	CClientVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	// Is the reply ok?
	if(bReply)
	{
		// Is it an entry reply
		if(byteEnterExitVehicleType == VEHICLE_ENTRY_RETURN)
		{
			// Enter the vehicle
			pPlayer->EnterVehicle(pVehicle, byteSeatId);
		}
		// Is it an entry cancellation?
		if(byteEnterExitVehicleType == VEHICLE_ENTRY_CANCELLED)
		{
			// If they are already in the vehicle, remove them, 
			// if not cancel their vehicle entry task
			if(pPlayer->IsInVehicle())
				pPlayer->ExitVehicle(true);
			else
				pPlayer->ClearPrimaryTask(true);
		}
		// Is it an exit reply?
		else if(byteEnterExitVehicleType == VEHICLE_EXIT_RETURN)
		{
			// Exit the vehicle
			pPlayer->ExitVehicle(false);
		}
	}
}

void CClientRPCHandler::PlayerSync(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	//g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got PlayerSync RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for PlayerSync RPC");
		return;
	}

	// Read the player id
	EntityId playerId;

	if(!pBitStream->ReadCompressed(playerId))
		return;

	// Get the network player pointer
	CClientPlayer * pPlayer = g_pClient->GetPlayerManager()->Get(playerId);

	// Is the network player pointer valid?
	if(pPlayer)
	{
		// Deserialize the player from the bit stream
		if(!pPlayer->Deserialize(pBitStream))
		{
#ifdef NIV_DEBUG
			g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Invalid player sync packet from player %d!", playerId);
#endif
		}
	}
}

void CClientRPCHandler::Register()
{
	AddFunction(RPC_INITIAL_DATA, InitialData);
	AddFunction(RPC_ADD_PLAYER, AddPlayer);
	AddFunction(RPC_DELETE_PLAYER, DeletePlayer);
	AddFunction(RPC_SPAWN_PLAYER, SpawnPlayer);
	AddFunction(RPC_DESTROY_PLAYER, DestroyPlayer);
	AddFunction(RPC_SPAWN_VEHICLE, SpawnVehicle);
	AddFunction(RPC_DESTROY_VEHICLE, DestroyVehicle);
	AddFunction(RPC_CHAT_INPUT, ChatInput);
	AddFunction(RPC_VEHICLE_ENTER_EXIT, VehicleEnterExit);
	AddFunction(RPC_PLAYER_SYNC, PlayerSync);
}

void CClientRPCHandler::Unregister()
{
	RemoveFunction(RPC_INITIAL_DATA);
	RemoveFunction(RPC_ADD_PLAYER);
	RemoveFunction(RPC_DELETE_PLAYER);
	RemoveFunction(RPC_SPAWN_PLAYER);
	RemoveFunction(RPC_DESTROY_PLAYER);
	RemoveFunction(RPC_SPAWN_VEHICLE);
	RemoveFunction(RPC_DESTROY_VEHICLE);
	RemoveFunction(RPC_CHAT_INPUT);
	RemoveFunction(RPC_VEHICLE_ENTER_EXIT);
	RemoveFunction(RPC_PLAYER_SYNC);
}

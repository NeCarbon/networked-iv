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

void CClientRPCHandler::InitialData(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got InitialData RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for InitialData RPC\n");
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

void CClientRPCHandler::AddPlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got AddPlayer RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for AddPlayer RPC\n");
		return;
	}

	// Read the data the server sent us
	EntityId newPlayerId;
	String strName;

	// Read the new player id
	if(!pBitStream->ReadCompressed(newPlayerId))
		return;

	// Read the player name
	if(!pBitStream->Read(strName))
		return;

	// Add this player to the player manager
	if(!g_pClient->GetPlayerManager()->Add(newPlayerId, strName))
	{
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "CPlayerManager::Add failed!");
	}

	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "%s (Id: %d) has connected to the server", strName.C_String(), newPlayerId);
}

void CClientRPCHandler::DeletePlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got DeletePlayer RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for DeletePlayer RPC\n");
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
		g_pClient->GetPlayerManager()->Delete(playerId);
	}
	else
	{
		g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Invalid player (Id: %d) has disconnected to the server", playerId);
	}
}

void CClientRPCHandler::SpawnPlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got SpawnPlayer RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for SpawnPlayer RPC\n");
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

void CClientRPCHandler::DestroyPlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got DestroyPlayer RPC from server");
}

void CClientRPCHandler::ChatInput(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for ChatInput RPC\n");
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

	// Set the message name to default
	String strMessageName = "Invalid player";

	// Get the network player pointer
	CClientPlayer * pPlayer = g_pClient->GetPlayerManager()->Get(inputPlayerId);

	// Is the network player pointer valid?
	if(pPlayer)
	{
		// Set the message name
		strMessageName = pPlayer->GetPlayerName();
	}

	// TODO: MESSAGE_CHAT_COLOR
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "%s (Id: %d): %s (%d)", strMessageName.Get(), inputPlayerId, strInput.Get(), strInput.GetLength());
}

void CClientRPCHandler::PlayerSync(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	//g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Got PlayerSync RPC from server");

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for PlayerSync RPC\n");
		return;
	}

	// Read the data the server sent us
	EntityId playerId;

	// Read the player id
	if(!pBitStream->ReadCompressed(playerId))
		return;

	// Read the player net pad state
	CNetworkPadState netPadState;

	if(!pBitStream->Read(netPadState))
		return;

	// Read the player position
	Vector3 vecPosition;

	if(!pBitStream->Read(vecPosition))
		return;

	// Read the player rotation
	Vector3 vecRotation;

	if(!pBitStream->Read(vecRotation))
		return;

	// Read the player move speed
	Vector3 vecMoveSpeed;

	if(!pBitStream->Read(vecMoveSpeed))
		return;

	// Read the player turn speed
	Vector3 vecTurnSpeed;

	if(!pBitStream->Read(vecTurnSpeed))
		return;

	// Get the network player pointer
	CClientPlayer * pPlayer = g_pClient->GetPlayerManager()->Get(playerId);

	// Is the network player pointer valid?
	if(pPlayer)
	{
		pPlayer->SetNetPadState(netPadState);
		pPlayer->SetTargetPosition(vecPosition, NETWORK_TICK_RATE);
		//pPlayer->SetTargetRotation(vecRotation, NETWORK_TICK_RATE);
		pPlayer->SetRotation(vecRotation);
		pPlayer->SetMoveSpeed(vecMoveSpeed);
		pPlayer->SetTurnSpeed(vecTurnSpeed);
	}
}

void CClientRPCHandler::Register()
{
	AddFunction(RPC_INITIAL_DATA, InitialData);
	AddFunction(RPC_ADD_PLAYER, AddPlayer);
	AddFunction(RPC_DELETE_PLAYER, DeletePlayer);
	AddFunction(RPC_SPAWN_PLAYER, SpawnPlayer);
	AddFunction(RPC_DESTROY_PLAYER, DestroyPlayer);
	AddFunction(RPC_CHAT_INPUT, ChatInput);
	AddFunction(RPC_PLAYER_SYNC, PlayerSync);
}

void CClientRPCHandler::Unregister()
{
	RemoveFunction(RPC_INITIAL_DATA);
	RemoveFunction(RPC_ADD_PLAYER);
	RemoveFunction(RPC_DELETE_PLAYER);
	RemoveFunction(RPC_SPAWN_PLAYER);
	RemoveFunction(RPC_DESTROY_PLAYER);
	RemoveFunction(RPC_CHAT_INPUT);
	RemoveFunction(RPC_PLAYER_SYNC);
}

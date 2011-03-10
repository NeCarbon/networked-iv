//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerRPCHandler.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager *  g_pPlayerManager;

void CServerRPCHandler::InitialData(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	CLogFile::Printf("Got InitialData RPC from player %d\n", senderSocket.playerId);

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for InitialData RPC\n");
		return;
	}

	// Read the data they sent us
	String strName;
	
	// Read the name
	if(!pBitStream->Read(strName))
		return;

	// Add them to the player manager
	g_pPlayerManager->Add(senderSocket.playerId, strName);

	// Construct the initial data bit stream
	CBitStream bitStream;

	// Write their player id
	bitStream.WriteCompressed(senderSocket.playerId);

	// Send the initial data bit stream
	g_pNetworkManager->RPC(RPC_INITIAL_DATA, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, senderSocket.playerId, false);

	// Send them all current players
	g_pPlayerManager->HandlePlayerJoin(senderSocket.playerId);

	// Spawn them for all current players
	g_pPlayerManager->Get(senderSocket.playerId)->SpawnForWorld();

	CLogFile::Printf("Player %d has joined the game (Name: %s)\n", senderSocket.playerId, strName.C_String());
}

void CServerRPCHandler::ChatInput(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	CLogFile::Printf("Got ChatInput RPC from player %d\n", senderSocket.playerId);

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for ChatInput RPC\n");
		return;
	}

	// Read the data they sent us
	bool bIsCommand;
	String strInput;

	// Read if its a command or not
	bIsCommand = pBitStream->ReadBit();

	// Read the input
	if(!pBitStream->Read(strInput))
		return;

	// Is it not a command?
	if(!bIsCommand)
	{
		// Trigger the event, if it is canceled, don't output the line to other players
		CClientPlayer* pPlayer = g_pPlayerManager->Get(senderSocket.playerId);
		CSquirrelArguments* pArguments = new CSquirrelArguments();
		pArguments->push(strInput);
		if(pPlayer && pPlayer->CallEvent("playerChat", pArguments))
		{
			// Construct the chat input bit stream
			CBitStream bitStream;

			// Write the player id
			bitStream.WriteCompressed(senderSocket.playerId);

			// Write the input
			bitStream.Write(strInput);

			// Send it to all other players
			g_pNetworkManager->RPC(RPC_CHAT_INPUT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		}
		delete pArguments;
	}

	CLogFile::Printf("Recieved chat input from player %d (Command?: %s, Input: %s)\n", senderSocket.playerId, bIsCommand ? "Yes" : "No", strInput.C_String());
}

void CServerRPCHandler::PlayerSync(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket)
{
	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for PlayerSync RPC\n");
		return;
	}

	// Get the player pointer
	CClientPlayer * pPlayer = g_pPlayerManager->Get(senderSocket.playerId);

	// Is the player pointer valid?
	if(pPlayer)
	{
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

		// Construct the bit stream
		CBitStream bitStream;

		// Write the player id
		bitStream.WriteCompressed(senderSocket.playerId);

		// Write the player net pad state
		bitStream.Write(netPadState);

		// Write the player position
		bitStream.Write(vecPosition);

		// Write the player rotation
		bitStream.Write(vecRotation);

		// Write the player move speed
		bitStream.Write(vecMoveSpeed);

		// Write the player turn speed
		bitStream.Write(vecTurnSpeed);

		// Send it to all other players
		g_pNetworkManager->RPC(RPC_PLAYER_SYNC, &bitStream, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED, senderSocket.playerId, true);
	}
}

void CServerRPCHandler::Register()
{
	CLogFile::Printf("Registering server RPCs\n");
	AddFunction(RPC_INITIAL_DATA, InitialData);
	AddFunction(RPC_CHAT_INPUT, ChatInput);
	AddFunction(RPC_PLAYER_SYNC, PlayerSync);
	CLogFile::Printf("Server RPCs registered\n");
}

void CServerRPCHandler::Unregister()
{
	CLogFile::Printf("Unregistering server RPCs\n");
	RemoveFunction(RPC_INITIAL_DATA);
	RemoveFunction(RPC_CHAT_INPUT);
	RemoveFunction(RPC_PLAYER_SYNC);
	CLogFile::Printf("Server RPCs unregistered\n");
}

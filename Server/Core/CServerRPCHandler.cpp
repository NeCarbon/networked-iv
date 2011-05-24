//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerRPCHandler.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CServer * g_pServer;

void CServerRPCHandler::InitialData(CBitStream * pBitStream, CPlayerSocket senderSocket)
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
	g_pServer->GetPlayerManager()->Add(senderSocket.playerId, strName, senderSocket.GetSerial());

	// Construct the initial data bit stream
	CBitStream bitStream;

	// Write their player id
	bitStream.WriteCompressed(senderSocket.playerId);

	// Send the initial data bit stream
	g_pServer->GetNetworkManager()->RPC(RPC_INITIAL_DATA, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, senderSocket.playerId, false);

	// Send them all current players
	g_pServer->GetPlayerManager()->HandlePlayerJoin(senderSocket.playerId);

	// Send them all current vehicles
	g_pServer->GetVehicleManager()->HandlePlayerJoin(senderSocket.playerId);

	// Spawn them for all current players
	g_pServer->GetPlayerManager()->Get(senderSocket.playerId)->SpawnForWorld();

	CLogFile::Printf("Player %d has joined the game (Name: %s)\n", senderSocket.playerId, strName.C_String());
}

void CServerRPCHandler::ChatInput(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	CLogFile::Printf("Got ChatInput RPC from player %d\n", senderSocket.playerId);

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for ChatInput RPC\n");
		return;
	}

	// Get the player pointer
	CPlayer * pPlayer = g_pServer->GetPlayerManager()->Get(senderSocket.playerId);

	// Is the player pointer valid?
	if(pPlayer)
	{
		// Read the data they sent us
		bool bIsCommand;
		String strInput;

		// Read if its a command or not
		bIsCommand = pBitStream->ReadBit();

		// Read the input
		if(!pBitStream->Read(strInput))
			return;

		// Prepare the event arguments
		CSquirrelArguments arguments;
		arguments.push(strInput);

		// Is it not a command?
		if(!bIsCommand)
		{
			// Trigger the event, if it is canceled, don't output the line to other players
			if(pPlayer->CallEvent("playerChat", &arguments))
			{
				// Construct the chat input bit stream
				CBitStream bitStream;

				// Write the player id
				bitStream.WriteCompressed(senderSocket.playerId);

				// Write the input
				bitStream.Write(strInput);

				// Send it to all other players
				g_pServer->GetNetworkManager()->RPC(RPC_CHAT_INPUT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			}
		}
		else
		{
			// Trigger the event
			pPlayer->CallEvent("playerCommand", &arguments);
		}

		CLogFile::Printf("Recieved chat input from player %d (Command?: %s, Input: %s)\n", senderSocket.playerId, bIsCommand ? "Yes" : "No", strInput.C_String());
	}
}

void CServerRPCHandler::VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	CLogFile::Printf("Got VehicleEnterExit RPC from player %d\n", senderSocket.playerId);

	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for VehicleEnterExit RPC\n");
		return;
	}

	// Get the player pointer
	CPlayer * pPlayer = g_pServer->GetPlayerManager()->Get(senderSocket.playerId);

	// Is the player pointer valid?
	if(pPlayer)
	{
		// Read the vehicle entry/exit type
		BYTE byteVehicleEntryExitType;

		if(!pBitStream->Read(byteVehicleEntryExitType))
			return;

		// Read the vehicle id
		EntityId vehicleId;

		if(!pBitStream->ReadCompressed(vehicleId))
			return;

		// Get the vehicle
		CVehicle * pVehicle = g_pServer->GetVehicleManager()->Get(vehicleId);

		// Does the vehicle not exist?
		if(!pVehicle)
			return;

		// Is this an entry request?
		if(byteVehicleEntryExitType == VEHICLE_ENTRY_REQUEST)
		{
			// Read the seat id
			BYTE byteSeatId;

			if(!pBitStream->Read(byteSeatId))
				return;

			// Get the reply
			CSquirrelArguments arguments;
			arguments.push(vehicleId);
			arguments.push(byteSeatId);
			bool bReply = pPlayer->CallEvent("vehicleEntryRequest", &arguments);

			// Reply to the vehicle entry request
			CBitStream bitStream;
			bitStream.WriteCompressed(senderSocket.playerId);
			bitStream.WriteBit(bReply);

			// Was the reply ok?
			if(bReply)
			{
				bitStream.Write((BYTE)VEHICLE_ENTRY_RETURN);
				bitStream.Write(vehicleId);
				bitStream.Write(byteSeatId);
				g_pServer->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
			}
		}
		// Is this an entry cancellation?
		if(byteVehicleEntryExitType == VEHICLE_ENTRY_CANCELLED)
		{
			// Read the seat id
			BYTE byteSeatId;

			if(!pBitStream->Read(byteSeatId))
				return;

			// Call the event
			CSquirrelArguments arguments;
			arguments.push(vehicleId);
			arguments.push(byteSeatId);
			pPlayer->CallEvent("vehicleEntryCancelled", &arguments);

			CBitStream bitStream;
			bitStream.WriteCompressed(senderSocket.playerId);
			bitStream.WriteBit(true);
			bitStream.Write((BYTE)VEHICLE_ENTRY_CANCELLED);
			bitStream.Write(vehicleId);
			g_pServer->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE, senderSocket.playerId, true);
		}
		// Is this an entry completion?
		else if(byteVehicleEntryExitType == VEHICLE_ENTRY_COMPLETE)
		{
			// Read the seat id
			BYTE byteSeatId;

			if(!pBitStream->Read(byteSeatId))
				return;

			// Call the event
			CSquirrelArguments arguments;
			arguments.push(vehicleId);
			arguments.push(byteSeatId);
			pPlayer->CallEvent("vehicleEntryComplete", &arguments);

			// Set the player vehicle and seat id
			pPlayer->SetVehicle(pVehicle);
			pPlayer->SetVehicleSeatId(byteSeatId);

			// Set the vehicle occupant
			pVehicle->SetOccupant(byteSeatId, pPlayer);
		}
		// Is this an exit request?
		else if(byteVehicleEntryExitType == VEHICLE_EXIT_REQUEST)
		{
			// Get the reply
			CSquirrelArguments arguments;
			arguments.push(vehicleId);
			bool bReply = pPlayer->CallEvent("vehicleExitRequest", &arguments);

			// Reply to the vehicle exit request
			CBitStream bitStream;
			bitStream.WriteCompressed(senderSocket.playerId);
			bitStream.WriteBit(bReply);

			// Was the reply ok?
			if(bReply)
			{
				bitStream.Write((BYTE)VEHICLE_EXIT_RETURN);
				bitStream.Write(vehicleId);
				g_pServer->GetNetworkManager()->RPC(RPC_VEHICLE_ENTER_EXIT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
			}
		}
		// Is this an exit completion?
		else if(byteVehicleEntryExitType == VEHICLE_EXIT_COMPLETE)
		{
			// Call the event
			CSquirrelArguments arguments;
			arguments.push(vehicleId);
			arguments.push(pPlayer->GetVehicleSeatId());
			pPlayer->CallEvent("vehicleExitComplete", &arguments);

			// Reset the vehicle occupant
			pVehicle->SetOccupant(pPlayer->GetVehicleSeatId(), NULL);

			// Reset the player vehicle and seat id
			pPlayer->SetVehicle(NULL);
			pPlayer->SetVehicleSeatId(0);
		}
		// Is this a forceful exit?
		else if(byteVehicleEntryExitType == VEHICLE_EXIT_FORCEFUL)
		{
			// Call the event
			CSquirrelArguments arguments;
			arguments.push(vehicleId);
			arguments.push(pPlayer->GetVehicleSeatId());
			pPlayer->CallEvent("vehicleForcefulExit", &arguments);

			// Reset the vehicle occupant
			pVehicle->SetOccupant(pPlayer->GetVehicleSeatId(), NULL);

			// Reset the player vehicle and seat id
			pPlayer->SetVehicle(NULL);
			pPlayer->SetVehicleSeatId(0);
		}
	}
}

void CServerRPCHandler::PlayerSync(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	// Ensure we have a valid bitstream
	if(!pBitStream)
	{
		CLogFile::Printf("Warning: Invalid bitstream for PlayerSync RPC\n");
		return;
	}

	// Get the player pointer
	CPlayer * pPlayer = g_pServer->GetPlayerManager()->Get(senderSocket.playerId);

	// Is the player pointer valid?
	if(pPlayer)
	{
		// Deserialize the player from the bit stream
		pPlayer->Deserialize(pBitStream);

		// Construct the bit stream
		CBitStream bitStream;

		// Write the player id
		bitStream.WriteCompressed(pPlayer->GetPlayerId());

		// Serialize the player to the bit stream
		pPlayer->Serialize(&bitStream);

		// Send it to all other players
		g_pServer->GetNetworkManager()->RPC(RPC_PLAYER_SYNC, &bitStream, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED, senderSocket.playerId, true);
	}
}

void CServerRPCHandler::Register()
{
	CLogFile::Printf("Registering server RPCs\n");
	AddFunction(RPC_INITIAL_DATA, InitialData);
	AddFunction(RPC_CHAT_INPUT, ChatInput);
	AddFunction(RPC_VEHICLE_ENTER_EXIT, VehicleEnterExit);
	AddFunction(RPC_PLAYER_SYNC, PlayerSync);
	CLogFile::Printf("Server RPCs registered\n");
}

void CServerRPCHandler::Unregister()
{
	CLogFile::Printf("Unregistering server RPCs\n");
	RemoveFunction(RPC_INITIAL_DATA);
	RemoveFunction(RPC_CHAT_INPUT);
	RemoveFunction(RPC_VEHICLE_ENTER_EXIT);
	RemoveFunction(RPC_PLAYER_SYNC);
	CLogFile::Printf("Server RPCs unregistered\n");
}

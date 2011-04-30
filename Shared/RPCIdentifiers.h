//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: RPCIdentifiers.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// Type used for rpc ids
typedef unsigned char RPCIdentifier;

enum eRPCIdentfiers
{
	// Request/send initial client data
	RPC_INITIAL_DATA,

	// Add a player
	RPC_ADD_PLAYER,

	// Delete a player
	RPC_DELETE_PLAYER,

	// Spawn a player
	RPC_SPAWN_PLAYER,

	// Destroy a player
	RPC_DESTROY_PLAYER,

	// Spawn a vehicle
	RPC_SPAWN_VEHICLE,

	// Destroy a vehicle
	RPC_DESTROY_VEHICLE,

	// Send chat input
	RPC_CHAT_INPUT,

	// Player vehicle entry/exit handling
	RPC_VEHICLE_ENTER_EXIT,

	// Update sync data for a player
	RPC_PLAYER_SYNC
};

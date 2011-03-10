//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPools.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define VAR_PedPool_7 0x18A82AC
#define VAR_VehiclePool_7 0x1619240
#define FUNC_CPool__AtHandle_7 0x426700
#define FUNC_CPool__HandleOf_7 0x447230
#define FUNC_CPool__Allocate_7 0x96D520
#define VAR_PlayerInfoArray 0x11A7008
#define VAR_LocalPlayerId_7 0xF1CC68
#define PLAYER_INFO_ARRAY_SIZE 32

class CPools
{
public:
	// Peds
	static unsigned int   GetHandleFromPed(IVPed * pPed);
	static IVPed *        GetPedFromHandle(unsigned int uiHandle);

	// Vehicles
	static unsigned int   GetHandleFromVehicle(IVVehicle * pVehicle);
	static IVVehicle *    GetVehicleFromHandle(unsigned int uiHandle);

	// Player Infos (An array not a pool)
	static IVPlayerInfo * GetPlayerInfoFromIndex(unsigned int uiIndex);
	static IVPlayerInfo * GetPlayerInfoFromPlayerPed(IVPlayerPed * pPlayerPed);
	static unsigned int   GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo);
	static void           SetPlayerInfoAtIndex(unsigned int uiIndex, IVPlayerInfo * pPlayerInfo);
	static unsigned int   FindFreePlayerInfoIndex();

	// Current Player Info (Doesn't really belong here, but it was the only place to put it)
	static unsigned int   GetLocalPlayerIndex();
	static void           SetLocalPlayerIndex(unsigned int uiIndex);
};

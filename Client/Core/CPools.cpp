//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPools.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

unsigned int CPools::GetHandleFromPed(IVPed * pPed)
{
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPool__HandleOf_7);
	DWORD dwPedPool = *(DWORD *)(g_pClient->GetBaseAddress() + VAR_PedPool_7);
	unsigned int uiHandle = 0;
	_asm
	{
		mov ecx, dwPedPool
		push pPed
		call dwFunc
		mov uiHandle, eax
	}
	return uiHandle;
}

IVPed * CPools::GetPedFromHandle(unsigned int uiHandle)
{
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPool__AtHandle_7);
	DWORD dwPedPool = *(DWORD *)(g_pClient->GetBaseAddress() + VAR_PedPool_7);
	IVPed * pPed = NULL;
	_asm
	{
		mov ecx, dwPedPool
		push uiHandle
		call dwFunc
		mov pPed, eax
	}
	return pPed;
}

unsigned int CPools::GetHandleFromVehicle(IVVehicle * pVehicle)
{
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPool__HandleOf_7);
	DWORD dwVehiclePool = *(DWORD *)(g_pClient->GetBaseAddress() + VAR_VehiclePool_7);
	unsigned int uiHandle = 0;
	_asm
	{
		mov ecx, dwVehiclePool
		push pVehicle
		call dwFunc
		mov uiHandle, eax
	}
	return uiHandle;
}

IVVehicle * CPools::GetVehicleFromHandle(unsigned int uiHandle)
{
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPool__AtHandle_7);
	DWORD dwVehiclePool = *(DWORD *)(g_pClient->GetBaseAddress() + VAR_VehiclePool_7);
	IVVehicle * pVehicle = NULL;
	_asm
	{
		mov ecx, dwVehiclePool
		push uiHandle
		call dwFunc
		mov pVehicle, eax
	}
	return pVehicle;
}

IVPlayerInfo * CPools::GetPlayerInfoFromIndex(unsigned int uiIndex)
{
	// Is the index more than the player info array size?
	if(uiIndex > PLAYER_INFO_ARRAY_SIZE)
		return NULL;

	// Get the player info pointer array
	IVPlayerInfo ** pPlayerInfos = (IVPlayerInfo **)(g_pClient->GetBaseAddress() + VAR_PlayerInfoArray);

	// Return the player info pointer
	return pPlayerInfos[uiIndex];
}

IVPlayerInfo * CPools::GetPlayerInfoFromPlayerPed(IVPlayerPed * pPlayerPed)
{
	// Loop through all player info indexes
	for(unsigned int i = 0; i < PLAYER_INFO_ARRAY_SIZE; i++)
	{
		// Get the current player info
		IVPlayerInfo * pPlayerInfo = GetPlayerInfoFromIndex(i);

		// Does the current player info exist?
		if(pPlayerInfo)
		{
			// Is the current player infos player ped the player ped?
			if(pPlayerInfo->m_pPlayerPed == pPlayerPed)
				return pPlayerInfo;
		}
	}

	// Player info not found for player ped
	return NULL;
}

unsigned int CPools::GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Loop through all player info indexes
	for(unsigned int i = 0; i < PLAYER_INFO_ARRAY_SIZE; i++)
	{
		// Is the current index the player info?
		if(GetPlayerInfoFromIndex(i) == pPlayerInfo)
			return i;
	}

	// Player info not found in array
	return INVALID_PLAYER_PED;
}

void CPools::SetPlayerInfoAtIndex(unsigned int uiIndex, IVPlayerInfo * pPlayerInfo)
{
	// Is the index more than the player info array size?
	if(uiIndex > PLAYER_INFO_ARRAY_SIZE)
		return;

	// Get the player info pointer array
	IVPlayerInfo ** pPlayerInfos = (IVPlayerInfo **)(g_pClient->GetBaseAddress() + VAR_PlayerInfoArray);

	// Set the player info pointer
	pPlayerInfos[uiIndex] = pPlayerInfo;
}

unsigned int CPools::FindFreePlayerInfoIndex()
{
	// Loop through all player info indexes
	for(unsigned int i = 0; i < PLAYER_INFO_ARRAY_SIZE; i++)
	{
		// Is the current index free?
		if(!GetPlayerInfoFromIndex(i))
			return i;
	}

	// No free player info indexes found
	return INVALID_PLAYER_PED;
}

unsigned int CPools::GetLocalPlayerIndex()
{
	return *(unsigned int *)(g_pClient->GetBaseAddress() + VAR_LocalPlayerId_7);
}

void CPools::SetLocalPlayerIndex(unsigned int uiIndex)
{
	*(unsigned int *)(g_pClient->GetBaseAddress() + VAR_LocalPlayerId_7) = uiIndex;
}

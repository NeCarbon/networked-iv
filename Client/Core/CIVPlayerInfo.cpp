//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPlayerInfo.cpp
// Project: Client
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CIVPlayerInfo::CIVPlayerInfo()
{
	m_bCreatedByUs = false;
	m_pPlayerInfo = NULL;
}

CIVPlayerInfo::CIVPlayerInfo(BYTE bytePlayerNumber)
{
	const DWORD dwSize = sizeof(IVPlayerInfo);
	DWORD dwFunc = (g_pClient->GetBaseAddress() + 0x4011D0); // alloc
	IVPlayerInfo * pPlayerInfo = NULL;
	_asm
	{
		push dwSize
		call dwFunc
		mov pPlayerInfo, eax
		add esp, 4
	}
#define FUNC_CPlayerInfo__Constructor 0x87AB70
	dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPlayerInfo__Constructor);
	_asm
	{
		mov ecx, pPlayerInfo
		call dwFunc
	}
	/*DWORD dwCount = *(DWORD *)(g_pClient->GetBaseAddress() + 0x11A7108);
	dwCount++;
	DWORD * dwArray = *(DWORD **)(g_pClient->GetBaseAddress() + 0x11A7088);
	dwArray[bytePlayerNumber] = dwCount;
	*(DWORD *)(g_pClient->GetBaseAddress() + 0x11A7108) = dwCount;*/
	pPlayerInfo->m_bytePlayerNumber = bytePlayerNumber;
	//memcpy(result, a2, 0x60u);
	*(DWORD *)(pPlayerInfo + 0x15C) = 0;
	*(DWORD *)(pPlayerInfo + 0x15D) = 0;
	*(DWORD *)(pPlayerInfo + 0x15B) = 0;
	*(DWORD *)(pPlayerInfo + 0x137) = 2;
	m_bCreatedByUs = true;
	m_pPlayerInfo = pPlayerInfo;
}

CIVPlayerInfo::CIVPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	m_bCreatedByUs = false;
	SetPlayerInfo(pPlayerInfo);
}

CIVPlayerInfo::~CIVPlayerInfo()
{
	if(m_bCreatedByUs)
	{
		XLivePBufferFree(m_pPlayerInfo->m_pDisplayScore);
		XLivePBufferFree(m_pPlayerInfo->m_pScore);
		IVPlayerInfo * pPlayerInfo = m_pPlayerInfo;
		DWORD dwFunc = (g_pClient->GetBaseAddress() + 0x5B1C10); // gta_free
		_asm
		{
			push pPlayerInfo
			call dwFunc
			add esp, 4
		}
	}
}

void CIVPlayerInfo::SetPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// If created by us should delete here?
	m_pPlayerInfo = pPlayerInfo;
}

IVPlayerInfo * CIVPlayerInfo::GetPlayerInfo()
{
	return m_pPlayerInfo;
}

void CIVPlayerInfo::SetName(char * szName)
{
	if(m_pPlayerInfo)
		strncpy(m_pPlayerInfo->m_szName, szName, 20);
}

char * CIVPlayerInfo::GetName()
{
	if(m_pPlayerInfo)
		return m_pPlayerInfo->m_szName;

	return 0;
}

void CIVPlayerInfo::SetPlayerNumber(BYTE bytePlayerNumber)
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_bytePlayerNumber = bytePlayerNumber;
}

BYTE CIVPlayerInfo::GetPlayerNumber()
{
	if(m_pPlayerInfo)
		return m_pPlayerInfo->m_bytePlayerNumber;

	return INVALID_PLAYER_PED;
}

void CIVPlayerInfo::SetColour(DWORD dwColour)
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_dwColour = dwColour;
}

DWORD CIVPlayerInfo::GetColour()
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_dwColour;

	return 0;
}

void CIVPlayerInfo::SetPlayerPed(IVPlayerPed * pPlayerPed)
{
	if(m_pPlayerInfo)
		m_pPlayerInfo->m_pPlayerPed = pPlayerPed;
}

IVPlayerPed * CIVPlayerInfo::GetPlayerPed()
{
	if(m_pPlayerInfo)
		return m_pPlayerInfo->m_pPlayerPed;

	return NULL;
}

int CIVPlayerInfo::GetScore()
{
	if(m_pPlayerInfo)
	{
		int iScore;
		XLivePBufferGetDWORD(m_pPlayerInfo->m_pScore, 0, (DWORD *)&iScore);
		return iScore;
	}

	return 0;
}

void CIVPlayerInfo::SetScore(int iScore)
{
	if(m_pPlayerInfo)
		XLivePBufferSetDWORD(m_pPlayerInfo->m_pScore, 0, (DWORD)iScore);
}

int CIVPlayerInfo::GetDisplayScore()
{
	if(m_pPlayerInfo)
	{
		int iScore;
		XLivePBufferGetDWORD(m_pPlayerInfo->m_pDisplayScore, 0, (DWORD *)&iScore);
		return iScore;
	}

	return 0;
}

void CIVPlayerInfo::SetDisplayScore(int iScore)
{
	if(m_pPlayerInfo)
		XLivePBufferSetDWORD(m_pPlayerInfo->m_pDisplayScore, 0, (DWORD)iScore);
}

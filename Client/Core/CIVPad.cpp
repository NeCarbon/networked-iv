//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPad.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CIVPad::CIVPad()
{
	// Flag ourself as created by ourself
	m_bCreatedByUs = true;

	// Allocate the new pad
	m_pPad = new IVPad;

	// Call the CPad constructor
	IVPad * pPad = m_pPad;
	DWORD dwFunc = (g_pClient->GetBaseAddress() + 0x834180);
	_asm
	{
		mov ecx, pPad
		call dwFunc
	}

	// Call CPad::Initialize
	dwFunc = (g_pClient->GetBaseAddress() + 0x8395B0);
	_asm
	{
		push 0
		mov ecx, pPad
		call dwFunc
	}

	// HACK: To fix analog values not being initialized properly
	for(int i = 0; i < INPUT_COUNT; i++)
	{
		if(IsAnalogInput((eInput)i))
		{
			m_pPad->m_padData[i].m_byteCurrentValue = DEFAULT_ANALOG_INPUT_VALUE;
			m_pPad->m_padData[i].m_byteLastValue = DEFAULT_ANALOG_INPUT_VALUE;
			//CLogFile::Printf("Analog input %d values are %d and %d", i, m_pPad->m_padData[i].m_byteCurrentValue, m_pPad->m_padData[i].m_byteLastValue);
		}
	}
}

CIVPad::CIVPad(IVPad * pPad)
{
	m_bCreatedByUs = false;
	m_pPad = pPad;
}

CIVPad::~CIVPad()
{
	// Has the pad been created by us?
	if(m_bCreatedByUs)
	{
		// Call the CPad destructor
		IVPad * pPad = m_pPad;
		DWORD dwFunc = (g_pClient->GetBaseAddress() + 0x7C3680);
		_asm
		{
			mov ecx, pPad
			call dwFunc
		}
	}
}

void CIVPad::SetPad(IVPad * pPad)
{
	m_pPad = pPad;
}

IVPad * CIVPad::GetPad()
{
	return m_pPad;
}

void CIVPad::SetCurrentClientPadState(CClientPadState clientPadState)
{
	if(m_pPad)
	{
		for(int i = 0; i < INPUT_COUNT; i++)
		{
			CPadData * pPadData = &m_pPad->m_padData[i];
			pPadData->m_byteCurrentValue = clientPadState.byteKeys[i];

			// Is it right to do this?
			// i think this is wrong, so disabling it for now
			// check code in CIVPad::SetLastClientPadState
			//if(pPadData->m_pHistory)
			//	pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_byteValue = clientPadState.byteKeys[i];
		}
	}
}

void CIVPad::GetCurrentClientPadState(CClientPadState& clientPadState)
{
	if(m_pPad)
	{
		for(int i = 0; i < INPUT_COUNT; i++)
			clientPadState.byteKeys[i] = m_pPad->m_padData[i].m_byteCurrentValue;
	}
}

void CIVPad::SetLastClientPadState(CClientPadState clientPadState)
{
	if(m_pPad)
	{
		for(int i = 0; i < INPUT_COUNT; i++)
		{
			CPadData * pPadData = &m_pPad->m_padData[i];
			pPadData->m_byteLastValue = clientPadState.byteKeys[i];

			/*if(pPadData->m_pHistory)
			{
				pPadData->m_byteHistoryIndex++;

				if(pPadData->m_byteHistoryIndex >= MAX_HISTORY_ITEMS)
					pPadData->m_byteHistoryIndex = 0;

				pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_byteValue = clientPadState.byteKeys[i];
				pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_dwLastUpdateTime = SharedUtility::GetTime();
			}*/
		}
	}
}

void CIVPad::GetLastClientPadState(CClientPadState& clientPadState)
{
	if(m_pPad)
	{
		for(int i = 0; i < INPUT_COUNT; i++)
			clientPadState.byteKeys[i] = m_pPad->m_padData[i].m_byteLastValue;
	}
}

void CIVPad::SetIsUsingController(bool bIsUsingController)
{
	if(m_pPad)
		m_pPad->m_bIsUsingController = bIsUsingController;
}

bool CIVPad::GetIsUsingController()
{
	if(m_pPad)
		return m_pPad->m_bIsUsingController;

	return false;
}

void CIVPad::SetLastUpdateTime(DWORD dwLastUpdateTime)
{
	if(m_pPad)
		m_pPad->m_dwLastUpdateTime = dwLastUpdateTime;
}

DWORD CIVPad::GetLastUpdateTime()
{
	if(m_pPad)
		return m_pPad->m_dwLastUpdateTime;

	return 0;
}

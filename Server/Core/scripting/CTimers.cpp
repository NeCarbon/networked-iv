//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CTimers.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CTimers::CTimers(CResource* pResource)
{
	m_pResource = pResource;
}

CTimers::~CTimers()
{
}

void CTimers::Process(DWORD dwTickCount)
{
	for(std::list<CTimer*>::iterator iter = m_pTimers.begin(); iter != m_pTimers.end();)
	{
		if(!(*iter)->Process(dwTickCount))
		{
			delete *iter;
			m_pTimers.erase(iter++);
		}
		else
			iter++;
	}
}

CTimer* CTimers::Add(SQObjectPtr pFunction, int iInterval, int iAmountRepeating, CSquirrelArguments* pArguments)
{
	CTimer* pTimer = new CTimer(m_pResource, pFunction, iInterval, iAmountRepeating, pArguments);
	if(pTimer && pTimer->GetID() != INVALID_ENTITY_ID_LONG)
	{
		m_pTimers.push_back(pTimer);
		return pTimer;
	}
	return NULL;
}

bool CTimers::Remove(CTimer* pTimer)
{
	for(std::list<CTimer*>::iterator iter = m_pTimers.begin(); iter != m_pTimers.end(); ++iter)
	{
		if(*iter == pTimer)
		{
			delete pTimer;
			m_pTimers.erase(iter);
			return true;
		}
	}
	return false;
}

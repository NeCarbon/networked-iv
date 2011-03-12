//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CTimers.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CTimers
{
public:
						 CTimers(CResource* pResource);
						~CTimers();

	CTimer*				Add(SQObjectPtr pFunction, int iInterval, int iAmountRepeating, CSquirrelArguments* pArguments);
	bool				Remove(CTimer* pTimer);

	void				Process(DWORD dwTickCount);
private:
	CResource*			m_pResource;
	std::list<CTimer*>	m_pTimers;
};

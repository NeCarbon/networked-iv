//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CTimer.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CTimer : public CEntity
{
public:
			 CTimer(CResource* pResource, SQObjectPtr pFunction, unsigned long ulInterval, unsigned int uiAmountRepeating, CSquirrelArguments* pArguments);
			~CTimer();

	void	Reset();
	bool	Process(unsigned long ulTickCount);

	bool	SetParent(CEntity* pEntity);
private:
	SQObjectPtr			pFunction;
	unsigned long		ulInterval;
	unsigned int		uiAmountRepeating;
	CSquirrelArguments*	pArguments;
	unsigned long		ulNextExecution;
};

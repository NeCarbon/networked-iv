//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CTimer.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CTimer::CTimer(CResource* pResource, SQObjectPtr pFunction, unsigned long ulInterval, unsigned int uiAmountRepeating, CSquirrelArguments* pArguments) : CEntity(ENTITY_TYPE_TIMER, pResource, "timer")
{
	this->pFunction = pFunction;
	this->ulInterval = ulInterval;
	this->uiAmountRepeating = uiAmountRepeating;
	this->pArguments = pArguments;

	Reset();
}

CTimer::~CTimer()
{
	delete pArguments;
}

void CTimer::Reset()
{
	ulNextExecution = GetTickCount() + ulInterval;
}

bool CTimer::Process(unsigned long ulTickCount)
{
	if(ulTickCount >= ulNextExecution)
	{
		CResource* pResource = dynamic_cast< CResource* >(GetParent());
		assert(pResource);

		pResource->GetVM()->Call(NULL, pFunction, pArguments);

		// reset the time left for the next execution
		Reset();

		if(uiAmountRepeating == 0)
			// infinite timer, so never delete
			return true;
		else if(uiAmountRepeating == 1)
			// had only one remaining run (0 now), so delete it now
			return false;
		else
		{
			--uiAmountRepeating;
			return true;
		}

	}
	return true;
}

bool CTimer::SetParent(CEntity* pEntity)
{
	// we won't allow anyone to set our parent element
	return false;
}

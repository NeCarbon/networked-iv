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
	m_pFunction = pFunction;
	m_ulInterval = ulInterval;
	m_uiAmountRepeating = uiAmountRepeating;
	m_pArguments = pArguments;

	Reset();
}

CTimer::~CTimer()
{
	delete m_pArguments;
}

void CTimer::Reset()
{
	m_ulNextExecution = (SharedUtility::GetTime() + m_ulInterval);
}

bool CTimer::Process(unsigned long ulTime)
{
	if(ulTime >= m_ulNextExecution)
	{
		CResource* pResource = dynamic_cast< CResource* >(GetParent());
		assert(pResource);

		pResource->GetVM()->Call(NULL, m_pFunction, m_pArguments);

		// reset the time left for the next execution
		Reset();

		if(m_uiAmountRepeating == 0)
			// infinite timer, so never delete
			return true;
		else if(m_uiAmountRepeating == 1)
			// had only one remaining run (0 now), so delete it now
			return false;
		else
		{
			--m_uiAmountRepeating;
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

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CTimerNatives.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CResourceManager * g_pResourceManager;

void CTimerNatives::LoadFunctions(CSquirrel* pSquirrel)
{
	pSquirrel->RegisterFunction("setTimer", Set, -1, "cii");
	pSquirrel->RegisterFunction("resetTimer", Reset, 1, "p");
	pSquirrel->RegisterFunction("killTimer", Kill, 1, "p");
}

int CTimerNatives::Set(SQVM* pVM)
{
	// we need enough parameters
	if(sq_gettop(pVM)<3)
	{
		sq_pushnull(pVM);
		return 1;
	}

	// get the function to execute upon calling the timer
	SQObject o;
	sq_getstackobj(pVM, 2, &o);
	if(o._type != OT_CLOSURE && o._type != OT_NATIVECLOSURE)
	{
		// not a function
		sq_pushnull(pVM);
		return 1;
	}

	// interval to call function in ms
	int iInterval;
	if(SQ_FAILED(sq_getinteger(pVM, 3, &iInterval)) || iInterval < 50)
	{
		sq_pushnull(pVM);
		return 1;
	}

	// how often it should be repeated (0 = infinite)
	int iAmountRepeating;
	if(SQ_FAILED(sq_getinteger(pVM, 4, &iAmountRepeating)) || iAmountRepeating < 0)
	{
		sq_pushnull(pVM);
		return 1;
	}

	CResource* pResource = g_pResourceManager->Get(pVM);
	if(pResource)
	{
		// collect the parameters
		CSquirrelArguments* pArguments = new CSquirrelArguments();
		for(int i = 5; i <= sq_gettop(pVM); ++ i)
		{
			// push all arguments from the stack
			if(!pArguments->pushFromStack(pVM, i))
			{
				sq_pushnull(pVM);
				delete pArguments;
				return 1;
			}
		}

		CTimer* pTimer = pResource->GetTimers()->Add(o, iInterval, iAmountRepeating, pArguments);
		if(pTimer)
			sq_pushentity(pVM, pTimer);
		else
		{
			delete pArguments; // cleanup is done by the timer otherwise
			sq_pushnull(pVM);
		}
	}
	else
		sq_pushnull(pVM);

	return 1;
}

int CTimerNatives::Reset(SQVM* pVM)
{
	// get the timer passed
	CTimer* pTimer = sq_totimer(pVM, 2);

	// reset it if it's valid
	if(pTimer)
	{
		pTimer->Reset();
		sq_pushbool(pVM, true);
	}
	else
		sq_pushbool(pVM, false);
	return 1;
}

int CTimerNatives::Kill(SQVM* pVM)
{
	// get the timer passed
	CTimer* pTimer = sq_totimer(pVM, 2);

	// get the resource calling (should be the same as the one starting the timer)
	CResource* pResource = g_pResourceManager->Get(pVM);

	// reset it if it's valid
	if(pTimer && pResource)
		sq_pushbool(pVM, pResource->GetTimers()->Remove(pTimer));
	else
		sq_pushbool(pVM, false);
	return 1;
}

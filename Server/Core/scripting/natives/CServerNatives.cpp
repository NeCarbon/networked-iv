//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerNatives.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

void CServerNatives::LoadFunctions(CSquirrel* pSquirrel)
{
	pSquirrel->RegisterFunction("log", Log, 1, "s");
	pSquirrel->RegisterFunction("getTickCount", TickCount, 0);
}

int CServerNatives::Log(SQVM* pVM)
{
	const char* text;
	sq_getstring(pVM, 2, &text);
	CLogFile::Printf(text);
	sq_pushbool(pVM, true);
	return 1;
}

int CServerNatives::TickCount(SQVM* pVM)
{
	sq_pushinteger(pVM, SharedUtility::GetTime());
	return 1;
}

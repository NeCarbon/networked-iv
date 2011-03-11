//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerNatives.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CServerNatives
{
public:
	static void LoadFunctions(CSquirrel* pSquirrel);
	static int  Log(SQVM* pVM);
	static int  TickCount(SQVM* pVM);
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CTimerNatives.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CTimerNatives
{
public:
	static void LoadFunctions(CSquirrel* pSquirrel);
	static int  Set(SQVM* pVM);
	static int  Reset(SQVM* pVM);
	static int  Kill(SQVM* pVM);
};

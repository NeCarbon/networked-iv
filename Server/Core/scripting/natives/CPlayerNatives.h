//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayerNatives.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CPlayerNatives
{
public:
	static void LoadFunctions(CSquirrel* pSquirrel);
	static int  GetName(SQVM* pVM);
	static int  GetIP(SQVM* pVM);
};

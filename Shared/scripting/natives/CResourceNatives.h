//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CResourceNatives.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CResourceNatives
{
public:
	static void LoadFunctions(CSquirrel* pSquirrel);
	static int  GetResource(SQVM* pVM);
	static int  GetFromName(SQVM* pVM);
	static int  GetName(SQVM* pVM);
	static int  Start(SQVM* pVM);
	static int  Restart(SQVM* pVM);
	static int  Stop(SQVM* pVM);
};

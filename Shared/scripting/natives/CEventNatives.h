//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CEntityNatives.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CEventNatives
{
public:
	static void LoadFunctions(CSquirrel* pSquirrel);
	static int  AddHandler(SQVM* pVM);
	static int  RemoveHandler(SQVM* pVM);
	static int  Call(SQVM* pVM);
};

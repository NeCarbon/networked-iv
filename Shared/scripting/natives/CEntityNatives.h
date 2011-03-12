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

class CEntityNatives
{
public:
	static void LoadFunctions(CSquirrel* pSquirrel);
	static int	Create(SQVM* pVM);
	static int  Delete(SQVM* pVM);
	static int  IsValid(SQVM* pVM);
	static int  GetID(SQVM* pVM);
	static int  GetType(SQVM* pVM);
	static int  GetParent(SQVM* pVM);
	static int  SetParent(SQVM* pVM);
	static int  GetRoot(SQVM* pVM);
};

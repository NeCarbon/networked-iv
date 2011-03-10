//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVWorld.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define FUNC_CWorld__Add_7 0x817350
#define FUNC_CWorld__Remove_7 0x8173C0

class CIVWorld
{
public:
	static void Add(IVEntity * pEntity, bool bUnknown = false);
	static void Remove(IVEntity * pEntity, bool bUnknown = false);
};

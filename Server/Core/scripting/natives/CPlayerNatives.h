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
	static int  GetId(SQVM * pVM);
	static int  GetName(SQVM* pVM);
	static int  GetIP(SQVM* pVM);
	static int  IsSpawned(SQVM * pVM);
	static int  GetVehicle(SQVM * pVM);
	static int  GetVehicleSeatId(SQVM * pVM);
};

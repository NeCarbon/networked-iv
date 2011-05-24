//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CVehicleNatives.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CVehicleNatives
{
public:
	static void LoadFunctions(CSquirrel* pSquirrel);
	static int  Create(SQVM * pVM);
	static int  GetId(SQVM * pVM);
	static int  GetPosition(SQVM * pVM);
	static int  SetPosition(SQVM * pVM);
	static int  GetRotation(SQVM * pVM);
	static int  SetRotation(SQVM * pVM);
	static int  Destroy(SQVM * pVM);
};

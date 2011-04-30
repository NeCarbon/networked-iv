//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientInputManager.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

struct InputControl
{
	const char * szName;
	DWORD dwAction;
	DWORD dwDIAction;
	bool bEnabled;
	bool bState;
};

enum eInputGTAControlType
{
	GTA_CONTROL_TYPE_ON_FOOT,
	GTA_CONTROL_TYPE_IN_VEHICLE,
	GTA_CONTROL_TYPE_BOTH
};

struct InputGTAControl
{
	const char * szName;
	const char * szDescription;
	eInput action;
	eInputGTAControlType type;
	bool bEnabled;
	bool bState;
};

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	void Process();
};

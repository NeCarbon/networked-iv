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

// TODO: Control structure and each control type (InputControl, InputGTAControl, InputScriptControl, e.t.c.)
// inherit from it

enum eInputType
{
	INPUT_TYPE_MOUSE,
	INPUT_TYPE_KEYBOARD,
	INPUT_TYPE_KEYBOARD_EXTENDED
};

struct InputControl
{
	const char * szName;
	DWORD dwAction;
	DWORD dwDIAction;
	eInputType type;
	bool bEnabled;
	BYTE byteValue;
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
	BYTE byteValue;
};

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	void PreProcess();
	void Process();
};

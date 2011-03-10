//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerInterface.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CServerInterface
{
public:
	virtual bool OnLoad() = 0;
	virtual void Process() = 0;
	virtual void OnUnload() = 0;
	virtual void SetActive(bool bActive) = 0;
	virtual bool IsActive() = 0;
	virtual void AddToInputQueue(String strString) = 0;
};

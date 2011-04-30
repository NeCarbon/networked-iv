//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CFont.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CFont
{
public:
	virtual void  Draw(const char * szText, DWORD dwColor, float fPosX, float fPosY) = 0;
	virtual float GetHeight() = 0;
	virtual float GetTextExtent(const char * szText) = 0;
	virtual void  OnDeviceLost() = 0;
	virtual void  OnDeviceReset() = 0;
};

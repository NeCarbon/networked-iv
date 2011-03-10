//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPlayerPed.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class IVPlayerPed : public IVPed
{
public:
	// 000-F00
};

class CIVPlayerPed : public CIVPed
{
public:
	CIVPlayerPed();
	CIVPlayerPed(IVPlayerPed * pPlayerPed);
	~CIVPlayerPed();

	void          SetPlayerPed(IVPlayerPed * pPlayerPed);
	IVPlayerPed * GetPlayerPed();
};

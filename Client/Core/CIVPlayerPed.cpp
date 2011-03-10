//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPlayerPed.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CIVPlayerPed::CIVPlayerPed() : CIVPed()
{
	
}

CIVPlayerPed::CIVPlayerPed(IVPlayerPed * pPlayerPed) : CIVPed(pPlayerPed)
{
	
}

CIVPlayerPed::~CIVPlayerPed()
{
	CIVPed::~CIVPed();
}

void CIVPlayerPed::SetPlayerPed(IVPlayerPed * pPlayerPed)
{
	SetPed(pPlayerPed);
}

IVPlayerPed * CIVPlayerPed::GetPlayerPed()
{
	return (IVPlayerPed *)GetPed();
}

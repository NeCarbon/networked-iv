//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPadState.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CClientPadState
{
public:
	BYTE byteKeys[INPUT_COUNT];

	CClientPadState();

	// TODO: FromPad
	// TODO: ToPad
	void FromNetPadState(const CNetworkPadState& netPadState, bool bOnFoot);
	void ToNetPadState(CNetworkPadState& netPadState, bool bOnFoot);
	void Reset();
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayerSocket.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CPlayerSocket
{
public:
	// The player id
	EntityId playerId;

	// The player binary address
	unsigned long ulBinaryAddress;

	// The player port
	unsigned short usPort;

	// The player serial
	String strSerial;

	CPlayerSocket()
	{
		playerId = INVALID_ENTITY_ID;
		ulBinaryAddress = 0xFFFFFFFF;
		usPort = 0xFFFF;
		strSerial.Set("0000000000000000");
	}

	EntityId       GetPlayerId() { return playerId; }
	unsigned long  GetBinaryAdress() { return ulBinaryAddress; }
	String         GetAddress()
	{
		in_addr in;
		memset(&in, 0, sizeof(in));
		in.s_addr = ulBinaryAddress;
		return String(inet_ntoa(in));
	}
	unsigned short GetPort() { return usPort; }
	String         GetSerial() { return strSerial; }
};

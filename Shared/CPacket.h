//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPacket.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// Invalid packet id value
#define INVALID_PACKET_ID 0xFF

// Type used for packet ids
typedef unsigned char PacketId;

class CPacket
{
public:
	// The player socket belonging to the sender
	CPlayerSocket playerSocket;

	// The packet identifier
	PacketId packetId;

	// The length of the packet in bytes
	unsigned int uLength;

	// The data of the packet
	unsigned char * ucData;
};

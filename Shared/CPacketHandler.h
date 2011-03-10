//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPacketHandler.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// Type used for packet ids
typedef unsigned char PacketIdentifier;

typedef void (* PacketFunction_t)(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);

// Structure used for packet functions
struct PacketFunction
{
	PacketIdentifier packetId;
	PacketFunction_t packetFunction;
};

class CPacketHandler
{
private:
	std::list<PacketFunction *> m_packetFunctionList;

public:
	CPacketHandler();
	~CPacketHandler();

	void             AddFunction(PacketIdentifier packetId, PacketFunction_t packetFunction);
	void             RemoveFunction(PacketIdentifier packetId);
	PacketFunction * GetFunctionFromIdentifier(PacketIdentifier packetId);
	bool             HandlePacket(CPacket * pPacket);
};

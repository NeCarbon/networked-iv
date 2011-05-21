//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerPacketHandler.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CServerPacketHandler : public CPacketHandler
{
private:
	static void NewConnection(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void Disconnected(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void LostConnection(CBitStream * pBitStream, CPlayerSocket senderSocket);

public:
	void        Register();
	void        Unregister();
};

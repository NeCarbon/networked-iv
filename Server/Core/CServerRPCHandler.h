//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerRPCHandler.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CServerRPCHandler : public CRPCHandler
{
private:
	static void InitialData(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void ChatInput(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void VehicleEnterExit(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void PlayerSync(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);

public:
	void        Register();
	void        Unregister();
};


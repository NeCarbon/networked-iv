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
	static void InitialData(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void ChatInput(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void PlayerSync(CBitStream * pBitStream, CPlayerSocket senderSocket);

public:
	void        Register();
	void        Unregister();
};


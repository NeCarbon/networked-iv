//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientRPCHandler.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CClientRPCHandler : public CRPCHandler
{
private:
	static void InitialData(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void AddPlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void DeletePlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void SpawnPlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void DestroyPlayer(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void ChatInput(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void PlayerSync(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);

public:
	void        Register();
	void        Unregister();
};

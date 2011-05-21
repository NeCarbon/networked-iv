//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPacketHandler.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CClientPacketHandler : public CPacketHandler
{
private:
	static void ConnectionRejected(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void ConnectionSucceeded(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void ConnectionFailed(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void AlreadyConnected(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void ServerFull(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void Disconnected(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void LostConnection(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void Banned(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void PasswordInvalid(CBitStream * pBitStream, CPlayerSocket senderSocket);

public:
	void        Register();
	void        Unregister();
};

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
	static void ConnectionRejected(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void ConnectionSucceeded(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void ConnectionFailed(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void AlreadyConnected(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void ServerFull(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void Disconnected(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void LostConnection(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void Banned(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);
	static void PasswordInvalid(CBitStreamInterface * pBitStream, CPlayerSocket senderSocket);

public:
	void        Register();
	void        Unregister();
};

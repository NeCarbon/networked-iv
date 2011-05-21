//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerPacketHandler.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

void CServerPacketHandler::NewConnection(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	CLogFile::Printf("New connection from IP %s:%d (Player %d, Serial %s)\n", g_pNetworkManager->GetPlayerIp(senderSocket.playerId).Get(), g_pNetworkManager->GetPlayerPort(senderSocket.playerId), senderSocket.playerId, senderSocket.strSerial.Get());
}

void CServerPacketHandler::Disconnected(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	if(!g_pPlayerManager->IsActive(senderSocket.playerId))
	{
		CLogFile::Printf("Invalid player %d disconnected\n", senderSocket.playerId);
		return;
	}

	CLogFile::Printf("Player %d disconnected\n", senderSocket.playerId);
	g_pPlayerManager->Delete(senderSocket.playerId);
}

void CServerPacketHandler::LostConnection(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	if(!g_pPlayerManager->IsActive(senderSocket.playerId))
	{
		CLogFile::Printf("Invalid player %d lost connection\n", senderSocket.playerId);
		return;
	}

	CLogFile::Printf("Player %d lost connection\n", senderSocket.playerId);
	g_pPlayerManager->Delete(senderSocket.playerId);
}

void CServerPacketHandler::Register()
{
	AddFunction(PACKET_NEW_CONNECTION, NewConnection);
	AddFunction(PACKET_DISCONNECTED, Disconnected);
	AddFunction(PACKET_LOST_CONNECTION, LostConnection);
}

void CServerPacketHandler::Unregister()
{
	RemoveFunction(PACKET_NEW_CONNECTION);
	RemoveFunction(PACKET_DISCONNECTED);
	RemoveFunction(PACKET_LOST_CONNECTION);
}

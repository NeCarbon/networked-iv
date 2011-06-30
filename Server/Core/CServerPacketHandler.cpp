//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerPacketHandler.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CServer * g_pServer;

void CServerPacketHandler::NewConnection(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	CLogFile::Printf("New connection from IP %s:%d (Player %d, Serial %s)", g_pServer->GetNetworkManager()->GetPlayerIp(senderSocket.playerId).Get(), g_pServer->GetNetworkManager()->GetPlayerPort(senderSocket.playerId), senderSocket.playerId, senderSocket.strSerial.Get());
}

void CServerPacketHandler::Disconnected(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	if(!g_pServer->GetPlayerManager()->IsActive(senderSocket.playerId))
	{
		CLogFile::Printf("Invalid player %d disconnected", senderSocket.playerId);
		return;
	}

	CLogFile::Printf("Player %d disconnected", senderSocket.playerId);
	g_pServer->GetPlayerManager()->Delete(senderSocket.playerId);
}

void CServerPacketHandler::LostConnection(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	if(!g_pServer->GetPlayerManager()->IsActive(senderSocket.playerId))
	{
		CLogFile::Printf("Invalid player %d lost connection", senderSocket.playerId);
		return;
	}

	CLogFile::Printf("Player %d lost connection", senderSocket.playerId);
	g_pServer->GetPlayerManager()->Delete(senderSocket.playerId);
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

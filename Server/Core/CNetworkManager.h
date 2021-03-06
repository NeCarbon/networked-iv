//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetworkManager.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CNetworkManager
{
private:
	CNetServerInterface *  m_pNetServer;
	CServerPacketHandler * m_pServerPacketHandler;
	CServerRPCHandler *    m_pServerRPCHandler;

public:
	CNetworkManager();
	~CNetworkManager();

	void           Startup(int iPort, int iMaxPlayers, String strPassword = "", String strHostAddress = "");
	static void    PacketHandler(CPacket * pPacket);
	void           Process();
	void           RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel = PACKET_CHANNEL_DEFAULT);
	String         GetPlayerIp(EntityId playerId);
	unsigned short GetPlayerPort(EntityId playerId);
};
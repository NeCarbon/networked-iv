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
	void           RPC(RPCIdentifier rpcId, CBitStreamInterface * pBitStream, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel = 0);
	String         GetPlayerIp(EntityId playerId);
	unsigned short GetPlayerPort(EntityId playerId);
};
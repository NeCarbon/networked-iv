//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetworkManager.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CNetworkManager
{
private:
	CNetClientInterface *  m_pNetClient;
	CClientPacketHandler * m_pClientPacketHandler;
	CClientRPCHandler *    m_pClientRPCHandler;
	unsigned long          m_ulLastFullSyncTime;

public:
	CNetworkManager();
	~CNetworkManager();

	void        Startup(String strHost, unsigned short usPort, String strPassword);
	static void PacketHandler(CPacket * pPacket);
	void        Process();
	void        ProcessSync(CClientPlayer * pPlayer);
	void        Connect();
	bool        IsConnected();
	void        Disconnect();
	void        RPC(RPCIdentifier rpcId, CBitStreamInterface * pBitStream, ePacketPriority priority, ePacketReliability reliability, char cOrderingChannel = 0);
};

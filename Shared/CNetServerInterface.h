//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetServerInterface.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

typedef void (* PacketHandler_t)(CPacket * pPacket);

class CNetServerInterface
{
public:
	virtual bool            Startup(unsigned short usPort, int iMaxPlayers, String strHostAddress = "") = 0;
	virtual void            Shutdown(int iBlockDuration) = 0;
	virtual void            Process() = 0;
	virtual void            SetPassword(String strPassword) = 0;
	virtual String          GetPassword() = 0;
	virtual unsigned int    Send(CBitStreamInterface * bsData, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel = PACKET_CHANNEL_DEFAULT) = 0;
	virtual unsigned int    RPC(RPCIdentifier rpcId, CBitStreamInterface * bsData, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel = PACKET_CHANNEL_DEFAULT) = 0;
	virtual String          GetPlayerIp(EntityId playerId) = 0;
	virtual unsigned short  GetPlayerPort(EntityId playerId) = 0;
	virtual void            SetPacketHandler(PacketHandler_t pfnPacketHandler) = 0;
	virtual PacketHandler_t GetPacketHandler() = 0;
	virtual String          GetPlayerSerial(EntityId playerId) = 0;
	virtual void            KickPlayer(EntityId playerId, bool bSendDisconnectionNotification = true, ePacketPriority disconnectionPacketPriority = PRIORITY_LOW) = 0;
	virtual CPlayerSocket   GetPlayerSocket(EntityId playerId) = 0;
};

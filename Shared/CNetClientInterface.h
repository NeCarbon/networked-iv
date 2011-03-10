//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetClientInterface.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

enum eConnectionAttemptResult
{
	CONNECTION_ATTEMPT_STARTED,
	INVALID_PARAMETER,
	CANNOT_RESOLVE_DOMAIN_NAME,
	ALREADY_CONNECTED_TO_ENDPOINT,
	CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS,
	SECURITY_INITIALIZATION_FAILED,
	NO_HOST_SET,
};

typedef void (* PacketHandler_t)(CPacket * pPacket);

class CNetClientInterface
{
public:
	virtual bool                     Startup() = 0;
	virtual void                     Shutdown(int iBlockDuration) = 0;
	virtual eConnectionAttemptResult Connect() = 0;
	virtual void                     Disconnect() = 0;
	virtual void                     Process() = 0;
	virtual void                     SetPassword(String strPassword) = 0;
	virtual String                   GetPassword() = 0;
	virtual unsigned int             Send(CBitStreamInterface * bsData, ePacketPriority priority, ePacketReliability reliability, char cOrderingChannel = 0) = 0;
	virtual unsigned int             RPC(RPCIdentifier rpcId, CBitStreamInterface * bsData, ePacketPriority priority, ePacketReliability reliability, char cOrderingChannel = 0) = 0;
	virtual void                     SetHost(String strHost) = 0;
	virtual String                   GetHost() = 0;
	virtual void                     SetPort(unsigned short usPort) = 0;
	virtual unsigned short           GetPort() = 0;
	virtual bool                     IsConnected() = 0;
	virtual void                     SetPacketHandler(PacketHandler_t pfnPacketHandler) = 0;
	virtual PacketHandler_t          GetPacketHandler() = 0;
	virtual String                   GetSerial() = 0;
	virtual CNetStats              * GetNetStats() = 0;
};

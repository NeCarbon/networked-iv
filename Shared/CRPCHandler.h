//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CRPCHandler.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

typedef void (* RPCFunction_t)(CBitStream * pBitStream, CPlayerSocket senderSocket);

// Structure used for rpc functions
struct RPCFunction
{
	RPCIdentifier rpcId;
	RPCFunction_t rpcFunction;
};

class CRPCHandler
{
private:
	std::list<RPCFunction *> m_rpcFunctionList;

public:
	CRPCHandler();
	~CRPCHandler();

	void          AddFunction(RPCIdentifier rpcId, RPCFunction_t rpcFunction);
	void          RemoveFunction(RPCIdentifier rpcId);
	RPCFunction * GetFunctionFromIdentifier(RPCIdentifier rpcId);
	bool          HandlePacket(CPacket * pPacket);
};

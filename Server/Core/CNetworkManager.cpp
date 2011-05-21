//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetworkManager.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CNetworkManager * g_pNetworkManager;

CNetworkManager::CNetworkManager()
{
	// Create the net server instance
	m_pNetServer = CNetworkModule::GetNetServerInterface();

	// Set the net server packet handler function
	m_pNetServer->SetPacketHandler(PacketHandler);

	// Create the packet handler instance
	m_pServerPacketHandler = new CServerPacketHandler();

	// Create the rpc handler instance
	m_pServerRPCHandler = new CServerRPCHandler();
}

CNetworkManager::~CNetworkManager()
{
	// Unregister the rpcs
	m_pServerRPCHandler->Unregister();

	// Delete the rpc handler instance
	SAFE_DELETE(m_pServerRPCHandler);

	// Unregister the packets
	m_pServerPacketHandler->Unregister();

	// Delete the packet handler instance
	SAFE_DELETE(m_pServerPacketHandler);

	// Shutdown the net server instance
	m_pNetServer->Shutdown(500);

	// Delete the net server instance
	CNetworkModule::DestroyNetServerInterface(m_pNetServer);
}

void CNetworkManager::Startup(int iPort, int iMaxPlayers, String strPassword, String strHostAddress)
{
	// Start up the net server
	m_pNetServer->Startup(iPort, iMaxPlayers, strHostAddress.C_String());

	// Do we have a password?
	if(!strPassword.IsEmpty())
	{
		// Set the RakServer password
		m_pNetServer->SetPassword(strPassword.C_String());
	}

	// Register the packets
	m_pServerPacketHandler->Register();

	// Register the rpcs
	m_pServerRPCHandler->Register();
}

void CNetworkManager::PacketHandler(CPacket * pPacket)
{
	// Get the network manager pointer
	CNetworkManager * pNetworkManager = g_pNetworkManager;

	// Pass it to the packet handler, if that doesn't handle it, pass it to the rpc handler
	if(!pNetworkManager->m_pServerPacketHandler->HandlePacket(pPacket) && 
	   !pNetworkManager->m_pServerRPCHandler->HandlePacket(pPacket))
	{
#ifdef NIV_DEBUG
		CLogFile::Printf("Warning: Unhandled packet (Id: %d, Player: %d)\n", pPacket->packetId, pPacket->playerSocket.playerId);
#endif
	}
}

void CNetworkManager::Process()
{
	// Process the net server
	m_pNetServer->Process();
}

void CNetworkManager::RPC(RPCIdentifier rpcId, CBitStream * pBitStream, ePacketPriority priority, ePacketReliability reliability, EntityId playerId, bool bBroadcast, char cOrderingChannel)
{
	m_pNetServer->RPC(rpcId, pBitStream, priority, reliability, playerId, bBroadcast, cOrderingChannel);
}

String CNetworkManager::GetPlayerIp(EntityId playerId)
{
	return m_pNetServer->GetPlayerIp(playerId);
}

unsigned short CNetworkManager::GetPlayerPort(EntityId playerId)
{
	return m_pNetServer->GetPlayerPort(playerId);
}

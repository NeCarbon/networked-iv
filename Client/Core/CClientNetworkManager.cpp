//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientNetworkManager.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CClientNetworkManager::CClientNetworkManager()
{
	// Create the net client instance
	m_pNetClient = CNetModule::GetNetClientInterface();

	// Set the net client packet handler function
	m_pNetClient->SetPacketHandler(PacketHandler);

	// Create the packet handler instance
	m_pClientPacketHandler = new CClientPacketHandler();

	// Create the rpc handler instance
	m_pClientRPCHandler = new CClientRPCHandler();

	m_ulLastFullSyncTime = 0;
}

CClientNetworkManager::~CClientNetworkManager()
{
	// Unregister the rpcs
	m_pClientRPCHandler->Unregister();

	// Delete the rpc handler instance
	SAFE_DELETE(m_pClientRPCHandler);

	// Unregister the packets
	m_pClientPacketHandler->Unregister();

	// Delete the packet handler instance
	SAFE_DELETE(m_pClientPacketHandler);

	// Shutdown the net client instance
	m_pNetClient->Shutdown(0);

	// Destroy the net client instance
	CNetModule::DestroyNetClientInterface(m_pNetClient);
}

void CClientNetworkManager::Startup(String strHost, unsigned short usPort, String strPassword)
{
	// Start up the net client
	m_pNetClient->Startup();

	// Set the net client host
	m_pNetClient->SetHost(strHost);

	// Set the net client port
	m_pNetClient->SetPort(usPort);

	// Do we have a password?
	if(!strPassword.IsEmpty())
	{
		// Set the net client password
		m_pNetClient->SetPassword(strPassword);
	}

	// Register the packets
	m_pClientPacketHandler->Register();

	// Register the rpcs
	m_pClientRPCHandler->Register();
}

void CClientNetworkManager::PacketHandler(CPacket * pPacket)
{
	// Get the network manager pointer
	CClientNetworkManager * pNetworkManager = g_pClient->GetNetworkManager();

	// Pass it to the packet handler, if that doesn't handle it, pass it to the rpc handler
	if(!pNetworkManager->m_pClientPacketHandler->HandlePacket(pPacket) && 
	   !pNetworkManager->m_pClientRPCHandler->HandlePacket(pPacket))
	{
#ifdef NIV_DEBUG
		CLogFile::Printf("Warning: Unhandled packet (Id: %d)\n", pPacket->packetId);
#endif
	}
}

void CClientNetworkManager::Process()
{
	// Process the net client
	m_pNetClient->Process();

	// Get the player manager
	CClientPlayerManager * pPlayerManager = g_pClient->GetPlayerManager();

	// Process the player manager
	pPlayerManager->Process();

	// Process the streamer
	g_pClient->GetStreamer()->Process();

	// Process sync for the local player
	ProcessSync(pPlayerManager->GetLocalPlayer());
}

void CClientNetworkManager::ProcessSync(CClientPlayer * pPlayer)
{
	// Is the player pointer valid?
	if(pPlayer)
	{
		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime();

		// Is a full sync needed (It has been NETWORK_TICK_RATE or more ms since our
		// last full sync)?
		if((ulCurrentTime - m_ulLastFullSyncTime) >= NETWORK_TICK_RATE)
		{
			// Are we in a vehicle?
			if(pPlayer->IsInVehicle())
			{
				
			}
			else
			{
				// Construct the bit stream
				CBitStream bitStream;

				// Write the player net pad state
				CNetworkPadState netPadState;
				pPlayer->GetNetPadState(netPadState);
				bitStream.Write(netPadState);

				// Write the player position
				Vector3 vecPosition;
				pPlayer->GetPosition(vecPosition);
				bitStream.Write(vecPosition);

				// Write the player rotation
				Vector3 vecRotation;
				pPlayer->GetRotation(vecRotation);
				bitStream.Write(vecRotation);

				// Write the player move speed
				Vector3 vecMoveSpeed;
				pPlayer->GetMoveSpeed(vecMoveSpeed);
				bitStream.Write(vecMoveSpeed);

				// Write the player turn speed
				Vector3 vecTurnSpeed;
				pPlayer->GetTurnSpeed(vecTurnSpeed);
				bitStream.Write(vecTurnSpeed);

				// Send the packet
				g_pClient->GetNetworkManager()->RPC(RPC_PLAYER_SYNC, &bitStream, PRIORITY_LOW, RELIABILITY_UNRELIABLE_SEQUENCED);
			}

			// Update the last full sync time
			m_ulLastFullSyncTime = ulCurrentTime;
		}
	}
}

void CClientNetworkManager::Connect()
{
	// Are we not already connected?
	if(!IsConnected())
	{
		// Start the net client connection process
		eConnectionAttemptResult connectionAttemptResult = m_pNetClient->Connect();

		if(connectionAttemptResult != CONNECTION_ATTEMPT_STARTED)
		{
			switch(connectionAttemptResult)
			{
			case INVALID_PARAMETER:
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Invalid Parameter)");
				break;
			case CANNOT_RESOLVE_DOMAIN_NAME:
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Cannot Resolve Domain Name)");
				break;
			case ALREADY_CONNECTED_TO_ENDPOINT:
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Already Connected To Endpoint)");
				break;
			case CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Connection Attempt Already In Progress)");
				break;
			case SECURITY_INITIALIZATION_FAILED:
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Security Initialization Failed)");
				break;
			case NO_HOST_SET:
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (No Host Set)");
				break;
			}
		}
	}
}

bool CClientNetworkManager::IsConnected()
{
	return m_pNetClient->IsConnected();
}

void CClientNetworkManager::Disconnect()
{
	// Are we connected?
	if(IsConnected())
	{
		// Disconnect from the server
		m_pNetClient->Disconnect();
	}
}

void CClientNetworkManager::RPC(RPCIdentifier rpcId, CBitStreamInterface * pBitStream, ePacketPriority priority, ePacketReliability reliability, char cOrderingChannel)
{
	m_pNetClient->RPC(rpcId, pBitStream, priority, reliability, cOrderingChannel);
}

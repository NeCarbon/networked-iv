//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPacketHandler.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

void CClientPacketHandler::ConnectionRejected(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Rejected)");
	g_pClient->GetNetworkManager()->Disconnect();
}

void CClientPacketHandler::ConnectionSucceeded(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection established!");

	// Construct the initial data bit stream
	CBitStream bitStream;

	// Write the name
	bitStream.Write(g_pClient->GetNick());

	// Send the initial data bit stream
	g_pClient->GetNetworkManager()->RPC(RPC_INITIAL_DATA, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED);
}

void CClientPacketHandler::ConnectionFailed(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Timeout)");
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Retrying...");
	g_pClient->GetNetworkManager()->Connect();
}

void CClientPacketHandler::AlreadyConnected(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Already Connected)");
}

void CClientPacketHandler::ServerFull(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Server Full)");
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Retrying...");
	g_pClient->GetNetworkManager()->Connect();
}

void CClientPacketHandler::Disconnected(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Disconnected from the server.");
	g_pClient->GetNetworkManager()->Disconnect();
}

void CClientPacketHandler::LostConnection(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection to server lost!");
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Reconnecting...");
	g_pClient->GetNetworkManager()->Connect();
}

void CClientPacketHandler::Banned(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Banned)");
	g_pClient->GetNetworkManager()->Disconnect();
}

void CClientPacketHandler::PasswordInvalid(CBitStream * pBitStream, CPlayerSocket senderSocket)
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Connection failed! (Invalid Password)");
	g_pClient->GetNetworkManager()->Disconnect();
}

void CClientPacketHandler::Register()
{
	AddFunction(PACKET_CONNECTION_REJECTED, ConnectionRejected);
	AddFunction(PACKET_CONNECTION_SUCCEEDED, ConnectionSucceeded);
	AddFunction(PACKET_CONNECTION_FAILED, ConnectionFailed);
	AddFunction(PACKET_ALREADY_CONNECTED, AlreadyConnected);
	AddFunction(PACKET_SERVER_FULL, ServerFull);
	AddFunction(PACKET_DISCONNECTED, Disconnected);
	AddFunction(PACKET_LOST_CONNECTION, LostConnection);
	AddFunction(PACKET_BANNED, Banned);
	AddFunction(PACKET_PASSWORD_INVALID, PasswordInvalid);
}

void CClientPacketHandler::Unregister()
{
	RemoveFunction(PACKET_CONNECTION_REJECTED);
	RemoveFunction(PACKET_CONNECTION_SUCCEEDED);
	RemoveFunction(PACKET_CONNECTION_FAILED);
	RemoveFunction(PACKET_ALREADY_CONNECTED);
	RemoveFunction(PACKET_SERVER_FULL);
	RemoveFunction(PACKET_DISCONNECTED);
	RemoveFunction(PACKET_LOST_CONNECTION);
	RemoveFunction(PACKET_BANNED);
	RemoveFunction(PACKET_PASSWORD_INVALID);
}

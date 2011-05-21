//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientRPCHandler.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CClientRPCHandler : public CRPCHandler
{
private:
	static void InitialData(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void AddPlayer(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void DeletePlayer(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void SpawnPlayer(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void DestroyPlayer(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void SpawnVehicle(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void DestroyVehicle(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void ChatInput(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket senderSocket);
	static void PlayerSync(CBitStream * pBitStream, CPlayerSocket senderSocket);

public:
	void        Register();
	void        Unregister();
};

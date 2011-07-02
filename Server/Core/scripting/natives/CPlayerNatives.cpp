//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPlayerNatives.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

void CPlayerNatives::LoadFunctions(CSquirrel * pSquirrel)
{
	pSquirrel->RegisterFunction("getPlayerId", GetId, 1, "p");
	pSquirrel->RegisterFunction("getPlayerName", GetName, 1, "p");
	pSquirrel->RegisterFunction("getPlayerIP", GetIP, 1, "p");
	pSquirrel->RegisterFunction("getPlayerSerial", GetSerial, 1, "p");
	pSquirrel->RegisterFunction("isPlayerSpawned", IsSpawned, 1, "p");
	pSquirrel->RegisterFunction("getPlayerVehicle", GetVehicle, 1, "p");
	pSquirrel->RegisterFunction("getPlayerVehicleSeatId", GetVehicleSeatId, 1, "p");
}

int CPlayerNatives::GetId(SQVM * pVM)
{
	CPlayer * pPlayer = sq_toplayer(pVM, 2);

	if(pPlayer)
		sq_pushinteger(pVM, pPlayer->GetPlayerId());
	else
		sq_pushnull(pVM);

	return 1;
}

int CPlayerNatives::GetName(SQVM* pVM)
{
	CPlayer * pPlayer = sq_toplayer(pVM, 2);

	if(pPlayer)
		sq_pushstring(pVM, pPlayer->GetName(), pPlayer->GetName().GetLength());
	else
		sq_pushnull(pVM);

	return 1;
}

int CPlayerNatives::GetIP(SQVM* pVM)
{
	CPlayer * pPlayer = sq_toplayer(pVM, 2);

	if(pPlayer)
		sq_pushstring(pVM, pPlayer->GetIp(), pPlayer->GetIp().GetLength());
	else
		sq_pushnull(pVM);

	return 1;
}

int CPlayerNatives::GetSerial(SQVM* pVM)
{
	CPlayer * pPlayer = sq_toplayer(pVM, 2);

	if(pPlayer)
		sq_pushstring(pVM, pPlayer->GetSerial().C_String(), pPlayer->GetSerial().GetLength());
	else
		sq_pushnull(pVM);

	return 1;
}

int CPlayerNatives::IsSpawned(SQVM * pVM)
{
	CPlayer * pPlayer = sq_toplayer(pVM, 2);

	if(pPlayer)
		sq_pushbool(pVM, pPlayer->IsSpawned());
	else
		sq_pushnull(pVM);

	return 1;
}

int CPlayerNatives::GetVehicle(SQVM * pVM)
{
	CPlayer * pPlayer = sq_toplayer(pVM, 2);

	if(pPlayer)
		sq_pushentity(pVM, pPlayer->GetVehicle());
	else
		sq_pushnull(pVM);

	return 1;
}

int CPlayerNatives::GetVehicleSeatId(SQVM * pVM)
{
	CPlayer * pPlayer = sq_toplayer(pVM, 2);

	if(pPlayer && pPlayer->GetVehicle())
		sq_pushinteger(pVM, pPlayer->GetVehicleSeatId());
	else
		sq_pushnull(pVM);

	return 1;
}

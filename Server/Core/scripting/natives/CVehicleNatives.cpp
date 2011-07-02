//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CVehicleNatives.cpp
// Project: Server
// Author(s): adamix
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CServer * g_pServer;

void CVehicleNatives::LoadFunctions(CSquirrel * pSquirrel)
{
	pSquirrel->RegisterFunction("createVehicle", Create, 7, "iffffff");
	pSquirrel->RegisterFunction("getVehicleId", GetId, 1, "p");
	pSquirrel->RegisterFunction("getVehiclePosition", GetPosition, 1, "p");
	pSquirrel->RegisterFunction("setVehiclePosition", SetPosition, 4, "pfff");
	pSquirrel->RegisterFunction("getVehicleRotation", GetRotation, 1, "p");
	pSquirrel->RegisterFunction("setVehicleRotation", SetRotation, 4, "pfff");
	pSquirrel->RegisterFunction("destroyVehicle", Destroy, 1, "p");
}

int CVehicleNatives::Create(SQVM * pVM)
{
	int iVehicleId;
	CVector3 vecPos, vecRot;
	sq_getinteger(pVM, -7, &iVehicleId);
	sq_getvector3(pVM, -6, &vecPos);
	sq_getvector3(pVM, -3, &vecRot);
	CVehicle * pVehicle = g_pServer->GetVehicleManager()->Add(iVehicleId);
	if(pVehicle)
	{
		pVehicle->SetPosition(vecPos);
		pVehicle->SetRotation(vecPos);
		pVehicle->SpawnForWorld();
		sq_pushentity(pVM, pVehicle);
	}
	sq_pushnull(pVM);
	return 1;
}

int CVehicleNatives::GetId(SQVM * pVM)
{
	CVehicle * pVehicle = sq_tovehicle(pVM, 2);

	if(pVehicle)
		sq_pushinteger(pVM, pVehicle->GetID());
	else
		sq_pushnull(pVM);

	return 1;
}

int CVehicleNatives::GetPosition(SQVM * pVM)
{
	CVehicle * pVehicle = sq_tovehicle(pVM, 2);

	if(pVehicle)
	{
		CVector3 vecPos;
		pVehicle->GetPosition(vecPos);
		sq_pushvector3(pVM, &vecPos);
	}
	else
		sq_pushnull(pVM);

	return 1;
}


int CVehicleNatives::SetPosition(SQVM * pVM)
{
	CVehicle * pVehicle = sq_tovehicle(pVM, 2);

	if(pVehicle)
	{
		CVector3 vecPos;
		sq_getvector3(pVM, 3, &vecPos);
		pVehicle->SetPosition(vecPos);
		sq_pushbool(pVM, true);
	}
	else
		sq_pushnull(pVM);

	return 1;
}

int CVehicleNatives::GetRotation(SQVM * pVM)
{
	CVehicle * pVehicle = sq_tovehicle(pVM, 2);

	if(pVehicle)
	{
		CVector3 vecPos;
		pVehicle->GetRotation(vecPos);
		sq_pushvector3(pVM, &vecPos);
	}
	else
		sq_pushnull(pVM);

	return 1;
}

int CVehicleNatives::SetRotation(SQVM * pVM)
{
	CVehicle * pVehicle = sq_tovehicle(pVM, 2);

	if(pVehicle)
	{
		CVector3 vecRot;
		sq_getvector3(pVM, 3, &vecRot);
		pVehicle->SetRotation(vecRot);
		sq_pushbool(pVM, true);
	}
	else
		sq_pushnull(pVM);

	return 1;
}

int CVehicleNatives::Destroy(SQVM * pVM)
{
	CVehicle * pVehicle = sq_tovehicle(pVM, 2);

	if(pVehicle)
	{
		g_pServer->GetVehicleManager()->Delete((EntityId)pVehicle->GetID());
		sq_pushbool(pVM, true);
	}
	else
		sq_pushnull(pVM);

	return 1;

}
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
	pSquirrel->RegisterFunction("createVehicle", Create, 1, "iffffff");
}

int CVehicleNatives::Create(SQVM * pVM)
{
	int iVehicleId;
	float fX, fY, fZ, rX, rY, rZ;
	sq_getinteger(pVM, -7, &iVehicleId);
	sq_getfloat(pVM, -6, &fX);
	sq_getfloat(pVM, -5, &fY);
	sq_getfloat(pVM, -4, &fZ);
	sq_getfloat(pVM, -3, &rX);
	sq_getfloat(pVM, -2, &rY);
	sq_getfloat(pVM, -1, &rZ);
	CVehicle * pVehicle = g_pServer->GetVehicleManager()->Add(iVehicleId);
	if(pVehicle)
	{
		pVehicle->SetPosition(CVector3(fX, fY, fZ));
		pVehicle->SetRotation(CVector3(rX, rY, rZ));
		pVehicle->SpawnForWorld();
		sq_pushentity(pVM, pVehicle);
	}
	sq_pushnull(pVM);
	return 1;
}

int CVehicleNatives::SetPosition(SQVM * pVM)
{
	CVehicle * pVehicle = (CVehicle *)sq_toentity(pVM, 2);

	if(pVehicle && pVehicle->GetType() == ENTITY_TYPE_VEHICLE)
	{
		CVector3 vecPos;
		sq_getfloat(pVM, 3, &vecPos.fX);
		sq_getfloat(pVM, 4, &vecPos.fY);
		sq_getfloat(pVM, 5, &vecPos.fZ);
		pVehicle->SetPosition(vecPos);
		sq_pushbool(pVM, true);
	}
	else
		sq_pushnull(pVM);

	return 1;

}

int CVehicleNatives::SetRotation(SQVM * pVM)
{
	CVehicle * pVehicle = (CVehicle *)sq_toentity(pVM, 2);

	if(pVehicle && pVehicle->GetType() == ENTITY_TYPE_VEHICLE)
	{
		CVector3 vecPos;
		sq_getfloat(pVM, 3, &vecPos.fX);
		sq_getfloat(pVM, 4, &vecPos.fY);
		sq_getfloat(pVM, 5, &vecPos.fZ);
		pVehicle->SetRotation(vecPos);
		sq_pushbool(pVM, true);
	}
	else
		sq_pushnull(pVM);

	return 1;
}

int CVehicleNatives::Destroy(SQVM * pVM)
{
	CVehicle * pVehicle = (CVehicle *)sq_toentity(pVM, 2);

	if(pVehicle && pVehicle->GetType() == ENTITY_TYPE_VEHICLE)
	{
		g_pServer->GetVehicleManager()->Delete((EntityId)pVehicle->GetID());
		sq_pushbool(pVM, true);
	}
	else
		sq_pushnull(pVM);

	return 1;

}
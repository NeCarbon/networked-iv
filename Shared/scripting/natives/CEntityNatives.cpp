//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CEntityNatives.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CResourceManager * g_pResourceManager;
extern CRootEntity * g_pRootEntity;

void CEntityNatives::LoadFunctions(CSquirrel* pSquirrel)
{
	pSquirrel->RegisterFunction("createEntity", Create, 1, "s");
	pSquirrel->RegisterFunction("deleteEntity", Delete, 1, "p");
	pSquirrel->RegisterFunction("destroyEntity", Delete, 1, "p");
	pSquirrel->RegisterFunction("isValidEntity", IsValid, 1, ".");
	pSquirrel->RegisterFunction("getEntityID", GetID, 1, "p");
	pSquirrel->RegisterFunction("getEntityType", GetType, 1, ".");
	pSquirrel->RegisterFunction("getEntityParent", GetParent, 1, "p");
	pSquirrel->RegisterFunction("setEntityParent", SetParent, 2, "pp");
	pSquirrel->RegisterFunction("getRootEntity", GetRoot, 0);
}

int CEntityNatives::Create(SQVM* pVM)
{
	CResource* pResource = g_pResourceManager->Get(pVM);
	assert( pResource );

	const char* szName;
	sq_getstring(pVM,2,&szName);

	if(CEntity::GetType(szName) == ENTITY_TYPE_CUSTOM)
	{
		CEntity* pEntity = new CEntity(ENTITY_TYPE_CUSTOM, pResource, szName);
		if(pEntity->GetID() != INVALID_ENTITY_ID_LONG)
		{
			sq_pushentity(pVM,pEntity);
		}
		else
		{
			delete pEntity;
			sq_pushnull(pVM);
		}
	}
	else
		sq_pushnull(pVM);

	return 1;
}

int CEntityNatives::Delete(SQVM* pVM)
{
	CEntity* pEntity = sq_toentity(pVM, 2);
	if(pEntity)
	{
		switch( pEntity->GetType() )
		{
			case ENTITY_TYPE_CUSTOM:
				delete pEntity;
				sq_pushbool(pVM,true);
				break;
			default:
				sq_pushbool(pVM,false);
		}
	}
	else
		sq_pushbool(pVM,false);
	return 1;
}

int CEntityNatives::IsValid(SQVM* pVM)
{
	sq_pushbool(pVM,sq_toentity(pVM, 2)?true:false);
	return 1;
}
	
int CEntityNatives::GetID(SQVM* pVM)
{
	CEntity* pEntity = sq_toentity(pVM, 2);
	if(pEntity)
		sq_pushinteger(pVM,pEntity->GetID());
	else
		sq_pushnull(pVM);
	return 1;
}
	
int CEntityNatives::GetType(SQVM* pVM)
{
	CEntity* pEntity = sq_toentity(pVM, 2);
	if(pEntity)
		sq_pushstring(pVM,pEntity->GetTag().Get(),pEntity->GetTag().GetLength());
	else
		sq_pushnull(pVM);
	return 1;
}

int CEntityNatives::GetParent(SQVM* pVM)
{
	CEntity* pEntity = sq_toentity(pVM, 2);
	if(pEntity)
	{
		CEntity* pParent = pEntity->GetParent();
		if(pParent)
			sq_pushentity(pVM,pParent);
		else
			sq_pushnull(pVM);
	}
	else
		sq_pushnull(pVM);
	return 1;
}

int CEntityNatives::SetParent(SQVM* pVM)
{
	CEntity* pEntity = sq_toentity(pVM, 2);
	CEntity* pNewParent = sq_toentity(pVM, 3);
	if(pEntity && pNewParent)
		return pEntity->SetParent(pNewParent);
	else
		sq_pushbool(pVM, false);
	return 1;
}

int CEntityNatives::GetRoot(SQVM* pVM)
{
	sq_pushentity(pVM, g_pRootEntity);
	return 1;
}

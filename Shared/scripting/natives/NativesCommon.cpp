//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: NativesCommon.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CEntity* sq_toentity(SQVM* pVM, int idx)
{
	SQUserPointer ptr;
	if(SQ_SUCCEEDED(sq_getuserpointer(pVM,idx,&ptr)))
	{
		CEntity* pEntity = CEntityIDs::Get( (int)ptr );
		if( pEntity )
			return pEntity;
	}
	return NULL;	
}

CResource* sq_toresource(SQVM* pVM, int idx)
{
	CEntity* pEntity = sq_toentity(pVM, idx);
	if( pEntity && pEntity->GetType() == ENTITY_TYPE_RESOURCE )
		return dynamic_cast< CResource* >( pEntity );
	return NULL;
}

CTimer* sq_totimer(SQVM* pVM, int idx)
{
	CEntity* pEntity = sq_toentity(pVM, idx);
	if( pEntity && pEntity->GetType() == ENTITY_TYPE_TIMER )
		return dynamic_cast< CTimer* >( pEntity );
	return NULL;
}

PLAYER_CLASS* sq_toplayer(SQVM* pVM, int idx)
{
	CEntity* pEntity = sq_toentity(pVM, idx);
	if( pEntity && pEntity->GetType() == ENTITY_TYPE_PLAYER )
		return dynamic_cast< PLAYER_CLASS* >( pEntity );
	return NULL;
}

VEHICLE_CLASS* sq_tovehicle(SQVM* pVM, int idx)
{
	CEntity* pEntity = sq_toentity(pVM, idx);
	if( pEntity && pEntity->GetType() == ENTITY_TYPE_VEHICLE )
		return dynamic_cast< VEHICLE_CLASS* >( pEntity );
	return NULL;
}

void sq_pushentity(SQVM* pVM, CEntity* pEntity)
{
	sq_pushuserpointer(pVM,(SQUserPointer)pEntity->GetID());
}

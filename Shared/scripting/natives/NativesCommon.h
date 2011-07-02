//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: NativesCommon.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#ifdef _SERVER
	#define PLAYER_CLASS CPlayer
	#define VEHICLE_CLASS CVehicle
#else
	#define PLAYER_CLASS CClientPlayer
	#define VEHICLE_CLASS CClientVehicle
#endif

CEntity*       sq_toentity(SQVM* pVM, int idx);
CResource*     sq_toresource(SQVM* pVM, int idx);
CTimer*        sq_totimer(SQVM* pVM, int idx);
PLAYER_CLASS*  sq_toplayer(SQVM* pVM, int idx);
VEHICLE_CLASS* sq_tovehicle(SQVM* pVM, int idx);

inline void sq_getvector3(SQVM * pVM, int idx, CVector3 * vec)
{
	sq_getfloat(pVM, idx, (SQFloat*)&vec->fX);
	sq_getfloat(pVM, idx + 1, (SQFloat*)&vec->fY);
	sq_getfloat(pVM, idx + 2, (SQFloat*)&vec->fZ);
}

inline void sq_pushvector3(SQVM * pVM, CVector3 * vec)
{
	sq_newtable(pVM);
	sq_pushinteger(pVM, 0);
	sq_pushfloat(pVM, vec->fX);
	sq_createslot(pVM, -3);
	sq_pushinteger(pVM, 1);
	sq_pushfloat(pVM, vec->fY);
	sq_createslot(pVM, -3);
	sq_pushinteger(pVM, 2);
	sq_pushfloat(pVM, vec->fZ);
	sq_createslot(pVM, -3);
}

void		sq_pushentity(SQVM* pVM, CEntity* pEntity);

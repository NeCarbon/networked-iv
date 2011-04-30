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

CEntity*	sq_toentity(SQVM* pVM, int idx);
CResource*	sq_toresource(SQVM* pVM, int idx);
CTimer*		sq_totimer(SQVM* pVM, int idx);

void		sq_pushentity(SQVM* pVM, CEntity* pEntity);

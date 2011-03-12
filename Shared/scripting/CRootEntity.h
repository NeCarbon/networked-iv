//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CRootEntity.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CRootEntity : public CEntity
{
public:
			CRootEntity();
			~CRootEntity();

	bool	SetParent(CEntity* pEntity);
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVDynamicEntity.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CIVDynamicEntity::CIVDynamicEntity() : CIVEntity()
{

}

CIVDynamicEntity::CIVDynamicEntity(IVDynamicEntity * pDynamicEntity) : CIVEntity(pDynamicEntity)
{
	
}

CIVDynamicEntity::~CIVDynamicEntity()
{
	CIVEntity::~CIVEntity();
}

void CIVDynamicEntity::SetDynamicEntity(IVDynamicEntity * pDynamicEntity)
{
	SetEntity((IVEntity *)pDynamicEntity);
}

IVDynamicEntity * CIVDynamicEntity::GetDynamicEntity()
{
	return (IVDynamicEntity *)GetEntity();
}

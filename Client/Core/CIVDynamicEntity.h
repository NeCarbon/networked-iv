//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVDynamicEntity.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class IVDynamicEntityVFTable : public IVEntityVFTable
{
public:
	DWORD mCC;
	DWORD mD0;
	DWORD mD4;
	DWORD mD8;
};

class IVDynamicEntity : public IVEntity
{
public:
                                      // 000-074
	PAD(IVDynamicEntity, pad0, 0x4);  // 074-078
	DWORD m_pAnim;                    // 078-07C
	PAD(IVDynamicEntity, pad1, 0x90); // 07C-10C
};

class CIVDynamicEntity : public CIVEntity
{
public:
	CIVDynamicEntity();
	CIVDynamicEntity(IVDynamicEntity * pDynamicEntity);
	~CIVDynamicEntity();

	void              SetDynamicEntity(IVDynamicEntity * pDynamicEntity);
	IVDynamicEntity * GetDynamicEntity();
};

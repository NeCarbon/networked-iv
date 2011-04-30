//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVWeaponInfo.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CIVWeaponInfo::CIVWeaponInfo()
{
	m_pWeaponInfo = NULL;
}

CIVWeaponInfo::CIVWeaponInfo(eWeaponType weaponType, IVWeaponInfo * pWeaponInfo)
{
	m_weaponType = weaponType;
	m_pWeaponInfo = pWeaponInfo;
}

CIVWeaponInfo::~CIVWeaponInfo()
{
	
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVWeapon.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CIVWeapon::CIVWeapon()
{
	m_pWeapon = NULL;
}

CIVWeapon::CIVWeapon(IVWeapon * pWeapon)
{
	m_pWeapon = pWeapon;
}

CIVWeapon::~CIVWeapon()
{

}

void CIVWeapon::SetWeaponType(eWeaponType weaponType)
{
	if(m_pWeapon)
		m_pWeapon->m_weaponType = weaponType;
}

eWeaponType CIVWeapon::GetWeaponType()
{
	if(m_pWeapon)
		return m_pWeapon->m_weaponType;

	return WEAPON_TYPE_UNARMED;
}

void CIVWeapon::SetAmmo(DWORD dwAmmo)
{
	if(m_pWeapon)
	{
		if(dwAmmo > 25000)
			dwAmmo = 25000;

		XLivePBufferSetDWORD(m_pWeapon->m_pAmmo, 0, dwAmmo);
	}
}

DWORD CIVWeapon::GetAmmo()
{
	if(m_pWeapon)
	{
		DWORD dwAmmo;
		XLivePBufferGetDWORD(m_pWeapon->m_pAmmo, 0, &dwAmmo);
		return dwAmmo;
	}

	return 0;
}

CIVWeaponInfo * CIVWeapon::GetWeaponInfo()
{
	if(m_pWeapon)
		return g_pClient->GetGame()->GetWeaponInfo(m_pWeapon->m_weaponType);

	return NULL;
}

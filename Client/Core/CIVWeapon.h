//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVWeapon.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class IVWeapon
{
public:
	eWeaponType m_weaponType;      // 00-04
	FakeProtectedBuffer * m_pAmmo; // 04-08
	BYTE m8;                       // 08-09 // WeaponState?
	PAD(IVWeapon, pad0, 0x3);      // 09-0C
};


class CIVWeapon
{
private:
	IVWeapon * m_pWeapon;

public:
	CIVWeapon();
	CIVWeapon(IVWeapon * pWeapon);
	~CIVWeapon();

	void            SetWeapon(IVWeapon * pWeapon) { m_pWeapon = pWeapon; }
	IVWeapon *      GetWeapon() { return m_pWeapon; }

	void            SetWeaponType(eWeaponType weaponType);
	eWeaponType     GetWeaponType();
	void            SetAmmo(DWORD dwAmmo);
	DWORD           GetAmmo();
	CIVWeaponInfo * GetWeaponInfo();
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPhysical.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define FUNC_CPhysical__SetMoveSpeed_7 0xA47750
#define FUNC_CPhysical__GetMoveSpeed_7 0xA477F0
#define FUNC_CPhysical__SetTurnSpeed_7 0xA47840
#define FUNC_CPhysical__GetTurnSpeed_7 0xA478E0

class IVPhysicalVFTable : public IVDynamicEntityVFTable
{
public:
	DWORD mDC;
	DWORD mE0;
	DWORD mE4;
	DWORD mE8;
	DWORD GetMoveSpeed;
	DWORD mF0;
	DWORD SetHealth;
	DWORD mF8;
	DWORD GetHealth;
	DWORD m100;
	DWORD m104;
	DWORD m108;
	DWORD m10C;
	DWORD m110;
	DWORD m114;
	DWORD m118;
	DWORD m11C;
};

class IVPhysical : public IVDynamicEntity
{
public:
	                                // 000-10C
	// 13C - In Water (BYTE)?
	PAD(IVPhysical, pad0, 0xD8);    // 10C-1E4
	// 0x1BC - IVEntity * m_pAttachedToEntity;
	// 0x140 - BYTE m_byteAttachedToEntity;
	// 0x150 - IVEntity * m_pCollidedEntity;
	// 0x1AA - BYTE m_byteHasDamageEntity;
	// 0x1C0 - Vector3 vecAttachedOffset;
	// 0x1D0 - Quaternion quatAttachedOffset;
	IVEntity * m_pLastDamageEntity; // 1E4-1E8
	PAD(IVPhysical, pad1, 0x4);     // 1E8-1EC
	DWORD m_dwLastDamageWeapon;     // 1EC-1F0 // -1: None, WeaponId: Weapon, 56-57: Special (Possibly vehicle damage?)
	float m_fHealth;                // 1F0-1F4
	PAD(IVPhysical, pad3, 0x1C);    // 1F4-210
};

class CIVPhysical : public CIVDynamicEntity
{
public:
	CIVPhysical();
	CIVPhysical(IVPhysical * pPhysical);
	~CIVPhysical();

	void         SetPhysical(IVPhysical * pPhysical);
	IVPhysical * GetPhysical();

	void         SetMoveSpeed(Vector3 * vecMoveSpeed);
	void         GetMoveSpeed(Vector3 * vecMoveSpeed);
	void         SetTurnSpeed(Vector3 * vecTurnSpeed);
	void         GetTurnSpeed(Vector3 * vecTurnSpeed);
	void         SetLastDamageEntity(IVEntity * pLastDamageEntity);
	IVEntity *   GetLastDamageEntity();
	void         SetHealth(float fHealth);
	float        GetHealth();
};

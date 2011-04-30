//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPed.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define FUNC_CPed__SetDucking 0x8A70C0
#define FUNC_CPed__IsDucking 0x89C780

enum ePedType
{
	PED_TYPE_PLAYER,
	PED_TYPE_CIVMALE,
	PED_TYPE_CIVFEMALE,
	PED_TYPE_COP,
	PED_TYPE_GANG_ALBANIAN,
	PED_TYPE_GANG_BIKER_1,
	PED_TYPE_GANG_BIKER_2,
	PED_TYPE_GANG_ITALIAN,
	PED_TYPE_GANG_RUSSIAN,
	PED_TYPE_GANG_RUSSIAN_2,
	PED_TYPE_GANG_IRISH,
	PED_TYPE_GANG_JAMAICAN,
	PED_TYPE_GANG_AFRICAN_AMERICAN,
	PED_TYPE_GANG_KOREAN,
	PED_TYPE_GANG_CHINESE_JAPANESE,
	PED_TYPE_GANG_PUERTO_RICAN,
	PED_TYPE_DEALER,
	PED_TYPE_MEDIC,
	PED_TYPE_FIREMAN,
	PED_TYPE_CRIMINAL,
	PED_TYPE_BUM,
	PED_TYPE_PROSTITUTE,
	PED_TYPE_SPECIAL//,
	//PED_TYPE_MISSION(1-8),
	//PED_TYPE_NETWORK_PLAYER_(1-32),
	//PED_TYPE_NETWORK_TEAM_(1-8)
};

class IVPlayerInfo;
class IVVehicle;

class IVPedIntelligence
{
public:
	PAD(IVPedIntelligence, pad0, 0x44);
	IVPedTaskManager * m_pPedTaskManager;
	// TODO: Find size
};

class IVPedWeapons
{
public:
	/*PAD(IVPedWeapons, pad0, 0x3C);          // 000-03C
	IVPedWeapon m_weapons[WEAPON_SLOT_MAX]; // 03C-0B4
	PAD(IVPedWeapons, pad1, 0x64);          // 0B4-118
	BYTE m_byteCurrentWeaponSlot;           // 118-119 (See eWeaponSlot)
	PAD(IVPedWeapons, pad2, 0x1);           // 119-11A*/
	PAD(IVPedWeapons, pad0, 0x11A);
};

class IVPedBase
{
public:
	PAD(IVPedBase, pad0, 0x12C); // 000-12C
	ePedType m_type;             // 12C-12D
	// TODO: Find size
};

enum eCharCreator
{
	UNUSED_CHAR,
	RANDOM_CHAR,
	MISSION_CHAR,
	REPLAY_CHAR
};

class IVPed : public IVPhysical
{
public:
	                                        // 000-210
	PAD(IVPed, pad0, 0x8);                  // 210-218
	BYTE m_bytePlayerNumber;                // 218-219
	BYTE m_byteIsPlayerPed;                 // 219-21A
	PAD(IVPed, pad1, 0x2);                  // 210-21C
	IVPedBase * m_pPedBase;                 // 21C-220
	PAD(IVPed, pad2, 0x4);                  // 220-224
	IVPedIntelligence * m_pPedIntelligence;	// 224-228
	IVPlayerInfo * m_pPlayerInfo;           // 228-22C
	DWORD m_pPedData;                       // 22C-230
	PAD(IVPed, pad3, 0x3C);                 // 230-26C
	BYTE m_byteUnknown;                     // 26C-26D - Bits 4: in vehicle
	PAD(IVPed, pad4, 0x43);                 // 26D-2B0
	IVPedWeapons m_weapons;                 // 2B0-3CA
	// 0x338 - BYTE m_byteShootRate;
	// 0x389 - BYTE m_byteAccuracy;
	// 0x3A8 - Target Entity?
	// 0x580 - Voice?
	PAD(IVPed, pad5, 0x6A6);                // 3CA-A70
	// 0x61C - DWORD m_dwVoiceHash;
	// 0x7F1 - BYTE m_byteWeaponObjectVisible;
	// 0x7FA - IVEntity * m_pTargetVehicle;
	BYTE m_byteCreatedBy;                   // A70-A71 - See eCharCreator
	PAD(IVPed, pad6, 0xF);                  // A71-A80
	DWORD m_dwArrestState;                  // A80-A84
	DWORD m_dwDeathState;                   // A84-A88
	PAD(IVPed, pad7, 0x8);                  // A88-A90
	DWORD m_pPedMoveBlendOnFoot;            // A90-A94
	float m_fMaxHealth;                     // A94-A98
	PAD(IVPed, pad8, 0x18);                 // A98-AB0
	float m_fCurrentHeading;                // AB0-AB4
	float m_fDesiredHeading;                // AB4-AB8
	PAD(IVPed, pad9, 0x88);                 // AB8-B40
	IVVehicle * m_pCurrentVehicle;          // B40-B44
	PAD(IVPed, pad10, 0x7C);                // B44-BC0
	void * m_pPedIKManager;                 // BC0-BC4 // +0x210 = pPed
	PAD(IVPed, pad11, 0x17C);               // BC4-D40
	BYTE m_byteRingState;                  // D40-D41
	PAD(IVPed, pad12, 0x17);                // D41-D58
	float m_fHeadingLimit[2];              // D58-D60
	PAD(IVPed, pad13, 0x1A0);               // BC4-F00
};

class CIVPed : public CIVPhysical
{
private:
	CIVPedTaskManager * m_pPedTaskManager; // TODO: Replace with ped intelligence class

public:
	CIVPed();
	CIVPed(IVPed * pPed);
	~CIVPed();

	void                SetPed(IVPed * pPed);
	IVPed *             GetPed();
	CIVPedTaskManager * GetPedTaskManager() { return m_pPedTaskManager; }

	void           SetType(ePedType type);
	ePedType       GetType();
	void           SetPlayerInfo(IVPlayerInfo * pPlayerInfo);
	IVPlayerInfo * GetPlayerInfo();
	bool           IsInVehicle();
	void           SetCurrentHeading(float fCurrentHeading);
	float          GetCurrentHeading();
	void           SetDesiredHeading(float fDesiredHeading);
	float          GetDesiredHeading();
	void           SetCurrentVehicle(IVVehicle * pVehicle);
	IVVehicle *    GetCurrentVehicle();
	void           SetDucking(bool bDucking, int iUnknown = -1);
	bool           IsDucking();
};

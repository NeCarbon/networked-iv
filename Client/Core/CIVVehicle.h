//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVVehicle.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

enum eSeat
{
	SEAT_FRONT_LEFT,
	SEAT_BACK_LEFT,
	SEAT_FRONT_RIGHT,
	SEAT_BACK_RIGHT
};

class IVVehicle : public IVPhysical
{
public:
	                             // 0000-0210
	PAD(IVVehicle, pad0, 0xD88); // 0210-0F98
	// 0x118 - DWORD m_dwUnknownFlags; // 1024 - Invincible
	// 0xF64 - BYTE m_byteUnknownFlags; // 128 - Handbrake On
	// 0xF65 - BYTE m_byteUnknownFlags; // 1 - Lights On, 2 - Freebies
	// 0xF66 - BYTE m_byteUnknownFlags; // 1 - Takes Less Damage
	// 0xF67 - BYTE m_byteUnknownFlags; // 1 - Part Of Convoy, 8 - Is Drowning
	// 0xF68 - BYTE m_byteUnknownFlags; // 64 - Can Be Targetted
	// 0xF70 - BYTE m_byteUnknownFlags; // 4 - Visibily Damaged
	// 0xF6D - BYTE m_byteUnknownFlags; // 64 - Pretend Occupants
	// 0xD60 - BYTE m_byteRadioStation;
	WORD m_wAlarm;               // 0F98-0F9A
	PAD(IVVehicle, pad1, 0x6);   // 0F9A-0FA0
	IVPed * m_pDriver;           // 0FA0-0FA4
	IVPed * m_pPassengers[8];    // 0FA4-0FC4
	PAD(IVVehicle, pad2, 0x20);  // 0FC4-0FE4
	BYTE m_byteColors[4];        // 0FE4-0FE8
	PAD(IVVehicle, pad3, 0xD8);  // 0FA4-10C0
	// 0xFDC - DWORD m_byteBoneCount;
	BYTE m_byteMaxPassengers;    // 10C0-10C1
	// 0x10C8 - float m_fGasPedal;
	// 0x10CC - float m_fBrakePedal;
	// 0x10D8 - float m_fSteering;
	PAD(IVVehicle, pad4, 0x3B);  // 10C1-10FC
	float m_fEngineHealth;       // 10FC-1100
	PAD(IVVehicle, pad5, 0x18);  // 1100-1118
	// 0x1108 - BYTE m_byteCreatedBy;
	float m_fDirtLevel;          // 1118-111C
	PAD(IVVehicle, pad6, 0xC);   // 111C-1128
	float m_fPetrolTankHealth;   // 1128-112C
	// 0x1130 - Damage Manager?
	// 0x11F0 - BYTE m_byteSirenOn[8];
	PAD(IVVehicle, pad7, 0x1F4); // 112C-1320
	DWORD m_dwDoorLockState;     // 1320-1324 // 0 - Unlocked, 1+ - Locked
	// 0x14C4 - BYTE m_byteUnknownFlags; // 8 - Not Damaged Upside Down, 64 - Firing Water Cannon
	// 0x14E8 - float m_fWaterCannonOrientation;
	// 0x14EC - float m_fWaterCannonElevation;
	// 0x1510 - Vector3 m_vecWaterCannonDirection;
	PAD(IVVehicle, pad8, 0xDAC); // 1324-20D0
	// F66 Needs To Be Hotwired (byte)
	// F67 Can Burst Tyres (byte)
	// F6C Hazard Lights/Indicator Lights/Interior Light (byte)
	// F6E Police Focus Will Track Car (byte)
	// F6F Blip Throttle Randomly (byte)
	// F70 Has Strong Axles/Can Be Visually Damaged (Flags?)
	// 10D4 Steer Bias (float)
	// 1108 Is Mission Car (bool)
	// 1120-12DC Damage
	// 1112 Lights On (word i *think*)
	// 133C Horn
};

class CIVVehicle : public CIVPhysical
{
public:
	CIVVehicle();
	CIVVehicle(IVVehicle * pVehicle);
	~CIVVehicle();

	void        SetVehicle(IVVehicle * pVehicle);
	IVVehicle * GetVehicle();

	void        SetAlarmDuration(WORD wDuration);
	WORD        GetAlarmDuration();
	void        SetDriver(IVPed * pPed);
	IVPed *     GetDriver();
	void        SetPassenger(BYTE bytePassengerSeatId, IVPed * pPassenger);
	IVPed *     GetPassenger(BYTE bytePassengerSeatId);
	void        SetColors(unsigned char ucColor1, unsigned char ucColor2, unsigned char ucColor3, unsigned char ucColor4);
	void        GetColors(unsigned char * ucColor1, unsigned char * ucColor2, unsigned char * ucColor3, unsigned char * ucColor4);
	void        SetMaxPassengers(BYTE byteMaxPassengers);
	BYTE        GetMaxPasssengers();
	void        SetSirenState(bool bStatus);
	bool        GetSirenState();
	void        SetEngineHealth(float fEngineHealth);
	float       GetEngineHealth();
	void        SetDirtLevel(float fDirtLevel);
	float       GetDirtLevel();
	void        SetPetrolTankHealth(float fPetrolTankHealth);
	float       GetPetrolTankHealth();
	void        SetDoorLockState(DWORD dwDoorLockState);
	DWORD       GetDoorLockState();
	void        SetEngineStatus(bool bStatus, bool bUnknown);
	bool        GetEngineStatus();
};
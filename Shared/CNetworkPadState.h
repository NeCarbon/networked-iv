//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetworkPadState.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>
#include <CNetBitStreamInterface.h>

class CNetworkPadState
{
public:
	// Analog Left Stick Left/Right
	BYTE byteLeftAnalogLR[2];

	// Analog Left Stick Up/Down
	BYTE byteLeftAnalogUD[2];

	// Analog Trigger Values (TODO: Implement trigger keys as this analog value instead of binary value)
	//BYTE byteTriggers[2];

	// Binary Key Count
	enum { KEY_COUNT = 18 };

	// Binary Keys
	struct 
	{
		// On Foot/In Vehicle
		bool bEnterExitVehicle : 1;

		// On Foot
		bool bSprint : 1;
		bool bJump : 1;
		bool bAttack : 1;
		bool bFreeAim1 : 1;
		bool bFreeAim2 : 1;
		bool bMouseAim : 1;
		bool bCombatPunch1 : 1;
		bool bCombatPunch2 : 1;
		bool bCombatKick : 1;
		bool bCombatBlock : 1;

		// In Vehicle
		bool bAccelerate : 1;
		bool bReverse : 1;
		bool bHandbrake : 1;
		bool bHandbrake2 : 1;
		bool bHorn : 1;
		bool bDriveBy : 1;
		bool bHeliPrimaryFire : 1;
	} keys;

	CNetworkPadState();

	bool operator== (const CNetworkPadState& o) const;
	bool operator!= (const CNetworkPadState& o) const;

	bool IsInCombat() const { return (keys.bCombatPunch1 || keys.bCombatPunch2 || keys.bCombatKick || keys.bCombatBlock); }
	bool IsFiring() const { return (keys.bAttack); }
	bool IsAiming() const { return (keys.bFreeAim1 || keys.bFreeAim2 || keys.bMouseAim); }
	bool IsDoingDriveBy() const { return (keys.bDriveBy); }
	bool IsFiringHelicoptor() const { return (keys.bHeliPrimaryFire); }
	bool IsSprinting() const { return (keys.bSprint); }
	bool IsUsingHorn() const { return (keys.bHorn); }
	bool IsJumping() const { return (keys.bJump); }
	bool IsUsingHandbrake() { return (keys.bHandbrake || keys.bHandbrake2); }
	bool IsUsingEnterExitVehicle() { return (keys.bEnterExitVehicle); }
	void Write(CNetBitStreamInterface * pBitStream) const;
	bool Read(CNetBitStreamInterface * pBitStream);
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPadState.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

// Helpers for CClientPadState::From/ToNetPadState
#define SET_BINARY_KEY(to, from) to = (from ? MAX_INPUT_VALUE : DEFAULT_BINARY_INPUT_VALUE)
#define GET_BINARY_KEY(to, from) to = ((from == MAX_INPUT_VALUE) ? true : false)

CClientPadState::CClientPadState()
{
	// Reset the keys
	Reset();
}

void CClientPadState::FromNetPadState(const CNetworkPadState& netPadState, bool bOnFoot)
{
	// Are we on foot?
	if(bOnFoot)
	{
		// Left Analog L/R
		byteKeys[INPUT_MOVE_LEFT] = netPadState.byteLeftAnalogLR[0];
		byteKeys[INPUT_MOVE_RIGHT] = netPadState.byteLeftAnalogLR[1];

		// Left Analog U/D
		byteKeys[INPUT_MOVE_UP] = netPadState.byteLeftAnalogUD[0];
		byteKeys[INPUT_MOVE_DOWN] = netPadState.byteLeftAnalogUD[1];
	}
	else
	{
		// In Vehicle Turn Left/Right
		byteKeys[INPUT_VEH_MOVE_LEFT] = netPadState.byteLeftAnalogLR[0];
		byteKeys[INPUT_VEH_MOVE_RIGHT] = netPadState.byteLeftAnalogLR[1];

		// In Vehicle Lean Forwards/Backwards
		byteKeys[INPUT_VEH_MOVE_UP] = netPadState.byteLeftAnalogUD[0];
		byteKeys[INPUT_VEH_MOVE_DOWN] = netPadState.byteLeftAnalogUD[1];
	}

	// Enter/Exit Vehicle (Only set if this is the local player)
	//SET_BINARY_KEY(byteKeys[INPUT_ENTER], netPadState.keys.bEnterExitVehicle);

	// On Foot Sprint
	SET_BINARY_KEY(byteKeys[INPUT_SPRINT], netPadState.keys.bSprint);

	// On Foot Jump
	SET_BINARY_KEY(byteKeys[INPUT_JUMP], netPadState.keys.bJump);

	// On Foot Attack
	SET_BINARY_KEY(byteKeys[INPUT_ATTACK], netPadState.keys.bAttack);

	// On Foot Free Aim/Melee Lock On 1
	SET_BINARY_KEY(byteKeys[INPUT_ATTACK2], netPadState.keys.bFreeAim1);

	// On Foot Free Aim/Melee Lock On 2
	SET_BINARY_KEY(byteKeys[INPUT_AIM], netPadState.keys.bFreeAim2);

	// On Foot Mouse Aim
	SET_BINARY_KEY(byteKeys[INPUT_FREE_AIM], netPadState.keys.bMouseAim);

	// On Foot Combat Punch 1
	SET_BINARY_KEY(byteKeys[INPUT_MELEE_ATTACK1], netPadState.keys.bCombatPunch1);

	// On Foot Combat Punch 2
	SET_BINARY_KEY(byteKeys[INPUT_MELEE_ATTACK2], netPadState.keys.bCombatPunch2);

	// On Foot Combat Kick
	SET_BINARY_KEY(byteKeys[INPUT_MELEE_KICK], netPadState.keys.bCombatKick);

	// On Foot Combat Block
	SET_BINARY_KEY(byteKeys[INPUT_MELEE_BLOCK], netPadState.keys.bCombatBlock);

	// In Vehicle Accelerate
	SET_BINARY_KEY(byteKeys[INPUT_VEH_ACCELERATE], netPadState.keys.bAccelerate);

	// In Vehicle Reverse
	SET_BINARY_KEY(byteKeys[INPUT_VEH_BRAKE], netPadState.keys.bReverse);

	// In Vehicle Handbrake 1
	SET_BINARY_KEY(byteKeys[INPUT_VEH_HANDBRAKE], netPadState.keys.bHandbrake);

	// In Vehicle Handbrake 2
	SET_BINARY_KEY(byteKeys[INPUT_VEH_HANDBRAKE_ALT], netPadState.keys.bHandbrake2);

	// In Vehicle Horn
	SET_BINARY_KEY(byteKeys[INPUT_VEH_HORN], netPadState.keys.bHorn);

	// In Vehicle Drive By
	SET_BINARY_KEY(byteKeys[INPUT_VEH_ATTACK], netPadState.keys.bDriveBy);

	// In Vehicle Heli Primary Fire
	SET_BINARY_KEY(byteKeys[INPUT_VEH_ATTACK2], netPadState.keys.bHeliPrimaryFire);
}

void CClientPadState::ToNetPadState(CNetworkPadState& netPadState, bool bOnFoot)
{
	// Are we on foot?
	if(bOnFoot)
	{
		// On Foot Left Analog L/R
		netPadState.byteLeftAnalogLR[0] = byteKeys[INPUT_MOVE_LEFT];
		netPadState.byteLeftAnalogLR[1] = byteKeys[INPUT_MOVE_RIGHT];

		// On Foot Left Analog U/D
		netPadState.byteLeftAnalogUD[0] = byteKeys[INPUT_MOVE_UP];
		netPadState.byteLeftAnalogUD[1] = byteKeys[INPUT_MOVE_DOWN];
	}
	else
	{
		// In Vehicle Turn Left/Right
		netPadState.byteLeftAnalogLR[0] = byteKeys[INPUT_VEH_MOVE_LEFT];
		netPadState.byteLeftAnalogLR[1] = byteKeys[INPUT_VEH_MOVE_RIGHT];

		// In Vehicle Lean Forwards/Backwards
		netPadState.byteLeftAnalogUD[0] = byteKeys[INPUT_VEH_MOVE_UP];
		netPadState.byteLeftAnalogUD[1] = byteKeys[INPUT_VEH_MOVE_DOWN];
	}

	// Enter/Exit Vehicle
	GET_BINARY_KEY(netPadState.keys.bEnterExitVehicle, byteKeys[INPUT_ENTER]);

	// On Foot Sprint
	GET_BINARY_KEY(netPadState.keys.bSprint, byteKeys[INPUT_SPRINT]);

	// On Foot Jump
	GET_BINARY_KEY(netPadState.keys.bJump, byteKeys[INPUT_JUMP]);

	// On Foot Attack
	GET_BINARY_KEY(netPadState.keys.bAttack, byteKeys[INPUT_ATTACK]);

	// On Foot Free Aim/Melee Lock On 1
	GET_BINARY_KEY(netPadState.keys.bFreeAim1, byteKeys[INPUT_ATTACK2]);

	// On Foot Free Aim/Melee Lock On 2
	GET_BINARY_KEY(netPadState.keys.bFreeAim2, byteKeys[INPUT_AIM]);

	// On Foot Mouse Aim
	GET_BINARY_KEY(netPadState.keys.bMouseAim, byteKeys[INPUT_FREE_AIM]);

	// On Foot Combat Punch 1
	GET_BINARY_KEY(netPadState.keys.bCombatPunch1, byteKeys[INPUT_MELEE_ATTACK1]);

	// On Foot Combat Punch 2
	GET_BINARY_KEY(netPadState.keys.bCombatPunch2, byteKeys[INPUT_MELEE_ATTACK2]);

	// On Foot Combat Kick
	GET_BINARY_KEY(netPadState.keys.bCombatKick, byteKeys[INPUT_MELEE_KICK]);

	// On Foot Combat Block
	GET_BINARY_KEY(netPadState.keys.bCombatBlock, byteKeys[INPUT_MELEE_BLOCK]);

	// In Vehicle Accelerate
	GET_BINARY_KEY(netPadState.keys.bAccelerate, byteKeys[INPUT_VEH_ACCELERATE]);

	// In Vehicle Reverse
	GET_BINARY_KEY(netPadState.keys.bReverse, byteKeys[INPUT_VEH_BRAKE]);

	// In Vehicle Handbrake 1
	GET_BINARY_KEY(netPadState.keys.bHandbrake, byteKeys[INPUT_VEH_HANDBRAKE]);

	// In Vehicle Handbrake 2
	GET_BINARY_KEY(netPadState.keys.bHandbrake2, byteKeys[INPUT_VEH_HANDBRAKE_ALT]);

	// In Vehicle Horn
	GET_BINARY_KEY(netPadState.keys.bHorn, byteKeys[INPUT_VEH_HORN]);

	// In Vehicle Drive By
	GET_BINARY_KEY(netPadState.keys.bDriveBy, byteKeys[INPUT_VEH_ATTACK]);

	// In Vehicle Heli Primary Fire
	GET_BINARY_KEY(netPadState.keys.bHeliPrimaryFire, byteKeys[INPUT_VEH_ATTACK2]);
}

void CClientPadState::Reset()
{
	// Loop through all keys
	for(int i = 0; i < INPUT_COUNT; i++)
	{
		// Is the current key an analog key?
		if(IsAnalogInput((eInput)i))
			byteKeys[i] = DEFAULT_ANALOG_INPUT_VALUE;
		else
			byteKeys[i] = DEFAULT_BINARY_INPUT_VALUE;
	}
}

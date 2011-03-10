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
#define SET_KEY(to, from) to = from ? 255 : 0
#define GET_KEY(to, from) to = (from == 255) ? true : false

CClientPadState::CClientPadState()
{
	Reset();
}

void CClientPadState::FromNetPadState(const CNetworkPadState& netPadState, bool bOnFoot)
{
	// Are we on foot?
	if(bOnFoot)
	{
		// Left Analog L/R
		byteCurrentKeys[INPUT_MOVE_LEFT] = netPadState.byteLeftAnalogLR[0];
		byteCurrentKeys[INPUT_MOVE_RIGHT] = netPadState.byteLeftAnalogLR[1];

		// Left Analog U/D
		byteCurrentKeys[INPUT_MOVE_UP] = netPadState.byteLeftAnalogUD[0];
		byteCurrentKeys[INPUT_MOVE_DOWN] = netPadState.byteLeftAnalogUD[1];
	}
	else
	{
		// In Vehicle Turn Left/Right
		byteCurrentKeys[INPUT_VEH_MOVE_LEFT] = netPadState.byteLeftAnalogLR[0];
		byteCurrentKeys[INPUT_VEH_MOVE_RIGHT] = netPadState.byteLeftAnalogLR[1];

		// In Vehicle Lean Forwards/Backwards
		byteCurrentKeys[INPUT_VEH_MOVE_UP] = netPadState.byteLeftAnalogUD[0];
		byteCurrentKeys[INPUT_VEH_MOVE_DOWN] = netPadState.byteLeftAnalogUD[1];
	}

	// Enter/Exit Vehicle (Only set if this is the local player)
	//SET_KEY(byteCurrentKeys[INPUT_ENTER], netPadState.keys.bEnterExitVehicle);

	// On Foot Sprint
	SET_KEY(byteCurrentKeys[INPUT_SPRINT], netPadState.keys.bSprint);

	// On Foot Jump
	SET_KEY(byteCurrentKeys[INPUT_JUMP], netPadState.keys.bJump);

	// On Foot Attack
	SET_KEY(byteCurrentKeys[INPUT_ATTACK], netPadState.keys.bAttack);

	// On Foot Free Aim/Melee Lock On 1
	SET_KEY(byteCurrentKeys[INPUT_ATTACK2], netPadState.keys.bFreeAim1);

	// On Foot Free Aim/Melee Lock On 2
	SET_KEY(byteCurrentKeys[INPUT_AIM], netPadState.keys.bFreeAim2);

	// On Foot Mouse Aim
	SET_KEY(byteCurrentKeys[INPUT_FREE_AIM], netPadState.keys.bMouseAim);

	// On Foot Combat Punch 1
	SET_KEY(byteCurrentKeys[INPUT_MELEE_ATTACK1], netPadState.keys.bCombatPunch1);

	// On Foot Combat Punch 2
	SET_KEY(byteCurrentKeys[INPUT_MELEE_ATTACK2], netPadState.keys.bCombatPunch2);

	// On Foot Combat Kick
	SET_KEY(byteCurrentKeys[INPUT_MELEE_KICK], netPadState.keys.bCombatKick);

	// On Foot Combat Block
	SET_KEY(byteCurrentKeys[INPUT_MELEE_BLOCK], netPadState.keys.bCombatBlock);

	// In Vehicle Accelerate
	SET_KEY(byteCurrentKeys[INPUT_VEH_ACCELERATE], netPadState.keys.bAccelerate);

	// In Vehicle Reverse
	SET_KEY(byteCurrentKeys[INPUT_VEH_BRAKE], netPadState.keys.bReverse);

	// In Vehicle Handbrake 1
	SET_KEY(byteCurrentKeys[INPUT_VEH_HANDBRAKE], netPadState.keys.bHandbrake);

	// In Vehicle Handbrake 2
	SET_KEY(byteCurrentKeys[INPUT_VEH_HANDBRAKE_ALT], netPadState.keys.bHandbrake2);

	// In Vehicle Horn
	SET_KEY(byteCurrentKeys[INPUT_VEH_HORN], netPadState.keys.bHorn);

	// In Vehicle Drive By
	SET_KEY(byteCurrentKeys[INPUT_VEH_ATTACK], netPadState.keys.bDriveBy);

	// In Vehicle Heli Primary Fire
	SET_KEY(byteCurrentKeys[INPUT_VEH_ATTACK2], netPadState.keys.bHeliPrimaryFire);
}

void CClientPadState::ToNetPadState(CNetworkPadState& netPadState, bool bOnFoot)
{
	// Are we on foot?
	if(bOnFoot)
	{
		// On Foot Left Analog L/R
		netPadState.byteLeftAnalogLR[0] = byteCurrentKeys[INPUT_MOVE_LEFT];
		netPadState.byteLeftAnalogLR[1] = byteCurrentKeys[INPUT_MOVE_RIGHT];

		// On Foot Left Analog U/D
		netPadState.byteLeftAnalogUD[0] = byteCurrentKeys[INPUT_MOVE_UP];
		netPadState.byteLeftAnalogUD[1] = byteCurrentKeys[INPUT_MOVE_DOWN];
	}
	else
	{
		// In Vehicle Turn Left/Right
		netPadState.byteLeftAnalogLR[0] = byteCurrentKeys[INPUT_VEH_MOVE_LEFT];
		netPadState.byteLeftAnalogLR[1] = byteCurrentKeys[INPUT_VEH_MOVE_RIGHT];

		// In Vehicle Lean Forwards/Backwards
		netPadState.byteLeftAnalogUD[0] = byteCurrentKeys[INPUT_VEH_MOVE_UP];
		netPadState.byteLeftAnalogUD[1] = byteCurrentKeys[INPUT_VEH_MOVE_DOWN];
	}

	// Enter/Exit Vehicle
	GET_KEY(netPadState.keys.bEnterExitVehicle, byteCurrentKeys[INPUT_ENTER]);

	// On Foot Sprint
	GET_KEY(netPadState.keys.bSprint, byteCurrentKeys[INPUT_SPRINT]);

	// On Foot Jump
	GET_KEY(netPadState.keys.bJump, byteCurrentKeys[INPUT_JUMP]);

	// On Foot Attack
	GET_KEY(netPadState.keys.bAttack, byteCurrentKeys[INPUT_ATTACK]);

	// On Foot Free Aim/Melee Lock On 1
	GET_KEY(netPadState.keys.bFreeAim1, byteCurrentKeys[INPUT_ATTACK2]);

	// On Foot Free Aim/Melee Lock On 2
	GET_KEY(netPadState.keys.bFreeAim2, byteCurrentKeys[INPUT_AIM]);

	// On Foot Mouse Aim
	GET_KEY(netPadState.keys.bMouseAim, byteCurrentKeys[INPUT_FREE_AIM]);

	// On Foot Combat Punch 1
	GET_KEY(netPadState.keys.bCombatPunch1, byteCurrentKeys[INPUT_MELEE_ATTACK1]);

	// On Foot Combat Punch 2
	GET_KEY(netPadState.keys.bCombatPunch2, byteCurrentKeys[INPUT_MELEE_ATTACK2]);

	// On Foot Combat Kick
	GET_KEY(netPadState.keys.bCombatKick, byteCurrentKeys[INPUT_MELEE_KICK]);

	// On Foot Combat Block
	GET_KEY(netPadState.keys.bCombatBlock, byteCurrentKeys[INPUT_MELEE_BLOCK]);

	// In Vehicle Accelerate
	GET_KEY(netPadState.keys.bAccelerate, byteCurrentKeys[INPUT_VEH_ACCELERATE]);

	// In Vehicle Reverse
	GET_KEY(netPadState.keys.bReverse, byteCurrentKeys[INPUT_VEH_BRAKE]);

	// In Vehicle Handbrake 1
	GET_KEY(netPadState.keys.bHandbrake, byteCurrentKeys[INPUT_VEH_HANDBRAKE]);

	// In Vehicle Handbrake 2
	GET_KEY(netPadState.keys.bHandbrake2, byteCurrentKeys[INPUT_VEH_HANDBRAKE_ALT]);

	// In Vehicle Horn
	GET_KEY(netPadState.keys.bHorn, byteCurrentKeys[INPUT_VEH_HORN]);

	// In Vehicle Drive By
	GET_KEY(netPadState.keys.bDriveBy, byteCurrentKeys[INPUT_VEH_ATTACK]);

	// In Vehicle Heli Primary Fire
	GET_KEY(netPadState.keys.bHeliPrimaryFire, byteCurrentKeys[INPUT_VEH_ATTACK2]);
}

void CClientPadState::Reset()
{
	// Set the entire pad state to 0 (default)
	memset(this, 0, sizeof(CClientPadState));

	// Set all analog values to 128 (default)
	byteCurrentKeys[INPUT_MOVE_LEFT] = 128;
	bytePreviousKeys[INPUT_MOVE_LEFT] = 128;
	byteCurrentKeys[INPUT_MOVE_RIGHT] = 128;
	bytePreviousKeys[INPUT_MOVE_RIGHT] = 128;
	byteCurrentKeys[INPUT_MOVE_UP] = 128;
	bytePreviousKeys[INPUT_MOVE_UP] = 128;
	byteCurrentKeys[INPUT_MOVE_DOWN] = 128;
	bytePreviousKeys[INPUT_MOVE_DOWN] = 128;
	byteCurrentKeys[INPUT_LOOK_LEFT] = 128;
	bytePreviousKeys[INPUT_LOOK_LEFT] = 128;
	byteCurrentKeys[INPUT_LOOK_RIGHT] = 128;
	bytePreviousKeys[INPUT_LOOK_RIGHT] = 128;
	byteCurrentKeys[INPUT_LOOK_UP] = 128;
	bytePreviousKeys[INPUT_LOOK_UP] = 128;
	byteCurrentKeys[INPUT_LOOK_DOWN] = 128;
	bytePreviousKeys[INPUT_LOOK_DOWN] = 128;
	byteCurrentKeys[INPUT_SNIPER_ZOOM_IN] = 128;
	bytePreviousKeys[INPUT_SNIPER_ZOOM_IN] = 128;
	byteCurrentKeys[INPUT_SNIPER_ZOOM_OUT] = 128;
	bytePreviousKeys[INPUT_SNIPER_ZOOM_OUT] = 128;
	byteCurrentKeys[INPUT_SNIPER_ZOOM_IN_ALTERNATE] = 128;
	bytePreviousKeys[INPUT_SNIPER_ZOOM_IN_ALTERNATE] = 128;
	byteCurrentKeys[INPUT_SNIPER_ZOOM_OUT_ALTERNATE] = 128;
	bytePreviousKeys[INPUT_SNIPER_ZOOM_OUT_ALTERNATE] = 128;
	byteCurrentKeys[INPUT_VEH_MOVE_LEFT] = 128;
	bytePreviousKeys[INPUT_VEH_MOVE_LEFT] = 128;
	byteCurrentKeys[INPUT_VEH_MOVE_RIGHT] = 128;
	bytePreviousKeys[INPUT_VEH_MOVE_RIGHT] = 128;
	byteCurrentKeys[INPUT_VEH_MOVE_UP] = 128;
	bytePreviousKeys[INPUT_VEH_MOVE_UP] = 128;
	byteCurrentKeys[INPUT_VEH_MOVE_DOWN] = 128;
	bytePreviousKeys[INPUT_VEH_MOVE_DOWN] = 128;
	byteCurrentKeys[INPUT_MOUSE_UD] = 128;
	bytePreviousKeys[INPUT_MOUSE_UD] = 128;
	byteCurrentKeys[INPUT_MOUSE_LR] = 128;
	bytePreviousKeys[INPUT_MOUSE_LR] = 128;
	byteCurrentKeys[INPUT_MOVE_KEY_STUNTJUMP] = 128;
	bytePreviousKeys[INPUT_MOVE_KEY_STUNTJUMP] = 128;
	byteCurrentKeys[INPUT_FRONTEND_AXIS_UD] = 128;
	bytePreviousKeys[INPUT_FRONTEND_AXIS_UD] = 128;
	byteCurrentKeys[INPUT_FRONTEND_AXIS_LR] = 128;
	bytePreviousKeys[INPUT_FRONTEND_AXIS_LR] = 128;
	byteCurrentKeys[INPUT_FE_MOUSE_UD] = 128;
	bytePreviousKeys[INPUT_FE_MOUSE_UD] = 128;
	byteCurrentKeys[INPUT_FE_MOUSE_LR] = 128;
	bytePreviousKeys[INPUT_FE_MOUSE_LR] = 128;
	byteCurrentKeys[INPUT_VEH_MOVE_LEFT_2] = 128;
	bytePreviousKeys[INPUT_VEH_MOVE_LEFT_2] = 128;
	byteCurrentKeys[INPUT_VEH_MOVE_RIGHT_2] = 128;
	bytePreviousKeys[INPUT_VEH_MOVE_RIGHT_2] = 128;
}

void CClientPadState::Invalidate()
{
	for(int i = 0; i < INPUT_MAX; i++)
		bytePreviousKeys[i] = byteCurrentKeys[i];
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientInputManager.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

// TODO: Instead of single name allow single or array (Which would allow for multiple names for controls :))
// TODO: Instead of single gta control action allow single or array (Which would allow for gta controls to be merged with each other (e.g. vehicle_enter and vehicle_exit)

InputControl g_ControlInfo[] = 
{
	{ "mouse1",           VK_LBUTTON,  0x00,          INPUT_TYPE_MOUSE,             true, NULL },
	{ "mouse2",           VK_RBUTTON,  0x00,          INPUT_TYPE_MOUSE,             true, NULL },
	{ "mouse3",           VK_MBUTTON,  0x00,          INPUT_TYPE_MOUSE,             true, NULL },
	{ "mouse_wheel_up",   0x00,        0x00,          INPUT_TYPE_MOUSE,             true, NULL },
	{ "mouse_wheel_down", 0x00,        0x00,          INPUT_TYPE_MOUSE,             true, NULL },
	{ "backspace",        VK_BACK,     DIK_BACKSPACE, INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "tab",              VK_TAB,      DIK_TAB,       INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "enter",            VK_RETURN,   DIK_RETURN,    INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "lshift",           VK_SHIFT,    DIK_LSHIFT,    INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "rshift",           VK_SHIFT,    DIK_RSHIFT,    INPUT_TYPE_KEYBOARD_EXTENDED, true, NULL },
	{ "lcontrol",         VK_CONTROL,  DIK_LCONTROL,  INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "rcontrol",         VK_CONTROL,  DIK_RCONTROL,  INPUT_TYPE_KEYBOARD_EXTENDED, true, NULL },
	{ "lalt",             VK_MENU,     DIK_LALT,      INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "ralt",             VK_MENU,     DIK_RALT,      INPUT_TYPE_KEYBOARD_EXTENDED, true, NULL },
	{ "pause",            VK_PAUSE,    DIK_PAUSE,     INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "capslock",         VK_CAPITAL,  DIK_CAPSLOCK,  INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "escape",           VK_ESCAPE,   DIK_CAPSLOCK,  INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "space",            VK_SPACE,    DIK_SPACE,     INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "page_up",          VK_PRIOR,    DIK_PRIOR,     INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "page_down",        VK_NEXT,     DIK_NEXT,      INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "end",              VK_END,      DIK_END,       INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "home",             VK_HOME,     DIK_HOME,      INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "arrow_left",       VK_LEFT,     DIK_LEFT,      INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "arrow_up",         VK_UP,       DIK_UP,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "arrow_right",      VK_RIGHT,    DIK_RIGHT,     INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "arrow_down",       VK_DOWN,     DIK_DOWN,      INPUT_TYPE_KEYBOARD,          true, NULL },
	// printscreen? (VK_PRINT?/VK_SNAPSHOT?)
	{ "insert",           VK_INSERT,   DIK_INSERT,    INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "delete",           VK_DELETE,   DIK_DELETE,    INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "0",                0x30,        DIK_0,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "1",                0x31,        DIK_1,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "2",                0x32,        DIK_2,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "3",                0x33,        DIK_3,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "4",                0x34,        DIK_4,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "5",                0x35,        DIK_5,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "6",                0x36,        DIK_6,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "7",                0x37,        DIK_7,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "8",                0x38,        DIK_8,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "9",                0x39,        DIK_9,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "a",                0x41,        DIK_A,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "b",                0x42,        DIK_B,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "c",                0x43,        DIK_C,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "d",                0x44,        DIK_D,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "e",                0x45,        DIK_E,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f",                0x46,        DIK_F,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "g",                0x47,        DIK_G,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "h",                0x48,        DIK_H,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "i",                0x49,        DIK_I,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "j",                0x4A,        DIK_J,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "k",                0x4B,        DIK_K,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "l",                0x4C,        DIK_L,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "m",                0x4D,        DIK_M,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "n",                0x4E,        DIK_N,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "o",                0x4F,        DIK_O,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "p",                0x50,        DIK_P,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "q",                0x51,        DIK_Q,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "r",                0x52,        DIK_R,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "s",                0x53,        DIK_S,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "t",                0x54,        DIK_T,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "u",                0x55,        DIK_U,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "v",                0x56,        DIK_V,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "w",                0x57,        DIK_W,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "x",                0x58,        DIK_X,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "y",                0x59,        DIK_Y,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "z",                0x5A,        DIK_Z,         INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_0",            VK_NUMPAD0,  DIK_NUMPAD0,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_1",            VK_NUMPAD1,  DIK_NUMPAD1,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_2",            VK_NUMPAD2,  DIK_NUMPAD2,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_3",            VK_NUMPAD3,  DIK_NUMPAD3,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_4",            VK_NUMPAD4,  DIK_NUMPAD4,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_5",            VK_NUMPAD5,  DIK_NUMPAD5,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_6",            VK_NUMPAD6,  DIK_NUMPAD6,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_7",            VK_NUMPAD7,  DIK_NUMPAD7,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_8",            VK_NUMPAD8,  DIK_NUMPAD8,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_9",            VK_NUMPAD9,  DIK_NUMPAD9,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_mul",          VK_MULTIPLY, DIK_MULTIPLY,  INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_add",          VK_ADD,      DIK_ADD,       INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_sub",          VK_SUBTRACT, DIK_SUBTRACT,  INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_dec",          VK_DECIMAL,  DIK_DECIMAL,   INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "num_div",          VK_DIVIDE,   DIK_DIVIDE,    INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f1",               VK_F1,       DIK_F1,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f2",               VK_F2,       DIK_F2,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f3",               VK_F3,       DIK_F3,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f4",               VK_F4,       DIK_F4,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f5",               VK_F5,       DIK_F5,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f6",               VK_F6,       DIK_F6,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f7",               VK_F7,       DIK_F7,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f8",               VK_F8,       DIK_F8,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f9",               VK_F9,       DIK_F9,        INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f10",              VK_F10,      DIK_F10,       INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f11",              VK_F11,      DIK_F11,       INPUT_TYPE_KEYBOARD,          true, NULL },
	{ "f12",              VK_F12,      DIK_F12,       INPUT_TYPE_KEYBOARD,          true, NULL }
};

InputGTAControl g_GTAControlInfo[] = 
{
	{ "change_camera",       "Change Camera",               INPUT_NEXT_CAMERA,           GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "sprint",              "Sprint",                      INPUT_SPRINT,                GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "jump",                "Jump",                        INPUT_JUMP,                  GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "enter_vehicle",       "Enter Vehicle",               INPUT_JUMP,                  GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "attack",              "Attack",                      INPUT_ATTACK,                GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "lock_on_1",           "Lock On 1",                   INPUT_ATTACK2,               GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	// merge lock_on_1 with aim?
	{ "aim",                 "Lock On 2",                   INPUT_AIM,                   GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "look_behind",         "Look Behind",                 INPUT_LOOK_BEHIND,           GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "next_weapon",         "Next Weapon",                 INPUT_NEXT_WEAPON,           GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "previous_weapon",     "Previous Weapon",             INPUT_PREV_WEAPON,           GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "next_target_left",    "Next Target Left",            INPUT_NEXT_TARGET_LEFT,      GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "next_target_right",   "Next Target Right",           INPUT_NEXT_TARGET_RIGHT,     GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "move_left",           "Move Left",                   INPUT_MOVE_LEFT,             GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "move_right",          "Move Right",                  INPUT_MOVE_RIGHT,            GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "move_up",             "Move Forward",                INPUT_MOVE_UP,               GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "move_down",           "Move Backwards",              INPUT_MOVE_DOWN,             GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "look_left",           "Look Left",                   INPUT_LOOK_LEFT,             GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "look_right",          "Look Right",                  INPUT_LOOK_RIGHT,            GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "look_up",             "Look Up",                     INPUT_LOOK_UP,               GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "look_down",           "Look Down",                   INPUT_LOOK_DOWN,             GTA_CONTROL_TYPE_BOTH,       true, NULL },
	// remove above 4 and just bind to mouse/joy?
	{ "duck",                "Duck",                        INPUT_DUCK,                  GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "phone_take_out",      "Take Phone Out",              INPUT_PHONE_TAKE_OUT,        GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "phone_put_away",      "Put Phone Away",              INPUT_PHONE_PUT_AWAY,        GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "action",              "Action",                      INPUT_PICKUP,                GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "sniper_zoom_in",      "Sniper Zoom In",              INPUT_SNIPER_ZOOM_IN,        GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "sniper_zoom_out",     "Sniper Zoom Out",             INPUT_SNIPER_ZOOM_OUT,       GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	//INPUT_SNIPER_ZOOM_IN_ALTERNATE, // merge with sniper_zoom_in
	//INPUT_SNIPER_ZOOM_OUT_ALTERNATE, // merge with sniper_zoom_out
	{ "take_cover",          "Take Cover",                  INPUT_COVER,                 GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "reload",              "Reload",                      INPUT_RELOAD,                GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "steer_left",          "Steer Left",                  INPUT_VEH_MOVE_LEFT,         GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "steer_right",         "Steer Right",                 INPUT_VEH_MOVE_RIGHT,        GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "lean_forward",        "Lean Forward",                INPUT_VEH_MOVE_UP,           GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "lean_back",           "Lean Back",                   INPUT_VEH_MOVE_DOWN,         GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	//INPUT_VEH_GUN_LEFT, // ? // this for drive by aim i guess?
	//INPUT_VEH_GUN_RIGHT, // ? // this for drive by aim i guess?
	//INPUT_VEH_GUN_UP, // ? // this for drive by aim i guess?
	//INPUT_VEH_GUN_DOWN, // ? // this for drive by aim i guess?
	{ "drive_by",            "Drive By",                    INPUT_VEH_ATTACK,            GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_primary_fire",   "Helicopter Primary Fire",     INPUT_VEH_ATTACK2,           GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "accelerate",          "Accelerate",                  INPUT_VEH_ACCELERATE,        GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "reverse",             "Reverse",                     INPUT_VEH_BRAKE,             GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "headlight",           "Headlight",                   INPUT_VEH_HEADLIGHT,         GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "exit_vehicle",        "Exit Vehicle",                INPUT_VEH_EXIT,              GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	// merge exit_vehicle with vehicle_enter for vehicle_enter_exit (Enter/Exit Vehicle)?
	{ "handbrake_1",         "Handbrake 1",                 INPUT_VEH_HANDBRAKE,         GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "handbrake_2",         "Handbrake 2",                 INPUT_VEH_HANDBRAKE_ALT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "hotwire_1",           "Hotwire 1",                   INPUT_VEH_HOTWIRE_LEFT,      GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "hotwire_2",           "Hotwire 2",                   INPUT_VEH_HOTWIRE_RIGHT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "veh_look_left",       "Vehicle Look Left",           INPUT_VEH_LOOK_LEFT,         GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "veh_look_right",      "Vehicle Look Right",          INPUT_VEH_LOOK_RIGHT,        GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	// make descriptions of above 2 just "Look Left" and "Look Right" (as it will be in 'in vehicle' catagory anyway)?
	{ "veh_look_behind",     "Vehicle Look Behind",         INPUT_VEH_LOOK_BEHIND,       GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	// merge veh_look_behind with look_behind?
	{ "cinematic camera",    "Cinematic Camera",            INPUT_VEH_CIN_CAM,           GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	// does above work on foot too?
	{ "next_radio",          "Next Radio",                  INPUT_VEH_NEXT_RADIO,        GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "previous_radio",      "Previous Radio",              INPUT_VEH_PREV_RADIO,        GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "horn",                "Horn",                        INPUT_VEH_HORN,              GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_throttle_up",    "Helicopter Throttle Up",      INPUT_VEH_FLY_THROTTLE_UP,   GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_throttle_down",  "Helicopter Throttle Down",    INPUT_VEH_FLY_THROTTLE_DOWN, GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_rotate_left",    "Helicopter Rotate Left",      INPUT_VEH_FLY_YAW_LEFT,      GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_rotate_right",   "Helicopter Rotate Right",     INPUT_VEH_FLY_YAW_RIGHT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "combat_punch_1",      "Combat Punch 1",              INPUT_MELEE_ATTACK1,         GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "combat_punch_2",      "Combat Punch 2",              INPUT_MELEE_ATTACK2,         GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	//INPUT_MELEE_ATTACK3, // ?
	{ "combat_kick",         "Combat Kick",                 INPUT_MELEE_KICK,            GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "combat_block",        "Combat Block",                INPUT_MELEE_BLOCK,           GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	// input 64-84 is not covered by N:IV
	//INPUT_MELEE_ATTACK4, // ?
	{ "zoom_radar",          "Zoom Radar",                  INPUT_ZOOM_RADAR,            GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "free_aim",            "Free Aim",                    INPUT_FREE_AIM,              GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	// merge free_aim with aim?
	// input 88-99 is not covered by N:IV
	{ "weapon_unarmed",      "Weapon Unarmed",              INPUT_WEAPON_UNARMED,        GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_melee",        "Weapon Melee",                INPUT_WEAPON_MELEE,          GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_handgun",      "Weapon Handgun",              INPUT_WEAPON_HANDGUN,        GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_shotgun",      "Weapon Shotgun",              INPUT_WEAPON_SHOTGUN,        GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_smg",          "Weapon Sub Machine Gun",      INPUT_WEAPON_SMG,            GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_rifle",        "Weapon Automatic Rifle",      INPUT_WEAPON_RIFLE,          GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_sniper",       "Weapon Sniper Rifle",         INPUT_WEAPON_SNIPER,         GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_heavy",        "Weapon Heavy Weapon",         INPUT_WEAPON_HEAVY,          GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_thrown",       "Weapon Grenade/Molotov",      INPUT_WEAPON_THROWN,         GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	{ "weapon_special",      "Weapon Special",              INPUT_WEAPON_SPECIAL,        GTA_CONTROL_TYPE_ON_FOOT,    true, NULL },
	//INPUT_VEH_KEY_UD, // ?
	//INPUT_VEH_KEY_LR, // ?
	//INPUT_VEH_NEXT_WEAPON, // merge with next_weapon
	//INPUT_VEH_PREV_WEAPON, // merge with previous_weapon
	{ "turn_off_radio",      "Turn Off Radio",              INPUT_TURN_OFF_RADIO,        GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	// input 115-129 is not covered by N:IV
	{ "phone_up",            "Phone Up",                    INPUT_KB_UP,                 GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "phone_down",          "Phone Down",                  INPUT_KB_DOWN,               GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "phone_left",          "Phone Left",                  INPUT_KB_LEFT,               GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "phone_right",         "Phone Right",                 INPUT_KB_RIGHT,              GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "next_track",          "Next Track",                  INPUT_NEXT_TRACK,            GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "prev_track",          "Previous Track",              INPUT_PREV_TRACK,            GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	//INPUT_MOUSE_LMB, // ?
	//INPUT_MOUSE_RMB, // ?
	// input 139-140 is not covered by N:IV
	{ "heli_pitch_forward",  "Helicopter Pitch Forward",    INPUT_VEH_FLY_PITCH_UP,      GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_pitch_back",     "Helicopter Pitch Back",       INPUT_VEH_FLY_PITCH_DOWN,    GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_bank_left",      "Helicopter Bank Left",        INPUT_VEH_FLY_ROLL_LEFT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	{ "heli_bank_right",     "Helicopter Bank Right",       INPUT_VEH_FLY_ROLL_RIGHT,    GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL },
	// input 145-169 is not covered by N:IV
	{ "toggle_definition",   "Toggle Definition",           INPUT_FRONTEND_DEFINITION,   GTA_CONTROL_TYPE_BOTH,       true, NULL },
	// input 171-178 is not covered by N:IV
	//INPUT_SNIPER_ZOOM_IN_KB, // merge with sniper_zoom_in
	//INPUT_SNIPER_ZOOM_OUT_KB, // merge with sniper_zoom_out
	{ "phone_accept",        "Phone Accept",                INPUT_KB_PHONE_ACCEPT,       GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "phone_cancel",        "Phone Cancel",                INPUT_KB_PHONE_CANCEL,       GTA_CONTROL_TYPE_BOTH,       true, NULL },
	{ "heli_secondary_fire", "Helicopter Secondary Fire",   INPUT_VEH_ATTACK3_KB,        GTA_CONTROL_TYPE_IN_VEHICLE, true, NULL }
	// input 184 is not covered by N:IV
	//INPUT_VEH_MOVE_LEFT_2, // ?
	//INPUT_VEH_MOVE_RIGHT_2, // ?
};

CInputManager::CInputManager()
{

}

CInputManager::~CInputManager()
{

}

void CInputManager::PreProcess()
{
	// To disable CPadConfig::Process
	// (CPad CPadData processing which is needed for this stuff to work)
	// you will need to do
	//*(DWORD *)(g_pClient->GetBaseAddress() + 0x63AAA0) = 0x900004C2; // retn 4; nop
	// Get the game pad
	CIVPad * pPad = g_pClient->GetGame()->GetPad();

	// Get the current pad state
	CClientPadState currentPadState;
	pPad->GetCurrentClientPadState(currentPadState);

	// Set the last pad state
	pPad->SetLastClientPadState(currentPadState);
}

void CInputManager::Process()
{
	// Get the game pad
	CIVPad * pPad = g_pClient->GetGame()->GetPad();

	// Do we have a valid game pad?
	if(pPad)
	{
		// Apply gta controls
		//for(int i = 0; i < (sizeof(g_GTAControlInfo) / sizeof(InputGTAControl)); i++)
			//pPad->m_padData[g_GTAControlInfo[i].action].m_byteUnknown6 = (g_GTAControlInfo[i].byteValue);
	}
}

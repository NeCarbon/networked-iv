//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientInputManager.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#if 0
InputControl g_ControlInfo[] = 
{
	{ "escape",   VK_ESCAPE,   DIK_ESCAPE,   true, true },
	{ "1",        0x31,        DIK_1,        true, true },
	{ "2",        0x32,        DIK_2,        true, true },
	{ "3",        0x33,        DIK_3,        true, true },
	{ "4",        0x34,        DIK_4,        true, true },
	{ "5",        0x35,        DIK_5,        true, true },
	{ "6",        0x36,        DIK_6,        true, true },
	{ "7",        0x37,        DIK_7,        true, true },
	{ "8",        0x38,        DIK_8,        true, true },
	{ "9",        0x39,        DIK_9,        true, true },
	{ "0",        0x30,        DIK_0,        true, true },
	{ "subtract", VK_SUBTRACT, DIK_MINUS,    true, true },
	{ "equals",   VK_NUMPAD}
//#define DIK_MINUS           0x0C    /* - on main keyboard */
#define DIK_EQUALS          0x0D
#define DIK_BACK            0x0E    /* backspace */
#define DIK_TAB             0x0F
#define DIK_Q               0x10
#define DIK_W               0x11
#define DIK_E               0x12
#define DIK_R               0x13
#define DIK_T               0x14
#define DIK_Y               0x15
#define DIK_U               0x16
#define DIK_I               0x17
#define DIK_O               0x18
#define DIK_P               0x19
#define DIK_LBRACKET        0x1A
#define DIK_RBRACKET        0x1B
#define DIK_RETURN          0x1C    /* Enter on main keyboard */
#define DIK_LCONTROL        0x1D
#define DIK_A               0x1E
#define DIK_S               0x1F
#define DIK_D               0x20
#define DIK_F               0x21
#define DIK_G               0x22
#define DIK_H               0x23
#define DIK_J               0x24
#define DIK_K               0x25
#define DIK_L               0x26
#define DIK_SEMICOLON       0x27
#define DIK_APOSTROPHE      0x28
#define DIK_GRAVE           0x29    /* accent grave */
#define DIK_LSHIFT          0x2A
#define DIK_BACKSLASH       0x2B
#define DIK_Z               0x2C
#define DIK_X               0x2D
#define DIK_C               0x2E
#define DIK_V               0x2F
#define DIK_B               0x30
#define DIK_N               0x31
#define DIK_M               0x32
#define DIK_COMMA           0x33
#define DIK_PERIOD          0x34    /* . on main keyboard */
#define DIK_SLASH           0x35    /* / on main keyboard */
#define DIK_RSHIFT          0x36
#define DIK_MULTIPLY        0x37    /* * on numeric keypad */
#define DIK_LMENU           0x38    /* left Alt */
#define DIK_SPACE           0x39
#define DIK_CAPITAL         0x3A
#define DIK_F1              0x3B
#define DIK_F2              0x3C
#define DIK_F3              0x3D
#define DIK_F4              0x3E
#define DIK_F5              0x3F
#define DIK_F6              0x40
#define DIK_F7              0x41
#define DIK_F8              0x42
#define DIK_F9              0x43
#define DIK_F10             0x44
#define DIK_NUMLOCK         0x45
#define DIK_SCROLL          0x46    /* Scroll Lock */
#define DIK_NUMPAD7         0x47
#define DIK_NUMPAD8         0x48
#define DIK_NUMPAD9         0x49
#define DIK_SUBTRACT        0x4A    /* - on numeric keypad */
#define DIK_NUMPAD4         0x4B
#define DIK_NUMPAD5         0x4C
#define DIK_NUMPAD6         0x4D
#define DIK_ADD             0x4E    /* + on numeric keypad */
#define DIK_NUMPAD1         0x4F
#define DIK_NUMPAD2         0x50
#define DIK_NUMPAD3         0x51
#define DIK_NUMPAD0         0x52
#define DIK_DECIMAL         0x53    /* . on numeric keypad */
#define DIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define DIK_F11             0x57
#define DIK_F12             0x58
#define DIK_F13             0x64    /*                     (NEC PC98) */
#define DIK_F14             0x65    /*                     (NEC PC98) */
#define DIK_F15             0x66    /*                     (NEC PC98) */
#define DIK_KANA            0x70    /* (Japanese keyboard)            */
#define DIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define DIK_CONVERT         0x79    /* (Japanese keyboard)            */
#define DIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define DIK_YEN             0x7D    /* (Japanese keyboard)            */
#define DIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define DIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define DIK_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define DIK_AT              0x91    /*                     (NEC PC98) */
#define DIK_COLON           0x92    /*                     (NEC PC98) */
#define DIK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define DIK_KANJI           0x94    /* (Japanese keyboard)            */
#define DIK_STOP            0x95    /*                     (NEC PC98) */
#define DIK_AX              0x96    /*                     (Japan AX) */
#define DIK_UNLABELED       0x97    /*                        (J3100) */
#define DIK_NEXTTRACK       0x99    /* Next Track */
#define DIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define DIK_RCONTROL        0x9D
#define DIK_MUTE            0xA0    /* Mute */
#define DIK_CALCULATOR      0xA1    /* Calculator */
#define DIK_PLAYPAUSE       0xA2    /* Play / Pause */
#define DIK_MEDIASTOP       0xA4    /* Media Stop */
#define DIK_VOLUMEDOWN      0xAE    /* Volume - */
#define DIK_VOLUMEUP        0xB0    /* Volume + */
#define DIK_WEBHOME         0xB2    /* Web home */
#define DIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define DIK_DIVIDE          0xB5    /* / on numeric keypad */
#define DIK_SYSRQ           0xB7
#define DIK_RMENU           0xB8    /* right Alt */
#define DIK_PAUSE           0xC5    /* Pause */
#define DIK_HOME            0xC7    /* Home on arrow keypad */
#define DIK_UP              0xC8    /* UpArrow on arrow keypad */
#define DIK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define DIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define DIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define DIK_END             0xCF    /* End on arrow keypad */
#define DIK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define DIK_NEXT            0xD1    /* PgDn on arrow keypad */
#define DIK_INSERT          0xD2    /* Insert on arrow keypad */
#define DIK_DELETE          0xD3    /* Delete on arrow keypad */
#define DIK_LWIN            0xDB    /* Left Windows key */
#define DIK_RWIN            0xDC    /* Right Windows key */
#define DIK_APPS            0xDD    /* AppMenu key */
#define DIK_POWER           0xDE    /* System Power */
#define DIK_SLEEP           0xDF    /* System Sleep */
#define DIK_WAKE            0xE3    /* System Wake */
#define DIK_WEBSEARCH       0xE5    /* Web Search */
#define DIK_WEBFAVORITES    0xE6    /* Web Favorites */
#define DIK_WEBREFRESH      0xE7    /* Web Refresh */
#define DIK_WEBSTOP         0xE8    /* Web Stop */
#define DIK_WEBFORWARD      0xE9    /* Web Forward */
#define DIK_WEBBACK         0xEA    /* Web Back */
#define DIK_MYCOMPUTER      0xEB    /* My Computer */
#define DIK_MAIL            0xEC    /* Mail */
#define DIK_MEDIASELECT     0xED    /* Media Select */

/*
 *  Alternate names for keys, to facilitate transition from DOS.
 */
#define DIK_BACKSPACE       DIK_BACK            /* backspace */
#define DIK_NUMPADSTAR      DIK_MULTIPLY        /* * on numeric keypad */
#define DIK_LALT            DIK_LMENU           /* left Alt */
#define DIK_CAPSLOCK        DIK_CAPITAL         /* CapsLock */
#define DIK_NUMPADMINUS     DIK_SUBTRACT        /* - on numeric keypad */
#define DIK_NUMPADPLUS      DIK_ADD             /* + on numeric keypad */
#define DIK_NUMPADPERIOD    DIK_DECIMAL         /* . on numeric keypad */
#define DIK_NUMPADSLASH     DIK_DIVIDE          /* / on numeric keypad */
#define DIK_RALT            DIK_RMENU           /* right Alt */
#define DIK_UPARROW         DIK_UP              /* UpArrow on arrow keypad */
#define DIK_PGUP            DIK_PRIOR           /* PgUp on arrow keypad */
#define DIK_LEFTARROW       DIK_LEFT            /* LeftArrow on arrow keypad */
#define DIK_RIGHTARROW      DIK_RIGHT           /* RightArrow on arrow keypad */
#define DIK_DOWNARROW       DIK_DOWN            /* DownArrow on arrow keypad */
#define DIK_PGDN            DIK_NEXT            /* PgDn on arrow keypad */
};
#endif

InputGTAControl g_GTAControlInfo[] = 
{
	{ "change_camera",       "Change Camera",               INPUT_NEXT_CAMERA,           GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "sprint",              "Sprint",                      INPUT_SPRINT,                GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "jump",                "Jump",                        INPUT_JUMP,                  GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "enter_vehicle",       "Enter Vehicle",               INPUT_JUMP,                  GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "attack",              "Attack",                      INPUT_ATTACK,                GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "lock_on_1",           "Lock On 1",                   INPUT_ATTACK2,               GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	// merge lock_on_1 with aim?
	{ "aim",                 "Lock On 2",                   INPUT_AIM,                   GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "look_behind",         "Look Behind",                 INPUT_LOOK_BEHIND,           GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "next_weapon",         "Next Weapon",                 INPUT_NEXT_WEAPON,           GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "previous_weapon",     "Previous Weapon",             INPUT_PREV_WEAPON,           GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "next_target_left",    "Next Target Left",            INPUT_NEXT_TARGET_LEFT,      GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "next_target_right",   "Next Target Right",           INPUT_NEXT_TARGET_RIGHT,     GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "move_left",           "Move Left",                   INPUT_MOVE_LEFT,             GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "move_right",          "Move Right",                  INPUT_MOVE_RIGHT,            GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "move_up",             "Move Forward",                INPUT_MOVE_UP,               GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "move_down",           "Move Backwards",              INPUT_MOVE_DOWN,             GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "look_left",           "Look Left",                   INPUT_LOOK_LEFT,             GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "look_right",          "Look Right",                  INPUT_LOOK_RIGHT,            GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "look_up",             "Look Up",                     INPUT_LOOK_UP,               GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "look_down",           "Look Down",                   INPUT_LOOK_DOWN,             GTA_CONTROL_TYPE_BOTH,       true, false },
	// remove above 4 and just bind to mouse/joy?
	{ "duck",                "Duck",                        INPUT_DUCK,                  GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "phone_take_out",      "Take Phone Out",              INPUT_PHONE_TAKE_OUT,        GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "phone_put_away",      "Put Phone Away",              INPUT_PHONE_PUT_AWAY,        GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "action",              "Action",                      INPUT_PICKUP,                GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "sniper_zoom_in",      "Sniper Zoom In",              INPUT_SNIPER_ZOOM_IN,        GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "sniper_zoom_out",     "Sniper Zoom Out",             INPUT_SNIPER_ZOOM_OUT,       GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	//INPUT_SNIPER_ZOOM_IN_ALTERNATE, // merge with sniper_zoom_in
	//INPUT_SNIPER_ZOOM_OUT_ALTERNATE, // merge with sniper_zoom_out
	{ "take_cover",          "Take Cover",                  INPUT_COVER,                 GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "reload",              "Reload",                      INPUT_RELOAD,                GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "steer_left",          "Steer Left",                  INPUT_VEH_MOVE_LEFT,         GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "steer_right",         "Steer Right",                 INPUT_VEH_MOVE_RIGHT,        GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "lean_forward",        "Lean Forward",                INPUT_VEH_MOVE_UP,           GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "lean_back",           "Lean Back",                   INPUT_VEH_MOVE_DOWN,         GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	//INPUT_VEH_GUN_LEFT, // ?
	//INPUT_VEH_GUN_RIGHT, // ?
	//INPUT_VEH_GUN_UP, // ?
	//INPUT_VEH_GUN_DOWN, // ?
	{ "drive_by",            "Drive By",                    INPUT_VEH_ATTACK,            GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_primary_fire",   "Helicopter Primary Fire",     INPUT_VEH_ATTACK2,           GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "accelerate",          "Accelerate",                  INPUT_VEH_ACCELERATE,        GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "reverse",             "Reverse",                     INPUT_VEH_BRAKE,             GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "headlight",           "Headlight",                   INPUT_VEH_HEADLIGHT,         GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "exit_vehicle",        "Exit Vehicle",                INPUT_VEH_EXIT,              GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	// merge exit_vehicle with vehicle_enter for vehicle_enter_exit (Enter/Exit Vehicle)?
	{ "handbrake_1",         "Handbrake 1",                 INPUT_VEH_HANDBRAKE,         GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "handbrake_2",         "Handbrake 2",                 INPUT_VEH_HANDBRAKE_ALT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "hotwire_1",           "Hotwire 1",                   INPUT_VEH_HOTWIRE_LEFT,      GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "hotwire_2",           "Hotwire 2",                   INPUT_VEH_HOTWIRE_RIGHT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "veh_look_left",       "Vehicle Look Left",           INPUT_VEH_LOOK_LEFT,         GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "veh_look_right",      "Vehicle Look Right",          INPUT_VEH_LOOK_RIGHT,        GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	// make descriptions of above 2 just "Look Left" and "Look Right" (as it will be in 'in vehicle' catagory anyway)?
	{ "veh_look_behind",     "Vehicle Look Behind",         INPUT_VEH_LOOK_BEHIND,       GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	// merge veh_look_behind with look_behind?
	{ "cinematic camera",    "Cinematic Camera",            INPUT_VEH_CIN_CAM,           GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	// does above work on foot too?
	{ "next_radio",          "Next Radio",                  INPUT_VEH_NEXT_RADIO,        GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "previous_radio",      "Previous Radio",              INPUT_VEH_PREV_RADIO,        GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "horn",                "Horn",                        INPUT_VEH_HORN,              GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_throttle_up",    "Helicopter Throttle Up",      INPUT_VEH_FLY_THROTTLE_UP,   GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_throttle_down",  "Helicopter Throttle Down",    INPUT_VEH_FLY_THROTTLE_DOWN, GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_rotate_left",    "Helicopter Rotate Left",      INPUT_VEH_FLY_YAW_LEFT,      GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_rotate_right",   "Helicopter Rotate Right",     INPUT_VEH_FLY_YAW_RIGHT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "combat_punch_1",      "Combat Punch 1",              INPUT_MELEE_ATTACK1,         GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "combat_punch_2",      "Combat Punch 2",              INPUT_MELEE_ATTACK2,         GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	//INPUT_MELEE_ATTACK3, // ?
	{ "combat_kick",         "Combat Kick",                 INPUT_MELEE_KICK,            GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "combat_block",        "Combat Block",                INPUT_MELEE_BLOCK,           GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	// input 64-84 is not covered by N:IV
	//INPUT_MELEE_ATTACK4, // ?
	{ "zoom_radar",          "Zoom Radar",                  INPUT_ZOOM_RADAR,            GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "free_aim",            "Free Aim",                    INPUT_FREE_AIM,              GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	// merge free_aim with aim?
	// input 88-99 is not covered by N:IV
	{ "weapon_unarmed",      "Weapon Unarmed",              INPUT_WEAPON_UNARMED,        GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_melee",        "Weapon Melee",                INPUT_WEAPON_MELEE,          GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_handgun",      "Weapon Handgun",              INPUT_WEAPON_HANDGUN,        GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_shotgun",      "Weapon Shotgun",              INPUT_WEAPON_SHOTGUN,        GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_smg",          "Weapon Sub Machine Gun",      INPUT_WEAPON_SMG,            GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_rifle",        "Weapon Automatic Rifle",      INPUT_WEAPON_RIFLE,          GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_sniper",       "Weapon Sniper Rifle",         INPUT_WEAPON_SNIPER,         GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_heavy",        "Weapon Heavy Weapon",         INPUT_WEAPON_HEAVY,          GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_thrown",       "Weapon Grenade/Molotov",      INPUT_WEAPON_THROWN,         GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	{ "weapon_special",      "Weapon Special",              INPUT_WEAPON_SPECIAL,        GTA_CONTROL_TYPE_ON_FOOT,    true, false },
	//INPUT_VEH_KEY_UD, // ?
	//INPUT_VEH_KEY_LR, // ?
	//INPUT_VEH_NEXT_WEAPON, // merge with next_weapon
	//INPUT_VEH_PREV_WEAPON, // merge with previous_weapon
	{ "turn_off_radio",      "Turn Off Radio",              INPUT_TURN_OFF_RADIO,        GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	// input 115-129 is not covered by N:IV
	{ "phone_up",            "Phone Up",                    INPUT_KB_UP,                 GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "phone_down",          "Phone Down",                  INPUT_KB_DOWN,               GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "phone_left",          "Phone Left",                  INPUT_KB_LEFT,               GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "phone_right",         "Phone Right",                 INPUT_KB_RIGHT,              GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "next_track",          "Next Track",                  INPUT_NEXT_TRACK,            GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "prev_track",          "Previous Track",              INPUT_PREV_TRACK,            GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	//INPUT_MOUSE_LMB, // ?
	//INPUT_MOUSE_RMB, // ?
	// input 139-140 is not covered by N:IV
	{ "heli_pitch_forward",  "Helicopter Pitch Forward",    INPUT_VEH_FLY_PITCH_UP,      GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_pitch_back",     "Helicopter Pitch Back",       INPUT_VEH_FLY_PITCH_DOWN,    GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_bank_left",      "Helicopter Bank Left",        INPUT_VEH_FLY_ROLL_LEFT,     GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	{ "heli_bank_right",     "Helicopter Bank Right",       INPUT_VEH_FLY_ROLL_RIGHT,    GTA_CONTROL_TYPE_IN_VEHICLE, true, false },
	// input 145-169 is not covered by N:IV
	{ "toggle_definition",   "Toggle Definition",           INPUT_FRONTEND_DEFINITION,   GTA_CONTROL_TYPE_BOTH,       true, false },
	// input 171-178 is not covered by N:IV
	//INPUT_SNIPER_ZOOM_IN_KB, // merge with sniper_zoom_in
	//INPUT_SNIPER_ZOOM_OUT_KB, // merge with sniper_zoom_out
	{ "phone_accept",        "Phone Accept",                INPUT_KB_PHONE_ACCEPT,       GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "phone_cancel",        "Phone Cancel",                INPUT_KB_PHONE_CANCEL,       GTA_CONTROL_TYPE_BOTH,       true, false },
	{ "heli_secondary_fire", "Helicopter Secondary Fire",   INPUT_VEH_ATTACK3_KB,        GTA_CONTROL_TYPE_IN_VEHICLE, true, false }
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

void CInputManager::Process()
{
	// Get the game pad
	IVPad * pPad = GetGamePad();

	// Do we have a valid game pad?
	if(pPad)
	{
		// Apply gta controls
		for(int i = 0; i < (sizeof(g_GTAControlInfo) / sizeof(InputGTAControl)); i++)
			pPad->m_padData[g_GTAControlInfo[i].action].m_byteUnknown6 = (g_GTAControlInfo[i].bState ? MAX_ANALOG_VALUE : 0);
	}
}

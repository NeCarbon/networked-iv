//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: KeySync.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// WRONG
/*enum eInputKey
{
	INPUT_KEY_LS_LR,
	INPUT_KEY_LS_UD,
	INPUT_KEY_RS_LR,
	INPUT_KEY_RS_UD,
	INPUT_KEY_SPACE = 16,
	INPUT_KEY_SPRINT = 64,
	INPUT_KEY_V = 256,
	INPUT_KEY_C = 512,
	INPUT_KEY_MMB = 1024,
	INPUT_KEY_MOUSE_UP = 8192,
	INPUT_KEY_MOUSE_DOWN = 32768
};*/

/*enum eInputState
{
	INPUT_STATE_ALL = 6,
	INPUT_STATE_IN_VEHICLE = 8,
	INPUT_STATE_ON_FOOT = 9
};*/

struct CPadData_m1_m1
{
	BYTE m_byteUnknown0;          // 0-1
	PAD(CPadData_m1_m1, pad0, 3); // 1-3 (Unknown)
	DWORD m_dwUnknown4;           // 4-8
};

struct CPadData_m1 // CPadData + 0xC = KeyBlock_m1 * pUnknown
{
	CPadData_m1_m1 m_unknown[64]; // 000-200
};

// rage::ioConfig?
class CPadConfig
{
public:
	DWORD dwCount;                // 000-004
	//DWORD dwUnknown;              // 004-008
	//CPadMap * m_pPadMaps[492]; // ??
	PAD(CPadConfig, pad0, 0x7B4); // ?
};

// this is actually rage::ioValue
class CPadData
{
public:
	// Control State Value:
	// Min = 0
	// Default = 0/128
	// Max = 255
	// Get current key (m_byteUnknown4 ^ m_byteUnknown6)
	// Get previous key (m_byteUnknown4 ^ m_byteUnknown7)
	DWORD m_dwVFTable;        // 00-04 (Virtual Function Table (0xD55F2C))
	BYTE m_byteUnknown4;      // 04-05 (Control state value? (xor with byte 6/7))
	BYTE m_byteUnknown5;      // 05-06 (value of 0x188B550 (gets updated every pad update))
	BYTE m_byteUnknown6;      // 06-07 (Control state value (Current?))
	BYTE m_byteUnknown7;      // 07-08 (Control state value (Previous?))
	BYTE m_byteUnknown8;      // 08-09 (current m_dwUnknownC index (0-64))
	PAD(CPadData, pad0, 3);   // 09-0C
	CPadData_m1 * m_pUnknown; // 0C-10 (Pointer to a 512 size dynamically allocated block of memory (KeyBlock_m1) (See 0x8340F0))
};

class CCamData
{
public:
	Matrix34 m_matMatrix;     // 00-40
	Vector3 m_vecUnknown;     // 40-4C
	float m_fUnknown;         // 4C-50
	float m_fFOV;             // 50-54
	float m_fNear;            // 54-58
	float m_fFar;             // 58-5C
	float m_fNearDOF;         // 5C-60
	float m_fFarDOF;          // 60-64
	float m_fMotionBlur;      // 64-68
	float m_fUnknown2;        // 68-6C
	float m_fUnknown3;        // 6C-70
	BYTE m_byteUnknown;       // 70-71
	PAD(CCamData, pad0, 0x3); // 71-74
};

class CCam
{
public:
	PAD(CCam, pad0, 0x10); // 000-010
	CCamData m_data1;      // 010-084
	PAD(CCam, pad1, 0xC);  // 084-090
	CCamData m_data2;      // 090-104
	PAD(CCam, pad2, 0x3C); // 104-140
};

// TODO: FIXME
class IVPad;
class CClientPadState;

IVPad * GetGamePad();
void    ResetGamePadState();
void    SetGamePadState(CClientPadState * padState);
void    GetGamePadState(CClientPadState * padState);
CCam *  GetGameCam();
void    SetGameCameraMatrix(Matrix * matMatrix);
void    GetGameCameraMatrix(Matrix * matMatrix);
BYTE    GetCurrentGameControlValue(BYTE byteControlId);
BYTE    GetPreviousGameControlValue(BYTE byteControlId);
void    InstallKeySyncHooks();

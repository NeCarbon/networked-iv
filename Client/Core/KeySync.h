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

class CPadDataHistoryItem
{
public:
	BYTE m_byteValue;                  // 0-1
	PAD(CPadDataHistoryItem, pad0, 3); // 1-3
	DWORD m_dwLastUpdateTime;          // 4-8
};

#define MAX_HISTORY_ITEMS 64

class CPadDataHistory
{
public:
	CPadDataHistoryItem m_historyItems[MAX_HISTORY_ITEMS]; // 000-200
};

// rage::ioConfig?
class CPadConfig
{
public:
	DWORD dwCount;                // 000-004
	//DWORD dwUnknown;              // 004-008
	//CPadMap * m_pPadMaps[492]; // ??
	PAD(CPadConfig, pad0, 0x7B4); // ?
	// +0x4 - (DWORD) Unknown (Initializes at 0)
	// +0x8 - (DWORD) First Control Type (9 - analog)
	// +0x4 - (DWORD) Next Control Type (Continue for dwCount)
	// +0x528 - (CPadData) Pad Data
	// +0x4 - (CPadData) Next Pad Data (Continue for dwCount)
};

// this is actually rage::ioValue
class CPadData
{
public:
	// Get current key (m_byteUnknown4 ^ m_byteCurrentValue)
	// Get previous key (m_byteUnknown4 ^ m_byteUnknown7)
	DWORD m_dwVFTable;            // 00-04
	BYTE m_byteUnknown4;          // 04-05 (Control state value? (xor with byte 6/7))
	BYTE m_byteContext;           // 05-06 (current context to avoid pad data being swapped more than once per process)
	BYTE m_byteCurrentValue;      // 06-07
	BYTE m_byteLastValue;         // 07-08
	BYTE m_byteHistoryIndex;      // 08-09
	PAD(CPadData, pad0, 3);       // 09-0C
	CPadDataHistory * m_pHistory; // 0C-10
};

class CCamData
{
public:
	Matrix34 m_matMatrix;     // 00-40
	CVector3 m_vecUnknown;     // 40-4C
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

CCam *  GetGameCam();
void    SetGameCameraMatrix(Matrix * matMatrix);
void    GetGameCameraMatrix(Matrix * matMatrix);
void    InstallKeySyncHooks();

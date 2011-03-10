//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: IVCommon.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//          Contains zlib licensed code from ScriptHook by aru
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define PAD(prefix, name, size) BYTE prefix##_##name##[size]

template <typename T>
struct CSimpleCollection
{
	T ** Data;
	unsigned short Count;
	unsigned short Size;
};

enum eThreadState
{
	ThreadStateIdle,
	ThreadStateRunning,
	ThreadStateKilled,
	ThreadState3,
	ThreadState4, // Sets opsToExecute to 1100000, and state to Idle in CallNative
};

class scrNativeCallContext
{
protected:
	DWORD m_pReturn;			  // 00-04
	unsigned int m_nArgCount;	  // 04-08
	DWORD m_pArgs;				  // 08-0C

	unsigned int m_nDataCount;	  // 0C-10
	Vector3 * m_pOriginalData[4]; // 10-40
	Vector4 m_TemporaryData[4];	  // 40-80
};

struct scrThreadContext
{
	unsigned int ThreadId;	 // 00-04
	unsigned int ScriptHash; // 04-08
	eThreadState State;		 // 08-0C
	unsigned int IP;		 // 0C-10
	unsigned int FrameSP;	 // 10-14
	unsigned int SP;		 // 14-18
	unsigned int TimerA;	 // 18-1C
	unsigned int TimerB;	 // 1C-20
	unsigned int TimerC;	 // 20-24
	float WaitTime;			 // 24-28
	unsigned int _f28;		 // 28-2C
	unsigned int _f2C;		 // 2C-30
	unsigned int _f30;		 // 30-34
	unsigned int _f34;		 // 34-38
	unsigned int _f38;		 // 38-3C
	unsigned int _f3C;		 // 3C-40
	unsigned int _f40;		 // 40-44
	unsigned int ExIP;		 // 44-48
	unsigned int ExFrameSP;	 // 48-4C
	unsigned int ExSP;		 // 4C-50
	unsigned int _f50;		 // 50-54
};

class scrThread
{
protected:
								// 00-04
	scrThreadContext m_Context; // 04-58
	DWORD m_pStack;				// 58-5C
	unsigned int _f5C;			// 5C-60
	DWORD m_pXLiveBuffer;		// 60-64
	unsigned int _f64;			// 64-68
	unsigned int _f68;			// 68-6C
	char * m_pszExitMessage;	// 6C-70
/*
public:
	virtual ~scrThread() {}
	virtual eThreadState Reset(unsigned int scriptHash, DWORD pArgs, unsigned int argCount) = 0;
	virtual eThreadState Run(unsigned int opsToExecute) = 0;
	virtual eThreadState Loop(unsigned int opsToExecute) = 0;
	virtual void Kill() = 0;

	scrThreadContext * GetContext() { return &m_Context; }*/
};

class GtaThread : public scrThread
{
protected:
											  // 00-70
	char m_szProgramName[24];				  // 70-88
	unsigned int _f88;						  // 88-8C
	unsigned int _f8C;						  // 8C-90
	unsigned int _f90;						  // 90-94
	bool _f94;								  // 94-95
	bool _f95;								  // 95-96
	bool m_bThisScriptShouldBeSaved;		  // 96-97
	bool m_bPlayerControlOnInMissionCleanup;  // 97-98
	bool m_bClearHelpInMissionCleanup;		  // 98-99
	bool _f99;								  // 99-9A
	bool m_bAllowNonMinigameTextMessages;	  // 9A-9B
	bool _f9B;								  // 9B-9C
	bool _f9C;								  // 9C-9D
	bool m_bCanBePaused;					  // 9D-9E
	bool _f9E;								  // 9E-9F
	bool _f9F;								  // 9F-A0
	bool _fA0;								  // A0-A4
	bool m_bCanRemoveBlipsCreatedByAnyScript; // A4-A5
	bool _fA5;								  // A5-A6
	bool _fA6;								  // A6-A7
	bool _fA7;								  // A7-A8
	unsigned int _fA8;						  // A8-AC
	unsigned int m_nFlags;					  // AC-B0
};

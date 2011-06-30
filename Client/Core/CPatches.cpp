//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPatches.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

GameLoadCallback_t    g_pfnGameLoadCallback = NULL;
GameProcessCallback_t g_pfnGameProcessCallback = NULL;
GtaThread           * g_pScriptThread = NULL;
bool                  g_bScriptLoaded = false;

unsigned int          uiPlayerInfoIndex = 0;
IVPlayerInfo        * pReturnedPlayerInfo = NULL;
unsigned int          uiReturnedIndex = 0;
//bool                  bInvalidReturn = false;
IVPlayerPed         * _pPlayerPed = NULL;
IVTask              * ___pTask = NULL;
DWORD                 dwTaskDestructorReturn = NULL;
DWORD                 dwTaskVFTable = NULL;
DWORD                 dwGameShutdownReturn = NULL;

void GameShutdown_Hook();
void CTask__Destructor_Hook();
void CEpisodes__IsEpisodeAvaliable_Hook();
void DisableLoadingScreens_Hook();
void GetPlayerInfoFromIndex_Hook();
void GetIndexFromPlayerInfo_Hook();
void GetLocalPlayerPed_Hook();
void GetRunningScriptThread_Hook();
void CScriptVM__Process_Hook();

void ToggleVehicleEntryExit(bool bVehicleEntry, bool bVehicleExit)
{
	if(!bVehicleEntry)
	{
		// Disable vehicle entrys
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x9B1ED0) = 0x0CC2C033;
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x9B1ED4) = 0x00;
	}
	else
	{
		// Enable vehicle entrys
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x9B1ED0) = 0x0C24448B;
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x9B1ED4) = 0x8B;
	}

	// NOTE: When vehicle exits are disabled, pressing the exit vehicle key
	// still applies the vehicle handbrake, fix this asap
	if(!bVehicleExit)
	{
		// Disable vehicle exits
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x9BBBFC) = 0xEB;
	}
	else
	{
		// Enable vehicle exits
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x9BBBFC) = 0x74;
	}
}

void StartGame();

void CPatches::Initialize()
{
	// Create a new script thread structure
	g_pScriptThread = new GtaThread;
	memset(g_pScriptThread, 0, sizeof(GtaThread));

	if(COffsets::GetVersion() == GAME_VERSION_7)
	{
		// 0x10C7F80 = game state or some shit
		// Unprotect the .text segment
		CPatcher::Unprotect((g_pClient->GetBaseAddress() + 0x400000 + 0x1000), 0x94C000);

		// Unprotect the .rdata segment
		CPatcher::Unprotect((g_pClient->GetBaseAddress() + 0x400000 + 0x94D000), 0x1BF000);

		// Disable vehicle entry's and exit's
		ToggleVehicleEntryExit(false, false);

		//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x4205B0), (DWORD)t);

		// Force CEpisodes::IsEpisodeAvaliable to always return 0
		/**(DWORD *)(g_pClient->GetBaseAddress() + 0x814810) = 0x04C2C032; // xor al, al; retn 
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x814814) = 0x00; // 4*/

		// Hook CTask::~CTask to use our own function
#define FUNC_CTask__Destructor 0xA288D0
		CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + FUNC_CTask__Destructor), (DWORD)CTask__Destructor_Hook);

		// Hook CEpisodes::IsEpisodeAvaliable to use our own function
		CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x814810), (DWORD)CEpisodes__IsEpisodeAvaliable_Hook);

		// Force IsDLCReady to always return 0
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x407320) = 0x90C3C032; // xor al, al; retn; nop

		// Force the title menu select handler to always use the 'Start' option
		//*(BYTE *)(g_pClient->GetBaseAddress() + 0x857133) = 0xE0;

		// Install title menu skip hook
		//CPatcher::InstallCallPatch((g_pClient->GetBaseAddress() + 0x40BF9D), (DWORD)StartGame);
		//CPatcher::InstallCallPatch((g_pClient->GetBaseAddress() + 0x40BF48), (DWORD)StartGame);

		// Disable call to CreateWebBrowser
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x42E91A), 5);

		// Disable call to CreateSocialWebBrowser
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x41F68E), 5);

		//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0xC28820), (DWORD)LogFunc_Hook);

		// Don't load the 'playidles' animations (Don't work?)
		//*(BYTE *)(g_pClient->GetBaseAddress() + 0xD8195C) = '\0';

		//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0xA5FD80), (DWORD)TestFunc_Hook);

		// For when wanted level is full?
		//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x922A45), (DWORD)CrashFix1_Hook);

		// test start game func hook
		//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x7B9980), (DWORD)StartGame_Hook);

		// test eflc selector hook
		// 0x858C70 - tlad/tbogt screen loader
		// 0x859E00 - eflc selector input handler (vanilla/iv screen and tlad/tbogt screen) handler
		// 0x859F64 - load tbogt menu (episode 2)
		// 0x859E60 - load tlad menu (episode 1)
		// 0x859E25 - load vanilla menu (episode 0)
		// 0x8589D3 - load vanilla/iv screen
		// 0x41FD90 - CGame::StartGame
		//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x8589D3), (g_pClient->GetBaseAddress() + 0x859E25));

		// Disable rendering of distant cars at night
		/*CPatcher::InstallRetnPatch(g_pClient->GetBaseAddress() + 0x9055D0);*/

		// Make the game think we are not connected to the internet
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x7AF1A0) = 0x90909090; // nop * 4
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x7AF1A4) = 0x90909090; // nop * 4
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x7AF1AF) = 0x90C300B0; // mov al, 0; retn; nop

		// Install patch to disable loading screens (Overwrites call to esp check)
		CPatcher::InstallCallPatch((g_pClient->GetBaseAddress() + 0x424B26), (DWORD)DisableLoadingScreens_Hook);

		// Disable the 'Loading...' text when the screen is faded
		//CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x7BC874), 5);

		// Hook GetPlayerInfoFromIndex to use our own function
		CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x817F20), (DWORD)GetPlayerInfoFromIndex_Hook);

		// Hook GetIndexFromPlayerInfo to use our own function
		CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x817DC0), (DWORD)GetIndexFromPlayerInfo_Hook);

		// Hook GetLocalPlayerPed to use our own function
		CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x817F40), (DWORD)GetLocalPlayerPed_Hook);

		// Hook GetPlayerPedFromPlayerInfo to use our own function
		//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x8788D0), (DWORD)GetPlayerPedFromPlayerInfo_Hook);

		// Hook GetRunningScriptThread to return our own script thread structure
		CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x5A4CA0), (DWORD)GetRunningScriptThread_Hook);

		// Hook for CScriptVM::Process
		CPatcher::InstallCallPatch((g_pClient->GetBaseAddress() + 0x80A092), (DWORD)CScriptVM__Process_Hook);

		// *(float *)(g_pClient->GetBaseAddress() + 0x1083FF0) = 6000.0f; // Game Speed (Default 1000.0f)?

		// Skip main menu
		//CPatcher::InstallCallPatch((g_pClient->GetBaseAddress() + 0x40BF9D), (DWORD)StartGame);

		// Don't load startup.sco
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x809A81) = 0xEB;
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x809A82) = 0x61;

		// Always start a new game
		// TODO: Change, this is messy
		//*(BYTE *)(g_pClient->GetBaseAddress() + 0x5B0311) = 0xEB; // jmp
		//*(BYTE *)(g_pClient->GetBaseAddress() + 0x5B0312) = 0xAE; // +AE
		// 0x5B0311 = 0xEB // jmp
		// 0x5B0312 = 0xAE // +AE
		CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x5B0311), (g_pClient->GetBaseAddress() + 0x5B03BF));

		// Stop game pausing
		// TODO

		// NOTE: All the functions below are all called from 0x4212A0 in one way or another
		// so find out what 0x4212A0 does

		// Disable parked cars
		CPatcher::InstallRetnPatch(g_pClient->GetBaseAddress() + 0xB3EDF0);

		// Disable emergency services and garbage trucks
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x4215CF), 5);

		// Disable random peds and vehicles
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x8ACD64), 5);

		// Disable scenario peds
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x9F72C0) = 0xB8; // mov eax,
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x9F72C1) = 0x0; // 0
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x9F72C5) = 0xC3; // retn

		// Shorten the initial sleep time
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x401835) = 1;

		// Disable error reporting
		CPatcher::InstallRetnPatch((g_pClient->GetBaseAddress() + 0xD356D0));

		// Certificates check
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x403F10) = 0x900008C2;

		// 
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x40262D) = 0x33; // xor eax
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x40262E) = 0xC0; // eax
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x40262F) = 0xE9; // jmp
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x402630) = 0x4A; // +4Ah

		// Skip RGSC connect and EFC checks (jmp 40289E)
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x402631) = 0x90000002;

		// NOP; MOV [g_rgsc], eax
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x402883) = 0x90;
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x402884) = 0xA3;

		// Disable VDS101 error
		// TODO

		// Disable VDS102 error
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x4028ED), 42);

		// Last RGSC init check
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x40291D), 6);

		// 
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x402B12), 14);
		CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x402D17), 14);

		// 
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x403870) = 0x90CC033; // xor eax, eax; retn
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x404250) = 0x90CC033; // xor eax, eax; retn

		// Disable securom spot checks (mov al, 1; retn; nop)
		*(DWORD *)(g_pClient->GetBaseAddress() + 0xBAC160) = 0x90C301B0;
		*(DWORD *)(g_pClient->GetBaseAddress() + 0xBAC180) = 0x90C301B0;
		*(DWORD *)(g_pClient->GetBaseAddress() + 0xBAC190) = 0x90C301B0;
		*(DWORD *)(g_pClient->GetBaseAddress() + 0xBAC1C0) = 0x90C301B0;
	}
}

void CPatches::Shutdown()
{
	// Delete the script thread structure
	delete g_pScriptThread;
}

void CPatches::SetGameLoadCallback(GameLoadCallback_t pfnGameLoadCallback)
{
	g_pfnGameLoadCallback = pfnGameLoadCallback;
}

void CPatches::SetGameProcessCallback(GameProcessCallback_t pfnGameProcessCallback)
{
	g_pfnGameProcessCallback = pfnGameProcessCallback;
}

void _declspec(naked) GetRunningScriptThread_Hook()
{
	_asm
	{
		mov eax, g_pScriptThread
		retn
	}
}

int CScriptVM__Process()
{
	if(!g_bScriptLoaded)
	{
		// Get the script thread pool
		CSimpleCollection<GtaThread> * pThreads = (CSimpleCollection<GtaThread> *)(g_pClient->GetBaseAddress() + VAR_ScrVM__ThreadPool_7);

		// Set the current script count and script thread pool size to 0
		pThreads->Size = 0;
		pThreads->Count = 0;

		// Create the local player
		unsigned int uiPlayerIndex;
		InvokeNative<void *>(0x335E3951, 0, 0.0f, 0.0f, 0.0f, &uiPlayerIndex); // CREATE_PLAYER

		// Get the local players char handle
		unsigned int uiCharHandle;
		InvokeNative<void *>(0x511454A9, uiPlayerIndex, &uiCharHandle); // GET_PLAYER_CHAR

		// Set the local players initial coordinates
		InvokeNative<void *>(0x689D0F5F, uiCharHandle, 0.0f, 0.0f, 0.0f); // SET_CHAR_COORDINATES

		// Set the local players initial heading
		InvokeNative<void *>(0x46B5523B, uiCharHandle, 0.0f); // SET_CHAR_HEADING

		// Disable wanted level
		InvokeNative<void *>(0x5D622498, 0); // SET_MAX_WANTED_LEVEL

		// Temporary code
		InvokeNative<void *>(0x689D0F5F, uiCharHandle, -341.36f, 1144.80f, 14.79f); // SET_CHAR_COORDINATES
		InvokeNative<void *>(0x46B5523B, uiCharHandle, 40.114815f); // SET_CHAR_HEADING

		// NOTE: Screen is faded out by default, so if we ever decide to remove the screen fade till connect
		// remember to fade the screen back in here :)

		// Call the game load callback (If we have one)
		if(g_pfnGameLoadCallback)
			g_pfnGameLoadCallback();

		// Flag the script as loaded
		g_bScriptLoaded = true;
	}
	else
	{
		// Call the game process callback (If we have one)
		if(g_pfnGameProcessCallback)
			g_pfnGameProcessCallback();
	}

	return 1;
}

void _declspec(naked) CScriptVM__Process_Hook()
{
	// Usually this would loop through all running script threads
	// and call scrThread::Loop with opsToExecute set to arg0 or 
	// 1100000 (if arg0 has a value of 0)
	_asm
	{
		pushad
	}

	CScriptVM__Process();

	_asm
	{
		popad
		mov al, 1
		retn
	}
}

void _declspec(naked) StartGame_Hook()
{
	_asm
	{
		pushad
		int 3
		popad
	}
}

void _declspec(naked) EFLCSelect_Hook()
{
	_asm
	{
		pushad
		int 3
		popad
	}
}

IVPlayerInfo * GetPlayerInfoFromIndex(unsigned int uiIndex)
{
	// Default to the local player info just incase the index is invalid
	pReturnedPlayerInfo = CPools::GetPlayerInfoFromIndex(0);

	// Is this not the local player info?
	if(uiIndex != 0)
	{
		CContextData * pContextInfo = CContextDataManager::GetContextData(uiIndex);

		if(pContextInfo)
			pReturnedPlayerInfo = pContextInfo->GetPlayerInfo()->GetPlayerInfo();
	}
	
	return pReturnedPlayerInfo;
}

void _declspec(naked) GetPlayerInfoFromIndex_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov uiPlayerInfoIndex, eax
		pushad
	}

	GetPlayerInfoFromIndex(uiPlayerInfoIndex);

	_asm
	{
		popad
		mov eax, pReturnedPlayerInfo
		retn
	}
}

unsigned int GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Default to the local player info just incase the player info is invalid
	uiReturnedIndex = 0;

	// Is this not the local player info?
	if(pPlayerInfo != CPools::GetPlayerInfoFromIndex(0))
	{
		CContextData * pContextInfo = CContextDataManager::GetContextData(pPlayerInfo);

		if(pContextInfo)
			uiReturnedIndex = pContextInfo->GetPlayerInfo()->GetPlayerNumber();
	}

	return uiReturnedIndex;
}

void _declspec(naked) GetIndexFromPlayerInfo_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov pReturnedPlayerInfo, eax
		pushad
	}

	GetIndexFromPlayerInfo(pReturnedPlayerInfo);

	_asm
	{
		popad
		mov eax, uiReturnedIndex
		retn
	}
}

IVPlayerPed * GetLocalPlayerPed()
{
	// Reset the player ped pointer
	_pPlayerPed = NULL;

	// Is the local player id valid?
	if(CPools::GetLocalPlayerIndex() != -1)
	{
		// Default to the local player ped (If available)
		IVPlayerInfo * pPlayerInfo = CPools::GetPlayerInfoFromIndex(0);

		if(pPlayerInfo)
		{
			_pPlayerPed = pPlayerInfo->m_pPlayerPed;
		//	CLogFile::Printf("GetLocalPlayerPed Set To REAL Local Player Ped");
		}

		if(CPools::GetLocalPlayerIndex() != 0)
		{
			CContextData * pContextInfo = CContextDataManager::GetContextData((BYTE)CPools::GetLocalPlayerIndex());

			if(pContextInfo)
			{
				_pPlayerPed = pContextInfo->GetPlayerPed()->GetPlayerPed();
		//		CLogFile::Printf("GetLocalPlayerPed Set To Remote Local Player Ped");
			}
		}
	}
	else
	{
		//CLogFile::Printf("GetLocalPlayerPed Invalid Local Player Index");
	}

	/*if(_pPlayerPed == NULL)
	{
		if(!bInvalidReturn)
		{
			CLogFile::Printf("GetLocalPlayerPed Return Is Invalid (Start)");
			bInvalidReturn = true;
		}
	}
	else
	{
		if(bInvalidReturn)
		{
			CLogFile::Printf("GetLocalPlayerPed Return Is Invalid (End)");
			bInvalidReturn = false;
		}
	}*/

	return _pPlayerPed;
}

void _declspec(naked) GetLocalPlayerPed_Hook()
{
	_asm
	{
		pushad
	}

	GetLocalPlayerPed();

	_asm
	{
		popad
		mov eax, _pPlayerPed
		retn
	}
}

IVPlayerPed * GetPlayerPedFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Reset the player ped pointer
	_pPlayerPed = NULL;

	// Is the player info pointer and player info player ped pointer valid?
	if(pPlayerInfo)
		_pPlayerPed = pPlayerInfo->m_pPlayerPed;
	else
	{
		// Player info pointer is invalid, use the local player ped
		_pPlayerPed = GetLocalPlayerPed();
	}

	return _pPlayerPed;
}

void _declspec(naked) GetPlayerPedFromPlayerInfo_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		mov pReturnedPlayerInfo, eax
		pushad
	}

	GetPlayerPedFromPlayerInfo(pReturnedPlayerInfo);

	_asm
	{
		popad
		mov eax, _pPlayerPed
		retn
	}
}

#define VAR_AudioFrontend 0x10F85B0
#define FUNC_StartGame 0x7B9980

void StartGame()
{
	CLogFile::Printf("startgame");
	DWORD dwFunc = (g_pClient->GetBaseAddress() + 0x407420);
	_asm
	{
		call dwFunc
	}
	/*DWORD dwVar = (g_pClient->GetBaseAddress() + VAR_AudioFrontend);
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_StartGame);
	_asm
	{
		mov ecx, dwVar
		call dwFunc
	}*/
}

#if 0
#define VAR_AudioFrontend 0x10F85B0
#define FUNC_StartGame 0x7B9980

void StartGame()
{
	CPatcher::InstallNopPatch((g_pClient->GetBaseAddress() + 0x859E02), 0x23);
	DWORD dwFunc2 = (g_pClient->GetBaseAddress() + /*0x859E25*/0x859E00);
	_asm call dwFunc2
	*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C74E1) = 1;
	// test
	*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C74E1) = 0;
	*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C79E4) = 1;
	*(BYTE *)(g_pClient->GetBaseAddress() + 0x119EB14) = 0;
	DWORD dwFunc3 = (g_pClient->GetBaseAddress() + 0x407420);
	_asm call dwFunc3
	// test end
	DWORD dwVar = (g_pClient->GetBaseAddress() + VAR_AudioFrontend);
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_StartGame);
	_asm
	{
		mov ecx, dwVar
		call dwFunc
	}
}
#endif

/*IVPlayerInfo * _pPlayerInfo = NULL;
DWORD _dwAddress = NULL;

void CrashFix1()
{
	
	CLogFile::Printf("Player index %d player info 0x%p", CPools::GetLocalPlayerIndex(), _pPlayerInfo);
}

void _declspec(naked) CrashFix1_Hook()
{
	_asm
	{
		pushad
	}

	_dwAddress = (g_pClient->GetBaseAddress() + 0x922A5A);

	_asm
	{
		popad
		mov ecx, _pPlayerInfo
		jmp _dwAddress
	}
}

void TestFunc()
{
	g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "CTaskComplexGun::Constructor");
}

void _declspec(naked) TestFunc_Hook()
{
	_asm
	{
		pushad
	}

	TestFunc();
	_dwAddress = (g_pClient->GetBaseAddress() + 0xA5FD86);

	_asm
	{
		popad
		push ebp
		mov ebp, esp
		and esp, 0FFFFFFF0h
		jmp _dwAddress
	}
}*/

void LogFunc_Hook(int a1, int a2, const char *a3, ...)
{
	if(a3)
		CLogFile::Printf("%s", a3);
}

void DisableLoadingScreens()
{
	// Legal, Legal 2, R*, R*N, GTA:IV, ...
	for(int i = 0; i < *(int *)(COffsets::VAR_NumLoadingScreens); ++i)
	{
		*(DWORD *)(COffsets::VAR_FirstLoadingScreenType + i * 400) = ((i <= 4) ? 4 : i);

		if(i <= 4)
			*(DWORD *)(COffsets::VAR_FirstLoadingScreenDuration + i * 400) = 0;
	}
}

void _declspec(naked) DisableLoadingScreens_Hook()
{
	_asm
	{
		pushad
	}

	DisableLoadingScreens();

	_asm
	{
		popad
		retn
	}
}

void _declspec(naked) CEpisodes__IsEpisodeAvaliable_Hook()
{
	_asm
	{
		mov eax, [esp+4]
		test eax, eax
		jnz it_not
		mov al, 1
		retn 4
it_not:
		xor al, al
		retn 4
	}
}

void _declspec(naked) t()
{
	_asm int 3
}

void _declspec(naked) CTask__Destructor_Hook()
{
	_asm
	{
		mov ___pTask, ecx
		pushad
	}


	// Do we have a client task manager?
	if(g_pClient->GetClientTaskManager())
	{
		// Let the client task manager handle this task deletion
		g_pClient->GetClientTaskManager()->HandleTaskDelete(___pTask);
	}

	dwTaskDestructorReturn = (g_pClient->GetBaseAddress() + 0xA288DA);
	dwTaskVFTable = (g_pClient->GetBaseAddress() + 0xD87224);

	_asm
	{
		popad
		push esi
		mov esi, ecx
		push esi
		mov dword ptr [esi], offset dwTaskVFTable
		jmp dwTaskDestructorReturn
	}
}

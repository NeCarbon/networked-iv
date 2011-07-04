//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClient.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CRootEntity *      g_pRootEntity = NULL;
CResourceManager * g_pResourceManager = NULL;

void CClient::SetFocused(bool bFocused)
{
	// Set the focused flag
	m_bFocused = bFocused;

	if(bFocused)
	{
		// Enable game input
		SetInputState(true);
	}
	else
	{
		// Disable game input
		SetInputState(false);
	}
}

class CCommandLine
{
private:
	std::map<String, String> m_commandLine;

public:
	String Get(String strOption) { return m_commandLine[strOption]; }

	void   Parse()
	{
		// Get the command line string
		char * szCommandLine = GetCommandLine();

		// Loop until we reach the end of the command line string
		while(*szCommandLine)
		{
			// Is the current char not a space?
			if(!isspace(*szCommandLine))
			{
				// Is the current char a '-'?
				if(*szCommandLine == '-')
				{
					// Collect the option string
					String strOption;

					while(*szCommandLine && !isspace(*szCommandLine))
					{
						strOption += *szCommandLine;
						szCommandLine++;
					}

					// If we have run out of command line to process break out of the loop
					if(!(*szCommandLine))
						break;

					// Skip the spaces between the option and the value
					while(*szCommandLine && isspace(*szCommandLine))
						szCommandLine++;

					// If we have run out of command line to process break out of the loop
					if(!(*szCommandLine))
						break;

					// Collect the value string
					String strValue;
					// TODO: Check if the value first char is a '-'?

					while(*szCommandLine && !isspace(*szCommandLine))
					{
						strValue += *szCommandLine;
						szCommandLine++;
					}

					// Add the option and value to the command line
					m_commandLine[strOption] = strValue;

					// If we have run out of command line to process break out of the loop
					if(!(*szCommandLine))
						break;
				}
			}

			// Increment the command line string pointer
			szCommandLine++;
		}
	}
};

bool CClient::OnLoad()
{
	// Open the log file
	CLogFile::Open("Client.log");

	// Set our log file callback
	CLogFile::SetCallback(LogFileCallback);

	// Set our log file to use our callback
	CLogFile::SetUseCallback(true);

	// Install our exception handler
	CExceptionHandler::Install();

	// Set our exception handler callback
	CExceptionHandler::SetCallback(ExceptionHandlerCallback);

	// Reset the class object pointers
	m_pGame = NULL;
	m_pXLiveHook = NULL;
	m_pDirect3D9Hook = NULL;
	m_pNetworkManager = NULL;
	m_pPlayerManager = NULL;
	m_pFont = NULL;
	m_pChatWindow = NULL;

	// Reset the port and set the nick limit
	m_usPort = 0;
	m_strNick.SetLimit(NICK_MAX);

	// Reset the flags
	m_bInputState = false;
	m_bFocused = false;

	// Get command line options
	CCommandLine commandLine;
	commandLine.Parse();
	m_strIp = commandLine.Get("-ip");
	m_usPort = commandLine.Get("-port").ToInteger();
	m_strPassword = commandLine.Get("-password");
	m_strNick = commandLine.Get("-nick");

	// Do we not have an ip or nick?
	if(m_strIp.IsEmpty() || m_strNick.IsEmpty() || !m_usPort)
	{
		MessageBox(NULL, "Not enough command line parameters specified. Cannot launch Networked: IV.", 
			NULL, NULL);
		return false;
	}

	// Get the application base address
	m_uiBaseAddress = (unsigned int)GetModuleHandle(NULL);

	// If the base address is not valid, exit
	if(!m_uiBaseAddress)
		return false;

	// Subtract the image base from the base address
	m_uiBaseAddress -= 0x400000;

	// Log the base address
	CLogFile::Printf("base address is 0x%p", m_uiBaseAddress);

	// Create the game instance
	m_pGame = new CGame();

	// If the instance creation failed, exit
	if(!m_pGame)
		return false;

	CLogFile::Printf("game instance created");

	// Create the xlive hook instance
	m_pXLiveHook = new CXLiveHook();

	// If the instance creation failed, exit
	if(!m_pXLiveHook)
		return false;

	CLogFile::Printf("xlive hook instance created");

	// Install the xlive hook
	m_pXLiveHook->Install();

	CLogFile::Printf("xlive hook installed");

	// Create the d3d9 hook instance
	m_pDirect3D9Hook = new CDirect3D9Hook();

	// If the instance creation failed, exit
	if(!m_pDirect3D9Hook)
		return false;

	CLogFile::Printf("d3d9 hook instance created");

	// Install the d3d9 hook
	m_pDirect3D9Hook->Install();

	CLogFile::Printf("d3d9 hook installed");

	// Create the dinput hook instance
	m_pDirectInput8Hook = new CDirectInput8Hook();

	// If the instance creation failed, exit
	if(!m_pDirectInput8Hook)
		return false;

	CLogFile::Printf("dinput hook instance created");

	// Install the dinput hook
	m_pDirectInput8Hook->Install();

	// Initialize the net module, if it fails, exit
	if(!CNetworkModule::Init())
	{
		CLogFile::Printf("Failed to initialize the net module!");
		return false;
	}

	CLogFile::Printf("net module initialized");

	// Create the network manager instance
	m_pNetworkManager = new CClientNetworkManager();

	CLogFile::Printf("network manager instance created");

	// Create the client task manager instance
	m_pClientTaskManager = new CClientTaskManager();

	CLogFile::Printf("client task manager instance created");

	// Create the resource and scripting manager
	CEntityIDs::Initalize();
	g_pRootEntity = new CRootEntity();
	g_pResourceManager = new CResourceManager("client/resources/");

	// Success!
	return true;
}

void CClient::OnUnload()
{
	CLogFile::Printf("Client unload 1");

	m_pNetworkManager->Disconnect();

	CLogFile::Printf("Game shutdown 1");

	// Delete the Root Entity instance
	SAFE_DELETE(g_pRootEntity);

	CLogFile::Printf("Game shutdown 2");

	// Delete the resource manager
	SAFE_DELETE(g_pResourceManager);

	CLogFile::Printf("Game shutdown 3");

	// Delete the chat window instance
	SAFE_DELETE(m_pChatWindow);

	CLogFile::Printf("Game shutdown 4");

	// Delete the font instance
	SAFE_DELETE(m_pFont);

	CLogFile::Printf("Game shutdown 5");

	// Delete the streamer instance
	SAFE_DELETE(m_pStreamer);

	CLogFile::Printf("Game shutdown 6");

	// Delete the player manager instance
	SAFE_DELETE(m_pPlayerManager);

	CLogFile::Printf("Game shutdown 7");

	// Delete the vehicle manager instance
	SAFE_DELETE(m_pVehicleManager);

	CLogFile::Printf("Game shutdown 8");

	// Delete the client task manager instance
	SAFE_DELETE(m_pClientTaskManager);

	CLogFile::Printf("Game shutdown 9");

	// Delete the network manager instance
	SAFE_DELETE(m_pNetworkManager);

	CLogFile::Printf("Game shutdown 10");

	// Shutdown the net module
	CNetworkModule::Shutdown();

	CLogFile::Printf("Game shutdown 11");

	// Uninstall the dinput hook
	if(m_pDirectInput8Hook)
		m_pDirectInput8Hook->Uninstall();

	CLogFile::Printf("Game shutdown 13");

	// Delete the dinput hook instance
	SAFE_DELETE(m_pDirectInput8Hook);

	CLogFile::Printf("Game shutdown 14");

	// Uninstall the d3d9 hook
	if(m_pDirect3D9Hook)
		m_pDirect3D9Hook->Uninstall();

	CLogFile::Printf("Game shutdown 15");

	// Delete the d3d9 hook instance
	SAFE_DELETE(m_pDirect3D9Hook);

	CLogFile::Printf("Game shutdown 16");

	// Uninstall the xlive hook
	m_pXLiveHook->Uninstall();

	CLogFile::Printf("Game shutdown 17");

	// Delete the xlive hook instance
	SAFE_DELETE(m_pXLiveHook);

	CLogFile::Printf("Game shutdown 18");

	CLogFile::Printf("Client unload 2");

	// Close the log file
	CLogFile::Close();
}

void CClient::LogFileCallback(const char * szBuffer)
{
	// Get the client chat window pointer
	CChatWindow * pChatWindow = g_pClient->m_pChatWindow;

	// Do we have a valid chat window instance?
	if(pChatWindow)
	{
		// Add the message to the chat window
		bool bLogMessages = pChatWindow->GetLogMessages();
		pChatWindow->SetLogMessages(false);
		g_pClient->m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, szBuffer);
		pChatWindow->SetLogMessages(bLogMessages);
	}
}

void CClient::ExceptionHandlerCallback(_EXCEPTION_POINTERS * ExceptionInfo)
{
	// Write the client information header
	CLogFile::Printf("Client information: ");

	// Write the local player index
	CLogFile::Printf("LPI: %d", CPools::GetLocalPlayerIndex());
}

void CClient::OnD3DCreateDevice(IDirect3DDevice9 * pD3DDevice)
{
	CLogFile::Printf("d3d create");
	// Create our font instance
	m_pFont = new CD3DXFont(pD3DDevice, 14, 0, FW_BOLD, "Tahoma");

	// Create out chat window instance
	m_pChatWindow = new CChatWindow(m_pFont);

	// Set the chat window to log messages
	m_pChatWindow->SetLogMessages(true);

	// Testing code
	m_pChatWindow->Enable();
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Chat window initialized!");

	// Initalize GUI
	CLogFile::Printf("Initalizing GUI");
	m_pGUI = new CGUI(pD3DDevice);
	CLogFile::Printf("GUI loaded");
}

void CClient::OnD3DLostDevice(IDirect3DDevice9 * pD3DDevice)
{
	// Does the font exist?
	if(m_pFont)
	{
		// Inform it of the device loss
		m_pFont->OnDeviceLost();
	}
}

void CClient::OnD3DResetDevice(IDirect3DDevice9 * pD3DDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
	// Does the font exist?
	if(m_pFont)
	{
		// Inform it of the device reset
		m_pFont->OnDeviceReset();
	}

	if(m_pGUI)
	{
		// Update screen size for GUI
		m_pGUI->SetScreenSize(pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);
	}
}

void CClient::OnD3DBeginScene(IDirect3DDevice9 * pD3DDevice)
{
	
}

// dword_10C7F80 == 0 = Inactive
// dword_10C7F80 == 9 = Load Main Menu
// dword_10C7F80 == 2/6 when pause menu active, else inactive
// byte_10C7F6F == 1 when menu active, else inactive

bool IsMenuShown()
{
	if(*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C7F6F) == 1)
		return true;

	return false;
}

bool IsMainMenuShown()
{
	if(*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) == 9)
		return true;

	return false;
}

void ShowMenu()
{
	*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C7F6F) = 1;
}

void HideMenu()
{
	*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C7F6F) = 0;
}

DWORD __dwReturnAddress = 0;
DWORD __dwTemp = 0;

void _declspec(naked) FuncHook()
{
	_asm
	{
		mov eax, [esp]
		mov __dwReturnAddress, eax
		mov eax, [esp+4]
		mov __dwTemp, eax
		pushad
	}

	if(*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) != __dwTemp)
		CLogFile::Printf("Call of sub_7870A0(%d) from 0x%p", __dwTemp, (__dwReturnAddress - g_pClient->GetBaseAddress()));
	*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = __dwTemp;

	_asm
	{
		popad
		mov al, 1
		retn
	}
}

DWORD dwVal1 = 0;
BYTE byteVal2 = 0;
/*bool bFlagged = false;
bool bFlagged2 = false;*/
bool bLoaded = false;
bool bHooked = false;
bool bHideMenu = true;
bool bSet = true;
bool bLol = false;
bool bLmao = false;
BYTE byteSomeValOld = 0;
DWORD __dwRet = 0;
DWORD __dwViewVal;

void _declspec(naked) MyHook()
{
	_asm
	{
		mov eax, [esp]
		mov __dwReturnAddress, eax
		mov eax, [esp+4]
		mov __dwTemp, eax
		pushad
	}

	CLogFile::Printf("sub_4205B0 called with arg %d from 0x%p", __dwTemp, (__dwReturnAddress - g_pClient->GetBaseAddress()));
	__dwViewVal = *(DWORD *)(g_pClient->GetBaseAddress() + 0x10F47F4);
	__dwRet = (g_pClient->GetBaseAddress() + 0x4205B9);

	_asm
	{
		popad
		push    ecx
		push    ebx
		push    esi
		mov     esi, __dwViewVal
		jmp __dwRet
	}
}

void CClient::OnD3DEndScene(IDirect3DDevice9 * pD3DDevice)
{
	if(GetAsyncKeyState(VK_F2) & 1)
	{
		// Show options screen
		/**(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7854) = 59;
		*(DWORD *)(g_pClient->GetBaseAddress() + 0xF0C01C) = -1;
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 66;*/
		// Start game
		//0x33 0xC0 0x83 0xF8 0x00 0x90 0x90 0x90 0x90 0x90 0x90 0x90
		/**(DWORD *)(g_pClient->GetBaseAddress() + 0x857131) = 0xF883C033;
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x857135) = 0x00;
		memset((void *)(g_pClient->GetBaseAddress() + 0x857136), 0x90, 7);
		DWORD dwFunc = (g_pClient->GetBaseAddress() + 0x857130);
		_asm
		{
			call dwFunc
		}*/
		CLogFile::Printf("BEGIN");
		int v7 = 0;
		/*DWORD dwFunc = (g_pClient->GetBaseAddress() + 0x5AF930);
		DWORD dwValue = *(DWORD *)(g_pClient->GetBaseAddress() + 0x10F8088);
		DWORD * pdwValue = &dwValue;
		_asm push v7
		_asm mov ecx, pdwValue
		_asm call dwFunc*/
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10F8078) = v7;
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10F805C) = v7;

		/*if(v7 > 0)
		{
			dwFunc = (g_pClient->GetBaseAddress() + 0x7B2DF0);
			_asm call dwFunc
		}*/

		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 6;
		CLogFile::Printf("END");
	}
	/*if(!bHooked)
	{
		CPatcher::InstallJmpPatch((DWORD)(g_pClient->GetBaseAddress() + 0x4205B0), (DWORD)MyHook);
		bHooked = true;
	}*/
	BYTE byteSomeVal = *(BYTE *)(g_pClient->GetBaseAddress() + 0x18A818B);
	if(byteSomeVal != byteSomeValOld)
	{
		CLogFile::Printf("byte_18A818B changed (Old %d, New %d)", byteSomeValOld, byteSomeVal);
		byteSomeValOld = byteSomeVal;
	}
	/*if(!bLol)
	{
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 14;
		bLol = true;
	}
	if(!bLmao)
	{
		if(IsMenuShown())
		{
			*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 6;
			HideMenu();
			bLmao = true;
		}
	}*/
	/*if(bSet)
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 0;
	if(GetAsyncKeyState(VK_F2) & 1)
	{
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 6;
		bSet = false;
	}*/
	/*if(bHideMenu && IsMenuShown())
		HideMenu();
	if(*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) == 9)
		bHideMenu = true;
	else if(*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) == 6)
	{
		ShowMenu();
		bHideMenu = false;
	}*/
	/*if(!bHooked)
	{
		CPatcher::InstallJmpPatch((DWORD)(g_pClient->GetBaseAddress() + 0x7870A0), (DWORD)FuncHook);
		bHooked = true;
	}*/
	/*if(IsMainMenuShown())
	{
		HideMenu();
	}
	else
	{
		if(!bLoaded)
		{
			ShowMenu();
			bLoaded = true;
		}
	}*/
	DWORD dwVal3 = *(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80);
	if(dwVal3 != dwVal1)
	{
		CLogFile::Printf("dword_10C7F80 changed (Old %d, New %d)", dwVal1, dwVal3);
		dwVal1 = dwVal3;
	}
	BYTE byteVal4 = *(BYTE *)(g_pClient->GetBaseAddress() + 0x10C7F6F);
	if(byteVal4 != byteVal2)
	{
		CLogFile::Printf("byte_10C7F6F changed (Old %d, New %d)", byteVal2, byteVal4);
		byteVal2 = byteVal4;
	}

	/*if(*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) == 6 && bFlagged2)
	{
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 0;
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C7F6F) = 0;
		bFlagged2 = false;
	}
	if(*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) == 0 && bFlagged)
	{
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 6;
		bFlagged = false;
		bFlagged2 = true;
	}
	if(*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) == 9 && *(BYTE *)(g_pClient->GetBaseAddress() + 0x10C7F6F) == 1)
	{
		*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 0;
		bFlagged = true;
	}*/

	if(GetAsyncKeyState(VK_F1) & 1)
	{
		CLogFile::Printf("Changing menu shown value to 0");
		//*(DWORD *)(g_pClient->GetBaseAddress() + 0x10C7F80) = 0;
		*(BYTE *)(g_pClient->GetBaseAddress() + 0x10C7F6F) = 0;
	}
	// Does the chat window exist?
	if(m_pChatWindow)
	{
		// Draw it
		m_pChatWindow->Draw();
	}

	// TODO: Draw warning text if fps is low (togglable though)

	// Does the GUI root exist?
	if(m_pGUI)
	{
		// Render the GUI
		m_pGUI->Render();
	}
}

void CClient::OnGameLoad()
{
	// Create the streamer instance
	m_pStreamer = new CStreamer();

	CLogFile::Printf("streamer instance created");

	// Create the player manager instance
	m_pPlayerManager = new CClientPlayerManager();

	CLogFile::Printf("player manager instance created");

	// Create the vehicle manager instance
	m_pVehicleManager = new CClientVehicleManager();

	CLogFile::Printf("vehicle manager instance created");

	// Set the local player name
	m_pPlayerManager->GetLocalPlayer()->SetPlayerName(m_strNick);

	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Game loaded!");
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Starting network manager...");
	m_pNetworkManager->Startup(m_strIp, m_usPort, m_strPassword);
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Network manager started!");
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Establishing connection to %s:%d...", m_strIp.C_String(), m_usPort);
	m_pNetworkManager->Connect();

	// Fade out the game screen
	//m_pGame->FadeScreen(FADE_TYPE_OUT, 0);
}

bool bCreatePlayer = false;
#define NUM_PL 64
CClientPlayer * pPlayers[NUM_PL];
CClientVehicle * pVehicles[NUM_PL];
extern bool bShowShit;

/*
// 1, 2, 11, 14, 15 = analog
int CPadConfig__MapKey(CPadConfig * pPadConfig, int iState, int iKey, CPadData * pPadData, int iUnknown) // iUnknown = -1
{
	//(pPadConfig + pPadConfig->dwCount + 4 * 8);
	//(pPadConfig->dwCount + 4 * pPadConfig->dwCount + 8) = iState;
	*(DWORD *)(pPadConfig + 4 * *(DWORD *)pPadConfig + 8) = iState;
	//(pPadConfig->dwCount + 4 * pPadConfig->dwCount + 0x298) = iKey | (iUnknown << 24);
	*(DWORD *)(pPadConfig + 4 * *(DWORD *)pPadConfig + 0x298) = iKey | (iUnknown << 24);
	//(pPadConfig->dwCount + 4 * pPadConfig->dwCount + 0x528) = pPadData;
	*(DWORD *)(pPadConfig + 4 * *(DWORD *)pPadConfig + 0x528) = pPadData;
	return pPadConfig->dwCount++;
}

int CPadConfig__RemapKey(CPadConfig * pPadConfig, CPadData * pPadData)
{
	for(DWORD i = 0; i < pPadConfig->dwCount; i++)
	{
		if()
	}
	if ( a2 < *(_DWORD *)this )
	{
		if ( *((_DWORD *)this + a2 + 330) == a5 )
		{
			*((_DWORD *)this + a2 + 2) = a3;
			*((_DWORD *)this + a2 + 166) = a4 | (a6 << 24);
		}
	}

}
*/

bool bPrinted = false;

#define SYNC_DELAY 10000 // how many seconds between us and each mimic
//#define SYNC_DELAY 5000 // how many seconds between us and each mimic
//#define SYNC_DELAY 2000 // how many seconds between us and each mimic
//#define SYNC_DELAY 1000 // how many seconds between us and each mimic
//#define SYNC_DELAY 0 // how many seconds between us and each mimic
struct FootSync
{
	CVector3 vecPosition;
	float fRotation;
	CVector3 vecMoveSpeed;
	unsigned int uiHealth;
	CNetworkPadState netPadState;
	Matrix matCameraMatrix;
	unsigned int uiWeapon;
	unsigned int uiAmmo;
	bool bDucking;
};
struct FootSyncData
{
	int iPlayer;
	unsigned long ulApplyTime;
	FootSync footSync;

	bool operator== (const FootSyncData& o) const
	{
		return (iPlayer == o.iPlayer && ulApplyTime == o.ulApplyTime && !memcmp(&footSync, &o.footSync, sizeof(FootSync)));
	}
};
struct VehicleData
{
	CVector3 vecPosition;
	CVector3 vecRotation;
	float fHealth;
	CVector3 vecMoveSpeed;
	CVector3 vecTurnSpeed;
	CNetworkPadState netPadState;
	Matrix matCameraMatrix;
	unsigned int uiWeapon;
	unsigned int uiAmmo;
};
struct VehicleSyncData
{
	int iPlayer;
	unsigned long ulApplyTime;
	VehicleData vehicleSync;

	bool operator== (const VehicleSyncData& o) const
	{
		return (iPlayer == o.iPlayer && ulApplyTime == o.ulApplyTime && !memcmp(&vehicleSync, &o.vehicleSync, sizeof(VehicleData)));
	}
};
std::list<FootSyncData> m_onFootSync;
std::list<VehicleSyncData> m_vehicleSync;

// TODO: Perhaps use some IV CGame::Process hook for this?
void CClient::OnGameProcess()
{
	if(!bPrinted)
	{
		for(int i = 0; i < NUM_PL; i++)
		{
			pPlayers[i] = NULL;
			pVehicles[i] = NULL;
		}
		CLogFile::Printf("sizeof(IVEntity) == 0x%p", sizeof(IVEntity));
		CLogFile::Printf("sizeof(IVDynamicEntity) == 0x%p", sizeof(IVDynamicEntity));
		CLogFile::Printf("sizeof(IVPhysical) == 0x%p", sizeof(IVPhysical));
		CLogFile::Printf("sizeof(IVPed) == 0x%p", sizeof(IVPed));
		CLogFile::Printf("sizeof(IVPlayerPed) == 0x%p", sizeof(IVPlayerPed));
		CLogFile::Printf("sizeof(IVVehicle) == 0x%p", sizeof(IVVehicle));
		bPrinted = true;
	}
#if 0
	if(GetAsyncKeyState(VK_F4) & 1)
	{
		// Test to remap jump to sprint and vice versa
		CLogFile::Printf("Remap jump to sprint...");
		CPad * pPad = GetGamePad();
		CPadData * pControl = &pPad->m_padData[INPUT_JUMP];
		CPadData * pControl2 = &pPad->m_padData[INPUT_SPRINT];
		int iKey = 128; // Default Jump Key
		int iKey2 = 64; // Default Sprint Key
		//int iUnknown = -1;
		for(int i = 0; i < 5; i++)
		{
			CPadConfig * pPadConfig = &pPad->m_padConfig[i];

			for(int j = 0; j < pPadConfig->dwCount; j++)
			{
				if(*(CPadData **)&pPadConfig->CPadConfig_pad0[4 * j + 1316] == pControl)
				{
					CLogFile::Printf("")
					//*(_DWORD *)&this->field_4[4 * a2 + 4] = a3;
					/*DWORD dwVal = *(DWORD *)&pPadConfig->CPadConfig_pad0[4 * j + 660];
					int iUnknown = (dwVal >> 24);
					*(DWORD *)&pPadConfig->CPadConfig_pad0[4 * j + 660] = iKey2 | (iUnknown << 24);*/
					*(CPadData **)&pPadConfig->CPadConfig_pad0[4 * j + 1316] = pControl2;
					//break;
				}
				//test
				/*if(*(CPadData **)&pPadConfig->CPadConfig_pad0[4 * j + 1316] == pControl2)
				{
					//*(_DWORD *)&this->field_4[4 * a2 + 4] = a3;
					DWORD dwVal = *(DWORD *)&pPadConfig->CPadConfig_pad0[4 * j + 660];
					int iUnknown = (dwVal >> 24);
					*(DWORD *)&pPadConfig->CPadConfig_pad0[4 * j + 660] = iKey | (iUnknown << 24);
					//break;
				}*/
				//test end
			}
		}
		CLogFile::Printf("Done");
	}
#endif
	// Testing code
	if(pPlayers[1])
	//if(0)
	{
		CClientPlayer * pLocalPlayer = GetPlayerManager()->GetLocalPlayer();

		if(pLocalPlayer->IsOnFoot())
		{
			if(m_vehicleSync.size() > 0)
				m_vehicleSync.clear();

			for(int i = 1; i < NUM_PL; i++)
				SAFE_DELETE(pVehicles[i]);

			FootSync onFootSync;
			pLocalPlayer->GetPosition(onFootSync.vecPosition);
			onFootSync.fRotation = pLocalPlayer->GetCurrentHeading();
			pLocalPlayer->GetMoveSpeed(onFootSync.vecMoveSpeed);
			onFootSync.uiHealth = pLocalPlayer->GetHealth();
			pLocalPlayer->GetNetPadState(onFootSync.netPadState);
			//bShowShit = netPadState.IsAiming();
			/*if(netPadState.IsFiring())
			{
				IVPedTaskManager * pPedTaskManager = (IVPedTaskManager *)&(pLocalPlayer->GetPlayerPed()->GetPlayerPed()->m_pPedIntelligence->m_pPedTaskManager);
				for(int i = 0; i < TASK_PRIORITY_MAX; i++)
				{
					if(pPedTaskManager->m_tasks[i])
					{
						CIVTask * pTask = new CIVTask(pPedTaskManager->m_tasks[i]);
						GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Priority Task %d = %d", i, pTask->GetType());
						delete pTask;
					}
				}
				for(int i = 0; i < TASK_SECONDARY_MAX; i++)
				{
					if(pPedTaskManager->m_secondaryTasks[i])
					{
						CIVTask * pTask = new CIVTask(pPedTaskManager->m_secondaryTasks[i]);
						GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Secondary Task %d = %d", i, pTask->GetType());
						delete pTask;
					}
				}
			}*/
			pLocalPlayer->GetAimData(onFootSync.matCameraMatrix);
			onFootSync.uiWeapon = pLocalPlayer->GetCurrentWeaponType();
			onFootSync.uiAmmo = pLocalPlayer->GetWeaponAmmo(onFootSync.uiWeapon);
			onFootSync.bDucking = pLocalPlayer->IsDucking();
			FootSyncData footSyncData;
			memcpy(&footSyncData.footSync, &onFootSync, sizeof(FootSync));

			for(int i = 1; i < NUM_PL; i++)
			{
				if(pPlayers[i])
				{
					footSyncData.iPlayer = i;
					footSyncData.ulApplyTime = (SharedUtility::GetTime() + (i * SYNC_DELAY));
					m_onFootSync.push_back(footSyncData);
				}
			}

top:
			for(std::list<FootSyncData>::iterator iter = m_onFootSync.begin(); iter != m_onFootSync.end(); iter++)
			{
				if(SharedUtility::GetTime() >= (*iter).ulApplyTime)
				{
					CClientPlayer * pPlayer = pPlayers[(*iter).iPlayer];
					(*iter).footSync.vecPosition.fX += (8.0f * (*iter).iPlayer);
					pPlayer->SetPosition((*iter).footSync.vecPosition);
					pPlayer->SetCurrentHeading((*iter).footSync.fRotation);
					pPlayer->SetMoveSpeed((*iter).footSync.vecMoveSpeed);
					pPlayer->SetHealth((*iter).footSync.uiHealth);
					pPlayer->SetNetPadState((*iter).footSync.netPadState);
					(*iter).footSync.matCameraMatrix.vecPosition.fX += (8.0f * (*iter).iPlayer);
					pPlayer->SetAimData((*iter).footSync.matCameraMatrix);
					pPlayer->GiveWeapon((*iter).footSync.uiWeapon, (*iter).footSync.uiAmmo);
					pPlayer->SetCurrentWeapon((*iter).footSync.uiWeapon);
					pPlayer->SetDucking((*iter).footSync.bDucking);
					m_onFootSync.remove((*iter));
					goto top;
				}
			}
		}
		else
		{
			if(m_onFootSync.size() > 0)
				m_onFootSync.clear();

			VehicleData vehicleSync;
			CClientVehicle * pVehicle = pLocalPlayer->GetVehicle();
			pVehicle->GetPosition(vehicleSync.vecPosition);

			for(int i = 1; i < NUM_PL; i++)
			{
				if(pPlayers[i])
				{
					if(!pVehicles[i])
					{
						CLogFile::Printf("Creating vehicle for clone ped...");

						// Create vehicle instance
						pVehicles[i] = new CClientVehicle(pVehicle->GetModelInfo()->GetIndex());

						// Set the vehicle as can be streamed in
						pVehicles[i]->SetCanBeStreamedIn(true);

						pVehicles[i]->Teleport(vehicleSync.vecPosition);
						CLogFile::Printf("Done!");
					}

					// Put the player in the vehicle
					pPlayers[i]->PutInVehicle(pVehicles[i], 0);
				}
			}

			pVehicle->GetRotation(vehicleSync.vecRotation);
			vehicleSync.fHealth = pVehicle->GetHealth();
			pVehicle->GetMoveSpeed(vehicleSync.vecMoveSpeed);
			pVehicle->GetTurnSpeed(vehicleSync.vecTurnSpeed);
			pLocalPlayer->GetNetPadState(vehicleSync.netPadState);
			pLocalPlayer->GetAimData(vehicleSync.matCameraMatrix);
			vehicleSync.uiWeapon = pLocalPlayer->GetCurrentWeaponType();
			vehicleSync.uiAmmo = pLocalPlayer->GetWeaponAmmo(vehicleSync.uiWeapon);
			VehicleSyncData vehicleSyncData;
			memcpy(&vehicleSyncData.vehicleSync, &vehicleSync, sizeof(VehicleData));

			for(int i = 1; i < NUM_PL; i++)
			{
				if(pPlayers[i])
				{
					vehicleSyncData.iPlayer = i;
					vehicleSyncData.ulApplyTime = (SharedUtility::GetTime() + (i * SYNC_DELAY));
					m_vehicleSync.push_back(vehicleSyncData);
				}
			}

vtop:
			for(std::list<VehicleSyncData>::iterator iter = m_vehicleSync.begin(); iter != m_vehicleSync.end(); iter++)
			{
				if(SharedUtility::GetTime() >= (*iter).ulApplyTime)
				{
					(*iter).vehicleSync.vecPosition.fX += (8.0f * (*iter).iPlayer);
					pVehicles[(*iter).iPlayer]->SetPosition((*iter).vehicleSync.vecPosition);
					pVehicles[(*iter).iPlayer]->SetRotation((*iter).vehicleSync.vecRotation);
					pVehicles[(*iter).iPlayer]->SetHealth((*iter).vehicleSync.fHealth);
					pVehicles[(*iter).iPlayer]->SetMoveSpeed((*iter).vehicleSync.vecMoveSpeed);
					pVehicles[(*iter).iPlayer]->SetTurnSpeed((*iter).vehicleSync.vecTurnSpeed);
					pPlayers[(*iter).iPlayer]->SetNetPadState((*iter).vehicleSync.netPadState);
					(*iter).vehicleSync.matCameraMatrix.vecPosition.fX += (8.0f * (*iter).iPlayer);
					pPlayers[(*iter).iPlayer]->SetAimData((*iter).vehicleSync.matCameraMatrix);
					pPlayers[(*iter).iPlayer]->GiveWeapon((*iter).vehicleSync.uiWeapon, (*iter).vehicleSync.uiAmmo);
					pPlayers[(*iter).iPlayer]->SetCurrentWeapon((*iter).vehicleSync.uiWeapon);
					m_vehicleSync.remove((*iter));
					goto vtop;
				}
			}
		}
	}
	if(GetAsyncKeyState(VK_F3) & 1)
	{
		if(!bCreatePlayer)
		{
			for(BYTE i = 1; i < 2; i++)
			{
				CLogFile::Printf("Creating new player...");
				pPlayers[i] = new CClientPlayer(false);
				pPlayers[i]->SetCanBeStreamedIn(true);
				pPlayers[i]->Teleport(CVector3(-341.36f + i, 1144.80f, 14.79f));
				pPlayers[i]->SetCurrentHeading(40.114815f);
				pVehicles[i] = NULL;
				CLogFile::Printf("Done!");
			}
		}
		else
		{
			for(BYTE i = 1; i < 2; i++)
			{
				CLogFile::Printf("Deleting new player...");
				SAFE_DELETE(pVehicles[i]);
				SAFE_DELETE(pPlayers[i]);
				CLogFile::Printf("Done!");
			}
		}

		bCreatePlayer = !bCreatePlayer;
	}
	else if(GetAsyncKeyState(VK_F7) & 1)
	{
		CClientVehicle * pVehicle = m_pPlayerManager->GetLocalPlayer()->GetVehicle();

		if(pVehicle)
		{
			CLogFile::Printf("Turning on vehicle interior lights and hazard lights");
			IVVehicle * pIVVehicle = pVehicle->GetGameVehicle()->GetVehicle();
			CLogFile::Printf("Before turn on is %d", pIVVehicle->m_byteUnknownFlags1);
			bool bHazardLights = true;
			bool bInteriorLight = true;

			if(bHazardLights)
				SET_BIT(pIVVehicle->m_byteUnknownFlags1, 2);
			else
				UNSET_BIT(pIVVehicle->m_byteUnknownFlags1, 2);

			if(bInteriorLight)
				SET_BIT(pIVVehicle->m_byteUnknownFlags1, 0x10);
			else
				UNSET_BIT(pIVVehicle->m_byteUnknownFlags1, 0x10);
			CLogFile::Printf("After turn on is %d", pIVVehicle->m_byteUnknownFlags1);

			//*(BYTE *)(pIVVehicle->GetVehicle() + 0xF6C) ^= (*(BYTE *)(pIVVehicle->GetVehicle() + 0xF6C) ^ 2 * cHazardLights) & 2;
			//*(BYTE *)(pIVVehicle->GetVehicle() + 0xF6C) ^= (*(BYTE *)(pIVVehicle->GetVehicle() + 0xF6C) ^ 0x10 * cInteriorLight) & 0x10;
		}
	}

	// Does the network manager exist?
	if(m_pNetworkManager)
	{
		// Process it
		m_pNetworkManager->Process();
	}

	// Does the resource manager exist?
	if(g_pResourceManager)
	{
		// Process it
		g_pResourceManager->Process(SharedUtility::GetTime());
	}
}

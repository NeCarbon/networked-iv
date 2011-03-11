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

	// TODO: CCommandLine class
	// TODO: CCommandLine::Parse
	// TODO: CCommandLine::GetOption
	// Process the command line parameters
	char * szCommandLine = GetCommandLine();

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

				// Handle the option and value string
				if(!strcmp(strOption.C_String(), "-ip"))
					m_strIp.Set(strValue.C_String());
				else if(!strcmp(strOption.C_String(), "-port"))
					m_usPort = atoi(strValue.C_String());
				else if(!strcmp(strOption.C_String(), "-password"))
					m_strPassword.Set(strValue.C_String());
				else if(!strcmp(strOption.C_String(), "-nick"))
					m_strNick.Set(strValue.C_String());
				else
				{
					CLogFile::Printf("Warning: Unhandled command line parameter %s (%s)\n", 
						strOption.C_String(), strValue.C_String());
				}

				// If we have run out of command line to process break out of the loop
				if(!(*szCommandLine))
					break;
			}
		}

		// Increment the command line string pointer
		szCommandLine++;
	}

	// Do we not have an ip or nick?
	if(m_strIp.IsEmpty() || m_strNick.IsEmpty())
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
	CLogFile::Printf("base address is 0x%p\n", m_uiBaseAddress);

	// Create the game instance
	m_pGame = new CGame();

	// If the instance creation failed, exit
	if(!m_pGame)
		return false;

	CLogFile::Printf("game instance created\n");

	// Apply the game patches
	m_pGame->ApplyPatches();

	CLogFile::Printf("game patches applied\n");

	// Create the xlive hook instance
	m_pXLiveHook = new CXLiveHook();

	// If the instance creation failed, exit
	if(!m_pXLiveHook)
		return false;

	CLogFile::Printf("xlive hook instance created\n");

	// Install the xlive hook
	m_pXLiveHook->Install();

	CLogFile::Printf("xlive hook installed\n");

	// Create the d3d9 hook instance
	m_pDirect3D9Hook = new CDirect3D9Hook();

	// If the instance creation failed, exit
	if(!m_pDirect3D9Hook)
		return false;

	CLogFile::Printf("d3d9 instance created\n");

	// Install the d3d9 hook
	m_pDirect3D9Hook->Install();

	CLogFile::Printf("d3d9 hook installed\n");

	// Initialize the net module, if it fails, exit
	if(!CNetModule::Init())
	{
		CLogFile::Printf("Failed to initialize the net module!\n");
		return false;
	}

	CLogFile::Printf("net module initialized\n");

	// Create the network manager instance
	m_pNetworkManager = new CClientNetworkManager();

	CLogFile::Printf("network manager instance created\n");

	// Create the client task manager instance
	m_pClientTaskManager = new CClientTaskManager();

	CLogFile::Printf("client task manager instance created\n");

	// Success!
	return true;
}

void CClient::OnUnload()
{
	// Delete the chat window instance
	SAFE_DELETE(m_pChatWindow);

	// Delete the font instance
	SAFE_DELETE(m_pFont);

	// Delete the streamer instance
	SAFE_DELETE(m_pStreamer);

	// Delete the player manager instance
	SAFE_DELETE(m_pPlayerManager);

	// Delete the client task manager instance
	SAFE_DELETE(m_pClientTaskManager);

	// Delete the network manager instance
	SAFE_DELETE(m_pNetworkManager);

	// Shutdown the net module
	CNetModule::Shutdown();

	// Uninstall the d3d9 hook
	if(m_pDirect3D9Hook)
		m_pDirect3D9Hook->Uninstall();

	// Delete the d3d9 hook instance
	SAFE_DELETE(m_pDirect3D9Hook);

	// Uninstall the xlive hook
	m_pXLiveHook->Uninstall();

	// Delete the xlive hook instance
	SAFE_DELETE(m_pXLiveHook);

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
	CLogFile::Printf("Client information: \n");

	// Write the local player index
	CLogFile::Printf("LPI: %d\n", CPools::GetLocalPlayerIndex());
}

void CClient::OnD3DCreateDevice(IDirect3DDevice9 * pD3DDevice)
{
	CLogFile::Printf("d3d create\n");
	// Create our font instance
	m_pFont = new CD3DXFont(pD3DDevice, 14, 0, FW_BOLD, "Tahoma");

	// Create out chat window instance
	m_pChatWindow = new CChatWindow(m_pFont);

	// Set the chat window to log messages
	m_pChatWindow->SetLogMessages(true);

	// Testing code
	m_pChatWindow->Enable();
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Chat window initialized!");
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

void CClient::OnD3DResetDevice(IDirect3DDevice9 * pD3DDevice)
{
	// Does the font exist?
	if(m_pFont)
	{
		// Inform it of the device reset
		m_pFont->OnDeviceReset();
	}
}

void CClient::OnD3DBeginScene(IDirect3DDevice9 * pD3DDevice)
{
	
}

void CClient::OnD3DEndScene(IDirect3DDevice9 * pD3DDevice)
{
	// Does the chat window exist?
	if(m_pChatWindow)
	{
		// Draw it
		m_pChatWindow->Draw();
	}

	// TODO: Draw warning text if fps is low (togglable though)
}

void CClient::OnGameLoad()
{
	// Create the streamer instance
	m_pStreamer = new CStreamer();

	CLogFile::Printf("streamer instance created\n");

	// Create the player manager instance
	m_pPlayerManager = new CClientPlayerManager();

	CLogFile::Printf("player manager instance created\n");

	// Set the local player name
	m_pPlayerManager->GetLocalPlayer()->SetPlayerName(m_strNick);

	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Game loaded!");
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Starting network manager...");
	m_pNetworkManager->Startup(m_strIp, m_usPort, m_strPassword);
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Network manager started!");
	m_pChatWindow->OutputMessage(MESSAGE_INFO_COLOR, "Establishing connection to %s:%d...", m_strIp.C_String(), m_usPort);
	m_pNetworkManager->Connect();

	// Fade out the game screen
	m_pGame->FadeScreen(FADE_TYPE_OUT, 0);
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

//#define SYNC_DELAY 5000 // how many seconds between us and each mimic
#define SYNC_DELAY 1000 // how many seconds between us and each mimic
struct FootSync
{
	Vector3 vecPosition;
	Vector3 vecRotation;
	Vector3 vecMoveSpeed;
	Vector3 vecTurnSpeed;
	unsigned int uiHealth;
	CNetworkPadState netPadState;
	Matrix matCameraMatrix;
	unsigned int uiWeapon;
	unsigned int uiAmmo;
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
	Vector3 vecPosition;
	Vector3 vecRotation;
	float fHealth;
	Vector3 vecMoveSpeed;
	Vector3 vecTurnSpeed;
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
		CLogFile::Printf("sizeof(IVEntity) == %d\n", sizeof(IVEntity));
		CLogFile::Printf("sizeof(IVDynamicEntity) == %d\n", sizeof(IVDynamicEntity));
		CLogFile::Printf("sizeof(IVPhysical) == %d\n", sizeof(IVPhysical));
		CLogFile::Printf("sizeof(IVPed) == %d\n", sizeof(IVPed));
		CLogFile::Printf("sizeof(IVPlayerPed) == %d\n", sizeof(IVPlayerPed));
		CLogFile::Printf("sizeof(IVVehicle) == %d\n", sizeof(IVVehicle));
		IVPad * pPad = GetGamePad();
		for(int i = 0; i < INPUT_MAX; i++)
		{
			if(pPad->m_padData[i].m_pUnknown)
			{
				CLogFile::Printf("Oh noez, Input %d has extra dataz!\n", i);
			}
		}
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
		CLogFile::Printf("Done\n");
	}
#endif
	// Testing code
	if(pPlayers[1])
	{
		CClientPlayer * pLocalPlayer = GetPlayerManager()->GetLocalPlayer();

		if(!pLocalPlayer->IsInVehicle())
		{
			if(m_vehicleSync.size() > 0)
				m_vehicleSync.clear();

			for(int i = 1; i < NUM_PL; i++)
				SAFE_DELETE(pVehicles[i]);

			FootSync onFootSync;
			pLocalPlayer->GetPosition(onFootSync.vecPosition);
			pLocalPlayer->GetRotation(onFootSync.vecRotation);
			pLocalPlayer->GetMoveSpeed(onFootSync.vecMoveSpeed);
			pLocalPlayer->GetTurnSpeed(onFootSync.vecTurnSpeed);
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
					(*iter).footSync.vecPosition.fX += (8.0f * (*iter).iPlayer);
					pPlayers[(*iter).iPlayer]->SetPosition((*iter).footSync.vecPosition);
					pPlayers[(*iter).iPlayer]->SetRotation((*iter).footSync.vecRotation);
					pPlayers[(*iter).iPlayer]->SetMoveSpeed((*iter).footSync.vecMoveSpeed);
					pPlayers[(*iter).iPlayer]->SetTurnSpeed((*iter).footSync.vecTurnSpeed);
					pPlayers[(*iter).iPlayer]->SetHealth((*iter).footSync.uiHealth);
					pPlayers[(*iter).iPlayer]->SetNetPadState((*iter).footSync.netPadState);
					(*iter).footSync.matCameraMatrix.vecPosition.fX += (8.0f * (*iter).iPlayer);
					pPlayers[(*iter).iPlayer]->SetAimData((*iter).footSync.matCameraMatrix);
					pPlayers[(*iter).iPlayer]->GiveWeapon((*iter).footSync.uiWeapon, (*iter).footSync.uiAmmo);
					pPlayers[(*iter).iPlayer]->SetCurrentWeapon((*iter).footSync.uiWeapon);
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
						pVehicles[i] = new CClientVehicle(pVehicle->GetModelInfo()->GetHash());

						// Set the vehicle as can be streamed in
						pVehicles[i]->SetCanBeStreamedIn(true);

						pVehicles[i]->Teleport(vehicleSync.vecPosition);
						CLogFile::Printf("Done!\n");
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
				pPlayers[i]->Teleport(Vector3(-341.36f + i, 1144.80f, 14.79f));
				pPlayers[i]->SetRotation(Vector3(0, 0, 40.114815f));
				pVehicles[i] = NULL;
				CLogFile::Printf("Done!\n");
			}
		}
		else
		{
			for(BYTE i = 1; i < 2; i++)
			{
				CLogFile::Printf("Deleting new player...");
				SAFE_DELETE(pVehicles[i]);
				SAFE_DELETE(pPlayers[i]);
				CLogFile::Printf("Done!\n");
			}
		}

		bCreatePlayer = !bCreatePlayer;
	}

	// Does the network manager exist?
	if(m_pNetworkManager)
	{
		// Process it
		m_pNetworkManager->Process();
	}
}

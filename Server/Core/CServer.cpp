//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServer.cpp
// Project: Server
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#define SERVER_TITLE MOD_NAME " v" MOD_VERSION_STRING " Server [" MOD_OS_STRING "]"

CServer          * g_pServer = NULL;
CConfig          * g_pConfig = NULL;
CResourceManager * g_pResourceManager = NULL;
CRootEntity      * g_pRootEntity = NULL;

/*String DumpDataAsHex(char * szData, unsigned int uiLength)
{
	String strHex;

	for(unsigned int i = 0; i < uiLength; i++)
	{
		if(i != 0)
			strHex.Append(", ");

		strHex.AppendF("%02X", szData[i]);
	}

	return strHex;
}*/

CServer::CServer()
{
	//ADD_EXPLOSION
	// 4 hash = 0x10AF5258
	// 5 hash = 0x32DA5E3A
	//DoVirtualFileSystemTest();
	m_bActive = true;
	m_bShowFPS = true;
	m_ulLastFPSUpdateTime = 0;
	m_ulFrameCount = 0;
	m_ulFramesPerSecond = 0;
}

CServer::~CServer()
{

}

#ifdef NIV_DEBUG
void MemoryTest()
{
	FILE * fp = fopen("MemoryTest.txt", "w");

	if(fp)
	{
		MemTrack::TrackDumpBlocks(fp);
		MemTrack::TrackListMemoryUsage(fp);
		fclose(fp);
	}
}
#endif

bool CServer::OnLoad()
{
	// If in debug mode set memory test function
#ifdef NIV_DEBUG
	atexit(MemoryTest);
#endif

	// Open the log file
	CLogFile::Open("Server.log", true);

	// Install our exception handler
	CExceptionHandler::Install();

	CLogFile::Printf("Starting up " SERVER_TITLE "...\n");

	// Create the config instance
	g_pConfig = new CConfig();

	if(!g_pConfig)
	{
		CLogFile::Printf("Failed to create config instance!\n");
		return false;
	}

	CLogFile::Printf("Config instance created\n");

	// Open the config file
	if(!g_pConfig->Open(String("%sServer.conf", SharedUtility::GetAppPath())))
		CLogFile::Printf("Failed to open config file! Settings will default to appropriate values.\n");
	else
		CLogFile::Printf("Config file opened\n");

	// Initialize the net module
	if(!CNetworkModule::Init())
	{
		CLogFile::Printf("Failed to initialize the net module!\n");
		return false;
	}

	CLogFile::Printf("Net module initialized\n");

	// Create the network manager instance
	m_pNetworkManager = new CNetworkManager();

	if(!m_pNetworkManager)
	{
		CLogFile::Printf("Failed to create network manager instance!\n");
		return false;
	}

	CLogFile::Printf("Network manager instance created\n");

	// Create the player manager instance
	m_pPlayerManager = new CPlayerManager();

	if(!GetPlayerManager())
	{
		CLogFile::Printf("Failed to create player manager instance!\n");
	}

	CLogFile::Printf("Player manager instance created\n");

	// Create the vehicle manager instance
	m_pVehicleManager = new CVehicleManager();

	if(!m_pVehicleManager)
	{
		CLogFile::Printf("Failed to create vehicle manager instance!\n");
	}

	CLogFile::Printf("Vehicle manager instance created\n");

	// Get server port
	int iServerPort = GetConfigInteger("port", 9999);

	// Get show fps
	m_bShowFPS = GetConfigBoolean("showfps", true);

	// Start up the network manager
	m_pNetworkManager->Startup(iServerPort, PLAYER_MAX);

	CLogFile::Printf("Network manager started up\n");

	// Create the resource and scripting manager
	CEntityIDs::Initalize();
	m_pRootEntity = new CRootEntity();
	m_pResourceManager = new CResourceManager("resources/");

	// Load resources, get the first resource node
	if(g_pConfig->GetXML()->findNode("resource"))
	{
		while(true)
		{
			// Get the resource name
			String strResource = g_pConfig->GetXML()->nodeContent();

			// Make sure the name is valid and attempt to load the resource
			if(strResource && !strResource.IsEmpty())
				m_pResourceManager->Load(strResource);

			// Attempt to load the next resource node (if any)
			if(!g_pConfig->GetXML()->nextNode())
				break;
		}
	}
	/*TiXmlDocument pDocument("Server.conf");
	if(pDocument.LoadFile())
	{
		TiXmlNode* pNode = pDocument.RootElement()->FirstChild("resource");
		while(pNode)
		{
			String strResource = pNode->ToElement()->GetText();
			
			pNode = pNode->NextSibling("resource");
		}
	}*/

	// Create the server lister instance
	m_pServerLister = new CServerLister(iServerPort);

	// Set the server title
#ifdef WIN32
	SetTitle(SERVER_TITLE);
#endif

	CLogFile::Printf("Server started on port %d\n", iServerPort);

	// Temporary code
	CVehicle * pVehicle = m_pVehicleManager->Add(174);
	pVehicle->SetPosition(CVector3(-341.36f, 1144.80f, 14.79f));
	pVehicle->SetRotation(CVector3(0.0f, 0.0f, 40.114815f));
	pVehicle->SpawnForWorld();
	return true;
}

void CServer::Process()
{
	// Get the current time
	unsigned long ulTime = SharedUtility::GetTime();

	// Is show fps enabled?
	if(m_bShowFPS)
	{
		// Get the amount of time elapsed since the last fps update
		unsigned long ulTimeElapsed = (ulTime - m_ulLastFPSUpdateTime);

		// Update the fps if a second or more has passed
		if(ulTimeElapsed >= 1000)
		{
			// Set the fps
			m_ulFramesPerSecond = m_ulFrameCount;

			// Set the server title
#ifdef WIN32
			String strTitle(SERVER_TITLE " - FPS: %d", m_ulFramesPerSecond);
			SetTitle(strTitle.C_String());
#endif

			// Reset the frame count
			m_ulFrameCount = 0;

			// Set the last fps update tick count
			m_ulLastFPSUpdateTime = ulTime;
		}

		// Increment the frame count
		m_ulFrameCount++;
	}

	// Process the network manager
	m_pNetworkManager->Process();

	// Process the resource manager
	m_pResourceManager->Process(ulTime);

	// Process the server lister
	m_pServerLister->Process();

	// Process the input queue
	ProcessInputQueue();

	/*if(GetAsyncKeyState(VK_F1) & 1)
	{
		MemoryInfo memoryInfo;
		GetMemoryInformation(memoryInfo);
#define BTOMB(bytes) (((bytes) / 1024) / 1024)
		printf("Total memory %dmb, avaiable memory %dmb, used memory %d, process used memory %dmb\n", BTOMB(memoryInfo.ulPhysicalTotalMemory), BTOMB(memoryInfo.ulPhysicalAvaliableMemory), BTOMB(memoryInfo.ulPhysicalUsedMemory), BTOMB(memoryInfo.ulProcessUsedMemory));
	}*/
}

void CServer::OnUnload()
{
	CLogFile::Printf("Server exiting...\n");

	// Delete the server lister instance
	SAFE_DELETE(m_pServerLister);

	// Delete the vehicle manager instance
	SAFE_DELETE(m_pVehicleManager);

	// Delete the player manager instance
	SAFE_DELETE(m_pPlayerManager);

	// Delete the resource manager
	SAFE_DELETE(m_pResourceManager);

	// Delete the scripting manager
	SAFE_DELETE(m_pRootEntity);

	// Delete the network manager instance
	SAFE_DELETE(m_pNetworkManager);

	// Shutdown the net module
	CNetworkModule::Shutdown();

	// Delete the config instance
	SAFE_DELETE(g_pConfig);

	CLogFile::Printf("Server exited\n");

	// Close the log file
	CLogFile::Close();
}

void CServer::SetActive(bool bActive)
{
	m_bActive = bActive;
}

bool CServer::IsActive()
{
	return m_bActive;
}

void CServer::AddToInputQueue(String strString)
{
	m_inputQueueMutex.Lock();
	m_inputQueue.push(strString);
	m_inputQueueMutex.Unlock();
}

void CServer::SetTitle(String strTitle)
{
#ifdef WIN32
	SetConsoleTitle(strTitle);
#endif
}

void CServer::ProcessInputQueue()
{
	if(m_inputQueueMutex.TryLock(0))
	{
		while(!m_inputQueue.empty())
		{
			// Get the input string
			String strInput = m_inputQueue.front();

			// Erase the input string from the input queue
			m_inputQueue.pop();

			// Process the input string
			// NOTE: HARDCODED FOR NOW
			// TODO: CServerCommandHandler
			if(!strInput.ICompare("shutdown") || !strInput.ICompare("exit") || !strInput.ICompare("quit"))
			{
				// Shutdown command, set the active flag to false and don't process any more input
				m_bActive = false;
				break;
			}
		}

		m_inputQueueMutex.Unlock();
	}
}

String CServer::GetConfigString(String strKey, String strDefaultValue)
{
	String strValue;

	if(!(g_pConfig && g_pConfig->GetValueAsString(strKey, strDefaultValue, &strValue)))
	{
		CLogFile::Printf("Failed to get '%s' value from config, defaulting to '%s'\n", strKey.C_String(), strDefaultValue.C_String());
	}

	return strValue;
}

int CServer::GetConfigInteger(String strKey, int iDefaultValue)
{
	int iValue = 0;

	if(!(g_pConfig && g_pConfig->GetValueAsInteger(strKey, iDefaultValue, &iValue)))
	{
		CLogFile::Printf("Failed to get '%s' value from config, defaulting to '%d'\n", strKey.C_String(), iDefaultValue);
	}

	return iValue;
}

float CServer::GetConfigFloat(String strKey, float fDefaultValue)
{
	float fValue = 0.0f;

	if(!(g_pConfig && g_pConfig->GetValueAsFloat(strKey, fDefaultValue, &fValue)))
	{
		CLogFile::Printf("Failed to get '%s' value from config, defaulting to '%f'\n", strKey.C_String(), fDefaultValue);
	}

	return fValue;
}

bool CServer::GetConfigBoolean(String strKey, bool bDefaultValue)
{
	bool bValue = false;

	if(!(g_pConfig && g_pConfig->GetValueAsBoolean(strKey, bDefaultValue, &bValue)))
	{
		CLogFile::Printf("Failed to get '%s' value from config, defaulting to '%s'\n", strKey.C_String(), (bDefaultValue ? "true" : "false"));
	}

	return bValue;
}

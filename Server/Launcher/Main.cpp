//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: Main.cpp
// Project: Server.Launcher
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#define SERVER_CORE_NAME "Server.Core"
#define TIMING_VALUE 10 // ms

CLibrary * g_pServerLibrary = NULL;
CServerInterface * g_pServer = NULL;

#ifdef WIN32
BOOL WINAPI CtrlHandler(DWORD dwType)
{
	switch(dwType)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		// Flag the server core as inactive
		g_pServer->SetActive(false);
		return TRUE;
	}

	return FALSE;
}
#else
void SignalHandler(int iType)
{
	// NOTE: I don't think param 1 is the signal type
	switch(iType)
	{
	case SIGINT:
	case SIGTERM:
		// Flag the server core as inactive
		g_pServer->SetActive(false);
	}
}
#endif

void InputThread(CThread * pCreator)
{
	char szInputBuffer[512]; // Buffer MUST be 3 or more (input char, \n, \0)
	String strInputString;

	// Loop until server shutdown
	while(pCreator->GetUserData<bool>())
	{
		// Wait for input from the console
		// TODO: Make non-blocking
		fgets(szInputBuffer, sizeof(szInputBuffer), stdin);

		// Do we have anything in the input?
		if(szInputBuffer[0] != '\n')
		{
			// Append the input to the input string
			strInputString += szInputBuffer;

			// Get the input string length
			size_t sLength = strInputString.GetLength();

			// Is this the last of the input?
			if(strInputString[sLength - 1] == '\n')
			{
#ifdef WIN32
				// Replace '\n' with '\0'
				strInputString.Truncate(sLength - 1);
#else
				// Replace '\r\n' with '\0'
				strInputString.Truncate(sLength - 2);
#endif

				// Add the input string to the server core console input queue
				g_pServer->AddToInputQueue(strInputString);

				// Clear the input string
				strInputString.Clear();
			}
		}

		// Wait
		Sleep(TIMING_VALUE);
	}
}

void WaitForKeyPress()
{
	printf("Press any key to exit...\n");
	getc(stdin);
}

int main()
{
	// Initialize timing
	SharedUtility::TimeInit();

	// Create the library instance
	g_pServerLibrary = new CLibrary();

	// Get the server core path
	String strPath("%s" SERVER_CORE_NAME DEBUG_SUFFIX LIBRARY_EXTENSION, SharedUtility::GetAppPath());

	// Load the server library
	if(!g_pServerLibrary->Load(strPath.C_String()))
	{
		printf("Failed to load server core!\n");
		WaitForKeyPress();
		return 1;
	}

	// Get the server core exports
	typedef CServerInterface * (* GetServerInterface_t)();
	typedef void (* DestroyServerInterface_t)(CServerInterface * pServer);
	GetServerInterface_t pfnGetServerInterface = 
		(GetServerInterface_t)g_pServerLibrary->GetProcedureAddress("GetServerInterface");
	DestroyServerInterface_t pfnDestroyServerInterface = 
		(DestroyServerInterface_t)g_pServerLibrary->GetProcedureAddress("DestroyServerInterface");

	// Create the server core instance
	g_pServer = pfnGetServerInterface();

	// Is the server core instance invalid?
	if(!g_pServer)
	{
		// Server instance invalid, exit
		printf("Server instance is invalid!\n");
		WaitForKeyPress();
		return 1;
	}

	// Call the server core OnLoad event
	if(!g_pServer->OnLoad())
	{
		// Server load failed, exit
		WaitForKeyPress();
		return 1;
	}

	// Register the close event handler
#ifdef WIN32
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
#else
	void (* prev_fn)(int);
	prev_fn = signal(SIGINT, SignalHandler);
	if(prev_fn == SIG_IGN) signal(SIGINT, SIG_IGN);
	prev_fn = signal(SIGTERM, SignalHandler);
	if(prev_fn == SIG_IGN) signal(SIGTERM, SIG_IGN);
#endif

	// Start the input thread
	CThread inputThread;
	inputThread.SetUserData<bool>(true);
	inputThread.Start(InputThread);

	// Loop until the server IsActive returns false
	while(g_pServer->IsActive())
	{
		// Call the server core Process event
		g_pServer->Process();

		// Wait
		Sleep(TIMING_VALUE);
	}

	// Stop the input thread
	inputThread.SetUserData<bool>(false);
	inputThread.Stop(false);

	// Call the server core OnUnload event
	g_pServer->OnUnload();

	// Destroy the server instance
	pfnDestroyServerInterface(g_pServer);

	// Shutdown timing
	SharedUtility::TimeShutdown();
	return 0;
}

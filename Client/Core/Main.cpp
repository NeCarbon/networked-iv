//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: Main.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CClient * g_pClient = NULL;

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, void *)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			// Disable thread calls
			DisableThreadLibraryCalls(hModule);

			// Create the client instance
			g_pClient = new CClient();

			// If the client instance was created call the client OnLoad function, if
			// it wasn't, exit
			if(!g_pClient || !g_pClient->OnLoad())
			{
				// Something bad happened, exit
				ExitProcess(0);
			}
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			// Call the client OnUnload
			g_pClient->OnUnload();

			// Delete the client instance
			SAFE_DELETE(g_pClient);
		}
		break;
	}

	return TRUE;
}

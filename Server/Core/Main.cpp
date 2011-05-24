//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: Main.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CServer * g_pServer;

EXPORT CServerInterface * GetServerInterface()
{
	g_pServer = new CServer();
	return g_pServer;
}

EXPORT void DestroyServerInterface(CServerInterface * pServer)
{
	delete pServer;
}

#ifdef WIN32
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, void *)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			// Disable thread calls
			DisableThreadLibraryCalls(hModule);
		}
		break;
	}

	return TRUE;
}
#endif

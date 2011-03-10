//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: Main.cpp
// Project: LauncherLibrary
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

typedef BOOL (WINAPI * CreateProcessW_t)(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);

CreateProcessW_t g_pfnCreateProcessW;

int ShowMessageBox(char * szText, UINT uType = (MB_ICONEXCLAMATION | MB_OK))
{
	return MessageBox(NULL, szText, MOD_NAME, uType);
}

BOOL WINAPI CreateProcessW_Hook(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	// CreateProcessW(GTAIV.exe, "\"path/to/exe.exe\"", "path/to/exe.exe", CREATE_NEW_PROCESS_GROUP)
	// The only call to CreateProcessW from LaunchGTAIV.exe should be to GTAIV.exe
	// however i should add an application name check to be safe

	// Set the CREATE_SUSPENDED flag in the creation flags
	dwCreationFlags |= CREATE_SUSPENDED;

	// Get the GTA IV install directory from the registry
	char szInstallDirectory[MAX_PATH];
	bool bFoundCustomDirectory = false;

	if(!SharedUtility::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\Rockstar Games\\Grand Theft Auto IV", 
										  "InstallFolder", NULL, szInstallDirectory, sizeof(szInstallDirectory)) || 
	   !SharedUtility::Exists(szInstallDirectory))
	{
		if(!SharedUtility::ReadRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "gtaivdir", NULL, 
											  szInstallDirectory, sizeof(szInstallDirectory)) || 
		   !SharedUtility::Exists(szInstallDirectory))
		{
			if(ShowMessageBox("Failed to retrieve GTA IV install directory from registry. Specify your GTA IV path now?", 
				(MB_ICONEXCLAMATION | MB_OKCANCEL)) == IDOK)
			{
				// Taken from http://vcfaq.mvps.org/sdk/20.htm
				BROWSEINFO browseInfo = { 0 };
				browseInfo.lpszTitle = "Pick a Directory";
				ITEMIDLIST * pItemIdList = SHBrowseForFolder(&browseInfo);

				if(pItemIdList != NULL)
				{
					// Get the name of the selected folder
					if(SHGetPathFromIDList(pItemIdList, szInstallDirectory))
						bFoundCustomDirectory = true;

					// Free any memory used
					IMalloc * pIMalloc = 0;
					if(SUCCEEDED(SHGetMalloc(&pIMalloc)))
					{
						pIMalloc->Free(pItemIdList);
						pIMalloc->Release();
					}
				}
			}

			if(!bFoundCustomDirectory)
			{
				ShowMessageBox("Failed to retrieve GTA IV install directory from registry. Cannot launch Networked: IV.");
				return FALSE;
			}
		}
	}

	// Get the full path to GTAIV.exe
	char szApplicationPath[MAX_PATH];
	sprintf(szApplicationPath, "%s\\GTAIV.exe", szInstallDirectory);

	// Make sure the GTAIV.exe path is valid
	if(!SharedUtility::Exists(szApplicationPath))
	{
		ShowMessageBox("Failed to find GTAIV.exe. Cannot launch Networked: IV.");
		return FALSE;
	}

	// If we have a custom directory save it
	if(bFoundCustomDirectory)
		SharedUtility::WriteRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "gtaivdir", szInstallDirectory, strlen(szInstallDirectory));

	// Convert the install directory to unicode
	wchar_t wszInstallDirectory[MAX_PATH];
	int iInstallDirectoryLength = SharedUtility::AnsiToUnicode(szInstallDirectory, strlen(szInstallDirectory), wszInstallDirectory, sizeof(wszInstallDirectory));
	wszInstallDirectory[iInstallDirectoryLength] = '\0';

	// Convert the application path to unicode
	wchar_t wszApplicationPath[MAX_PATH];
	int iApplicationPathLength = SharedUtility::AnsiToUnicode(szApplicationPath, strlen(szApplicationPath), wszApplicationPath, sizeof(wszApplicationPath));
	wszApplicationPath[iApplicationPathLength] = '\0';

	// Create the process
	BOOL bReturn = g_pfnCreateProcessW(wszApplicationPath, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, wszInstallDirectory, lpStartupInfo, lpProcessInformation);

	if(bReturn)
	{
		// Get the full path of the client dll
		char szLibraryPath[MAX_PATH];
		sprintf(szLibraryPath, "%s" CLIENT_CORE_NAME DEBUG_SUFFIX ".dll", SharedUtility::GetAppPath());

		// Inject Client.dll into GTAIV.exe
		int iReturn = SharedUtility::InjectLibraryIntoProcess(lpProcessInformation->hProcess, szLibraryPath);

		// Did the injection fail?
		if(iReturn > 0)
		{
			// Terminate the process
			TerminateProcess(lpProcessInformation->hProcess, 0);

			if(iReturn == 1)
				ShowMessageBox("Failed to write library path into remote process. Cannot launch Networked: IV.");
			else if(iReturn == 2)
				ShowMessageBox("Failed to create remote thread in remote process. Cannot launch Networked: IV.");
			else if(iReturn == 3)
				ShowMessageBox("Failed to open the remote process, Cannot launch Networked: IV.");

			return FALSE;
		}

		// Resume the GTAIV.exe thread
		ResumeThread(lpProcessInformation->hThread);
	}

	return bReturn;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			// Disable thread library notifications
			DisableThreadLibraryCalls(hModule);

			// Install the CreateProcessW hook
			g_pfnCreateProcessW = (CreateProcessW_t)CPatcher::InstallDetourPatch("Kernel32.dll", "CreateProcessW", (DWORD)CreateProcessW_Hook);
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			// Uninstall our CreateProcessW hook
			CPatcher::UninstallDetourPatch((void *)g_pfnCreateProcessW, (DWORD)CreateProcessW);
		}
		break;
	}

	return TRUE;
}

//============== IV-Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Launcher
// License: See LICENSE in root directory
//
//=============================================================================

#include <StdInc.h>

int ShowMessageBox(char * szText)
{
	return MessageBox(NULL, szText, "IV:MP", MB_ICONEXCLAMATION | MB_OK);
}

bool GetProcessIdFromProcessName(char * szProcessName, DWORD * dwProcessId)
{
	bool bReturn = false;

	// Create a tool help 32 process snapshot
	HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(!hProcessSnapShot)
		return false;

	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(ProcessEntry);

	if(Process32First(hProcessSnapShot, &ProcessEntry))
	{
		// Enumerate through all processes
		while(Process32Next(hProcessSnapShot, &ProcessEntry))
		{
			// Check the process name to see if it matches szProcessName
			if(!strcmp(ProcessEntry.szExeFile, szProcessName))
			{
				// It matches, set the process id and return true
				*dwProcessId = ProcessEntry.th32ProcessID;
				bReturn = true;
				break;
			}
		}
	}

	// Close the snapshot handle
	CloseHandle(hProcessSnapShot); 

	return bReturn;
}

bool IsProcessRunning(char * szProcessName)
{
	// Simply return the value of GetProcessIdFromProcessName
	DWORD dwProcessId = 0;
	return GetProcessIdFromProcessName(szProcessName, &dwProcessId);
}

bool InjectLibraryIntoProcess(DWORD dwProcessId, char * szLibraryPath)
{
	bool bReturn = true;

	// Open our target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

	if(!hProcess)
	{
		// Failed to open the process
		ShowMessageBox("Failed to open the remote process, Cannot launch IV:MP.");
		return false;
	}

	// Get the length of the library path
	size_t sLibraryPathLen = (strlen(szLibraryPath) + 1);

	// Allocate the a block of memory in our target process for the library name
	void * pRemoteLibraryPath = VirtualAllocEx(hProcess, NULL, sLibraryPathLen, MEM_COMMIT, PAGE_READWRITE);

	// Write our library name to the allocated block of memory
	SIZE_T sBytesWritten = 0;
	WriteProcessMemory(hProcess, pRemoteLibraryPath, (void *)szLibraryPath, sLibraryPathLen, &sBytesWritten);

	if(sBytesWritten != sLibraryPathLen)
	{
		ShowMessageBox("Failed to write library path into remote process. Cannot launch IV:MP.");
		bReturn = false;
	}
	else
	{
		// Get the handle of Kernel32.dll
		HMODULE hKernel32 = GetModuleHandle("Kernel32");

		// Get the address of the LoadLibraryA function from Kernel32.dll
		FARPROC pfnLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

		// Create a thread inside the target process to load our library
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibraryA, pRemoteLibraryPath, 0, NULL);

		if(hThread)
		{
			// Wait for the created thread to end
			WaitForSingleObject(hThread, INFINITE);

			// Get the remote thread exit code
			/*DWORD dwExitCode;
			GetExitCodeThread(hThread, &dwExitCode);

			if(dwExitCode != 0)
			{
				ShowMessageBox("Failed to inject library into remote process. Cannot launch IV:MP.");
				bReturn = false;
			}*/

			// Close our thread handle
			CloseHandle(hThread);
		}
		else
		{
			// Thread creation failed
			ShowMessageBox("Failed to create remote thread in remote process. Cannot launch IV:MP.");
			bReturn = false;
		}
	}

	// Free the allocated block of memory inside the target process
	VirtualFreeEx(hProcess, pRemoteLibraryPath, sizeof(pRemoteLibraryPath), MEM_RELEASE);

	// If the injection failed terminate the target process
	if(!bReturn)
		TerminateProcess(hProcess, 0);

	// Close our target process handle
	CloseHandle(hProcess);

	return bReturn;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Get the LaunchGTAIV.exe path
	char szLaunchGTAIVExe[1024];
	sprintf(szLaunchGTAIVExe, "%sLaunchGTAIV.exe", SharedUtil::GetAppPath());

	// Check if LaunchGTAIV.exe exists
	WIN32_FIND_DATA fdFileInfo;
	if(FindFirstFile(szLaunchGTAIVExe, &fdFileInfo) == INVALID_HANDLE_VALUE)
	{
		ShowMessageBox("Failed to find LaunchGTAIV.exe. Cannot launch IV:MP.");
		return 1;
	}

	// Get the full path of Client.dll
	char szLibraryPath[MAX_PATH];
	sprintf(szLibraryPath, "%sIVMP\\Client.dll", SharedUtil::GetAppPath());

	// Check if Client.dll exists
	if(FindFirstFile(szLibraryPath, &fdFileInfo) == INVALID_HANDLE_VALUE)
	{
		ShowMessageBox("Failed to find Client.dll. Cannot launch IV:MP.");
		return 1;
	}

	// Get the full path of LauncherLibrary.dll
	sprintf(szLibraryPath, "%sIVMP\\LauncherLibrary.dll", SharedUtil::GetAppPath());

	// Check if Launcher.dll exists
	if(FindFirstFile(szLibraryPath, &fdFileInfo) == INVALID_HANDLE_VALUE)
	{
		ShowMessageBox("Failed to find LauncherLibrary.dll. Cannot launch IV:MP.");
		return 1;
	}

	// Check if LaunchGTAIV.exe is already running
	if(IsProcessRunning("LaunchGTAIV.exe"))
	{
		ShowMessageBox("LaunchGTAIV.exe is already running. Cannot launch IV:MP.");
		return 1;
	}

	// Check if GTAIV.exe is already running
	if(IsProcessRunning("GTAIV.exe"))
	{
		ShowMessageBox("GTAIV.exe is already running. Cannot launch IV:MP.");
		return 1;
	}

	// Start LaunchGTAIV.exe
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);
	if(!CreateProcess(szLaunchGTAIVExe, NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, SharedUtil::GetAppPath(), 
		&siStartupInfo, &piProcessInfo))
	{
		ShowMessageBox("Failed to start LaunchGTAIV.exe. Cannot launch IV:MP.");
		return 1;
	}

	// Inject our code into LaunchGTAIV.exe
	if(!InjectLibraryIntoProcess(piProcessInfo.dwProcessId, szLibraryPath))
	{
		TerminateProcess(piProcessInfo.hProcess, 0);
		ShowMessageBox("Failed to inject LauncherLibrary.dll into LaunchGTAIV.exe. Cannot launch IV:MP.");
		return 1;
	}

	// Resume the LaunchGTAIV.exe thread
	ResumeThread(piProcessInfo.hThread);

	return 0;
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: SharedUtility.cpp
// Project: Shared
// Author(s): jenksta
//            MaVe
//            Einstein
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>
#include <time.h>
#include <sys/stat.h>
#ifdef WIN32
#include <direct.h>
#else
#include <sys/time.h>
#endif
#include <errno.h>

namespace SharedUtility
{
bool StripPath1(char * szString)
{
	for(size_t i = strlen(szString); i > 0; --i)
	{
		if(szString[i] == '\\' || szString[i] == '/')
		{
			szString[i + 1] = '\0';
			return true;
		}
	}

	return false;
}

char * StripPath2(char * szString)
{
	char * szPath = szString;
	char * szStrippedPath = (szString + strlen(szString));

	while(szStrippedPath != szPath)
	{
		if(*szStrippedPath == '\\' || *szStrippedPath == '/')
			return (szStrippedPath + 1);

		szStrippedPath--;
	}

	return szPath;
}

const char * GetAppPath()
{
#ifdef WIN32
	static unsigned int nDummy;
#endif
	static char szAppPath[MAX_PATH];
#ifdef WIN32
	HMODULE hModuleHandle;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, 
		(LPCSTR)&nDummy, &hModuleHandle);
	GetModuleFileName(hModuleHandle, szAppPath, MAX_PATH);
#else
	readlink("/proc/self/exe", szAppPath, MAX_PATH);
#endif
	StripPath1(szAppPath);
	return szAppPath;
}

#ifdef WIN32
int InjectLibraryIntoProcess(HANDLE hProcess, const char * szLibraryPath)
{
	int iReturn = 0;

	// Get the length of the library path
	size_t sLibraryPathLen = (strlen(szLibraryPath) + 1);

	// Allocate the a block of memory in our target process for the library path
	void * pRemoteLibraryPath = VirtualAllocEx(hProcess, NULL, sLibraryPathLen, MEM_COMMIT, PAGE_READWRITE);

	// Write our library path to the allocated block of memory
	SIZE_T sBytesWritten = 0;
	WriteProcessMemory(hProcess, pRemoteLibraryPath, (void *)szLibraryPath, sLibraryPathLen, &sBytesWritten);

	if(sBytesWritten != sLibraryPathLen)
	{
		// Failed to write the library path
		iReturn = 1;
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

			// Close our thread handle
			CloseHandle(hThread);
		}
		else
		{
			// Thread creation failed
			iReturn = 2;
		}
	}

	// Free the allocated block of memory inside the target process
	VirtualFreeEx(hProcess, pRemoteLibraryPath, sizeof(pRemoteLibraryPath), MEM_RELEASE);

	return iReturn;
}

int InjectLibraryIntoProcess(DWORD dwProcessId, const char * szLibraryPath)
{
	// Open our target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

	if(!hProcess)
	{
		// Failed to open the process
		return 3;
	}

	// Inject the library into the process
	int iReturn = InjectLibraryIntoProcess(hProcess, szLibraryPath);

	// Close the process handle
	CloseHandle(hProcess);

	return iReturn;
}

bool ReadRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize)
{
	HKEY hKey = NULL;

	if(RegOpenKeyEx(hKeyLocation, szSubKey, NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		LONG lStatus = RegQueryValueEx(hKey, szKey, NULL, &dwType, (BYTE *)szData, &dwSize);
		RegCloseKey(hKey);
		return (lStatus == ERROR_SUCCESS);
	}

	if(szDefault)
		strncpy(szData, szDefault, dwSize);

	return false;
}

bool WriteRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, DWORD dwSize)
{
	HKEY hKey = NULL;
	RegOpenKeyEx(hKeyLocation, szSubKey, NULL, KEY_WRITE, &hKey);

	if(!hKey)
		RegCreateKey(hKeyLocation, szSubKey, &hKey);

	if(hKey)
	{
		DWORD dwType = REG_SZ;
		RegSetValueEx(hKey, szKey, NULL, dwType, (BYTE *)szData, dwSize);
		RegCloseKey(hKey);
		return true;
	}

	return false;
}

int AnsiToUnicode(const char * szAnsiString, size_t sAnsiStringLength, wchar_t * wszUnicodeString, size_t sUnicodeStringLength, DWORD dwFlags)
{
	return MultiByteToWideChar(CP_ACP, dwFlags, szAnsiString, sAnsiStringLength, wszUnicodeString, sUnicodeStringLength);
}

int UnicodeToAnsi(const wchar_t * wszUnicodeString, size_t sUnicodeStringLength, char * szAnsiString, size_t sAnsiStringLength, DWORD dwFlags)
{
	return WideCharToMultiByte(CP_UTF8, dwFlags, wszUnicodeString, sUnicodeStringLength, szAnsiString, sAnsiStringLength, NULL, NULL);
}
#endif

const char * GetTimeAndDateString()
{
	time_t timeValue;
	time(&timeValue);
	return ctime(&timeValue);
}

const char * GetTimeString()
{
	static char szTime[256];
	time_t t = time(NULL);
	const struct tm * tm = localtime(&t);
	strftime(szTime, sizeof(szTime), "%H:%M:%S", tm);
	return szTime;
}

void TimeInit()
{
#ifdef WIN32
	timeBeginPeriod(1);
#endif
}

void TimeShutdown()
{
#ifdef WIN32
	timeEndPeriod(1);
#endif
}

unsigned long GetTime()
{
#ifdef WIN32
	return timeGetTime();
#else
	timeval ts;
	gettimeofday(&ts, 0);
	return (DWORD)(ts.tv_sec * 1000 + (ts.tv_usec / 1000));
#endif
}

bool Exists(const char * szPath)
{
	struct stat St;
	return (stat(szPath, &St) == 0);
}

bool CopyFile(const char * szSource, const char * szDestination)
{
	// Open the source file
	FILE * fSource = fopen(szSource, "rb");

	if(!fSource)
		return false;

	// Open the destination file
	FILE * fDestination = fopen(szDestination, "wb");

	if(!fDestination)
	{
		fclose(fSource);
		return false;
	}

	// Copy the source file to the destination file
	char szBuffer[8192];
	size_t sBytesRead = 0;

	while((sBytesRead = fread(szBuffer, 1, sizeof(szBuffer), fSource)) > 0)
	{
		fwrite(szBuffer, 1, sBytesRead, fDestination);
	}

	// Close the destination file
	fclose(fDestination);

	// Close the source file
	fclose(fSource);
	return true;
}

int CreateDirectory(const char * szPath)
{
	// Create the directory
	int 
#ifdef WIN32
		iReturn = mkdir(szPath);
#else
		iReturn = mkdir(szPath, 0775);
#endif

	// Was the directory created successfully?
	if(!iReturn)
		return 1;

	// Does the directory already exist?
	if(iReturn == EEXIST)
		return 2;

	// Does the directory already exist?
#ifdef WIN32
	if(iReturn == -1 && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		SetLastError(0);
		return 2;
	}
#endif

	// Failed to create the directory
	return 0;
}

unsigned int IVHash(std::string strString, unsigned int uiInitialHash, bool bEnsureLowercase)
{
	// Ensure the string is lowercase if needed
	if(bEnsureLowercase)
	{
		for(size_t i = 0; i < strString.size(); i++)
			strString[i] = tolower(strString[i]);
	}

	// Set the initial hash value
	unsigned int uiValue = uiInitialHash;
	unsigned int uiTemp = 0;
	int iCurrent = 0;

	for(size_t sIndex = 0; sIndex < strString.size(); sIndex++)
	{
		iCurrent = strString[sIndex];

		if(iCurrent == '\\')
			iCurrent = '/';

		uiTemp = iCurrent;
		uiTemp = (uiTemp + uiValue);
		uiValue = (uiTemp << 10);
		uiTemp += uiValue;
		uiValue = (uiTemp >> 6);
		uiValue = (uiValue ^ uiTemp);
	}

	uiTemp = (uiValue << 3);
	uiTemp = (uiValue + uiTemp);
	unsigned int uiTemp2 = (uiTemp >> 11);
	uiTemp = (uiTemp2 ^ uiTemp);
	uiTemp2 = (uiTemp << 15);
	uiValue = (uiTemp2 + uiTemp);

	if(uiValue < 2)
		uiValue += 2;

	return uiValue;
}
};

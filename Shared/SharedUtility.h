//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: SharedUtility.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#ifdef NIV_DEBUG
#define DEBUG_SUFFIX ".Debug"
#else
#define DEBUG_SUFFIX
#endif

namespace SharedUtility
{
// Strip a path back to the last slash
bool StripPath1(char * szString);

// Return the stripped path from a string
// pointer but don't edit the actual pointer
char * StripPath2(char * szString);

// Get the application's path
const char * GetAppPath();

#ifdef WIN32
// Inject the library into the specified process
int InjectLibraryIntoProcess(HANDLE hProcess, const char * szLibraryPath);

// Get the process id's handle then inject the library into it
int InjectLibraryIntoProcess(DWORD dwProcessId, const char * szLibraryPath);

// Read a string from the registry
bool ReadRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize);

// Write a string to the registry
bool WriteRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, DWORD dwSize);

// Convert an Ansi string to a Unicode string
int AnsiToUnicode(const char * szAnsiString, size_t sAnsiStringLength, wchar_t * wszUnicodeString, size_t sUnicodeStringLength, DWORD dwFlags = NULL);

// Convert a Unicode string to an Ansi string
int UnicodeToAnsi(const wchar_t * wszUnicodeString, size_t sUnicodeStringLength, char * szAnsiString, size_t sAnsiStringLength, DWORD dwFlags = NULL);
#endif

// Returns the current time and date in the
// format 'DOW MO DY HR:MN:SC YR'
const char * GetTimeAndDateString();

// Returns the current time in the format
// 'HR:MN:SC'
const char * GetTimeString();

// 
void TimeInit();

// 
void TimeShutdown();

// 
unsigned long GetTime();

// Check if a path exists
bool Exists(const char * szPath);

// Copy a file to another file
bool CopyFile(const char * szSource, const char * szDestination);

// Creates a directory
int CreateDirectory(const char * szPath);

// Parts taken from SparkIV
// All of the IV hash functions merged into one
unsigned int IVHash(std::string strString, unsigned int uiInitialHash = 0, bool bEnsureLowercase = true);

/*namespace Memory
{
	static size_t sMemoryLimit = -1; // TODO: Implement
	static size_t sMemoryUsed = 0;
	struct MemoryTracker
	{
		String       strFile;
		unsigned int uiLine;
		void       * pMemory;
	};
	std::list<MemoryTracker *> m_memoryTrackers;

	bool AddMemoryTracker(const char * szFile, unsigned int uiLine, void * pMemory, size_t sMemorySize)
	{
		// Did the memory not allocate successfully?
		if(!pMemory)
		{
			//CLogFile::Printf("Failed to allocate memory (Size: %d, File: %s, Line :%d)!\n", sMemorySize, szFile, uiLine);
			return false;
		}

		// Allocate a new memory tracker
		MemoryTracker * pMemoryTracker = new MemoryTracker;

		// Did the memory tracker not allocate successfully?
		if(!pMemoryTracker)
		{
			//CLogFile::Printf("Failed to allocate memory tracker for memory 0x%p (Size: %d, File: %s, Line :%d)!\n", pMemory, sMemorySize, szFile, uiLine);
			return false;
		}

		// Initialize the memory tracker
		pMemoryTracker->strFile = szFile;
		pMemoryTracker->uiLine = uiLine;
		pMemoryTracker->pMemory = pMemory;

		// Add it to the memory tracker list
		m_memoryTrackers.push_back(pMemoryTracker);

		// Increment the memory used count
		sMemoryUsed += sMemorySize;
		return true;
	}

	template <class Type>
	Type * CustomNew(const char * szFile, unsigned int uiLine)
	{
		Type * pMemory = new Type();
#ifdef NIV_DEBUG
		AddMemoryTracker(szFile, uiLine, (void *)pMemory, sizeof(Type));
#endif
		return pMemory;
	}

	void * CustomMalloc(size_t sSize, const char * szFile, unsigned int uiLine)
	{
		void * pMemory = malloc(sSize);
#ifdef NIV_DEBUG
		AddMemoryTracker(szFile, uiLine, (void *)pMemory, sSize);
#endif
		return pMemory;
	}
};*/
};

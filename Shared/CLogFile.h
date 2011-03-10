//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CLogFile.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

typedef void (* LogFileCallback_t)(const char * szBuffer);

class CLogFile
{
private:
	static FILE *            m_fLogFile;
	static bool              m_bUseCallback;
	static LogFileCallback_t m_pfnCallback;

public:
	static void              Open(char * szLogFile, bool bAppend = false);
	static void              Printf(const char * szFormat, ...);
	static void              Close();
	static void              SetUseCallback(bool bUseCallback) { m_bUseCallback = bUseCallback; }
	static bool              GetUseCallback() { return m_bUseCallback; }
	static void              SetCallback(LogFileCallback_t pfnCallback) { m_pfnCallback = pfnCallback; }
	static LogFileCallback_t GetCallback() { return m_pfnCallback; }
};

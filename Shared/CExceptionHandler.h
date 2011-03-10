//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CExceptionHandler.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#ifdef WIN32
typedef void (* ExceptionHandlerCallback_t)(_EXCEPTION_POINTERS * ExceptionInfo);
#endif

class CExceptionHandler
{
private:
#ifdef WIN32
	static              ExceptionHandlerCallback_t m_pfnCallback;
	static const char * ExceptionCodeToString(DWORD dwExceptionCode);
	static bool         WriteMiniDump(_EXCEPTION_POINTERS * ExceptionInfo);
	static void         WriteUnhandledExceptionReport(_EXCEPTION_POINTERS * ExceptionInfo);
	static long WINAPI  ExceptionHandler(_EXCEPTION_POINTERS * ExceptionInfo);
#endif

public:
	static void         Install();
#ifdef WIN32
	static void         SetCallback(ExceptionHandlerCallback_t pfnCallback) { m_pfnCallback = pfnCallback; }
#endif
};

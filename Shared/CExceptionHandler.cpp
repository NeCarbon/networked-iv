//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CExceptionHandler.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#ifdef WIN32
// TODO: CLEAN ME UP!
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#include <tlhelp32.h>

ExceptionHandlerCallback_t CExceptionHandler::m_pfnCallback = NULL;

const char * CExceptionHandler::ExceptionCodeToString(DWORD dwExceptionCode)
{
	switch(dwExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION: return "Access Violation";
	case EXCEPTION_DATATYPE_MISALIGNMENT: return "Datatype Misalignment";
	case EXCEPTION_BREAKPOINT: return "Breakpoint";
	case EXCEPTION_SINGLE_STEP: return "Single Step";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "Array Bounds Exceeded";
	case EXCEPTION_FLT_DENORMAL_OPERAND: return "Float: Denormal Operand";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "Float: Divide By Zero";
	case EXCEPTION_FLT_INEXACT_RESULT: return "Float: Inexact Result";
	case EXCEPTION_FLT_INVALID_OPERATION: return "Float: Invalid Operation";
	case EXCEPTION_FLT_OVERFLOW: return "Float: Overflow";
	case EXCEPTION_FLT_STACK_CHECK: return "Float: Stack Check";
	case EXCEPTION_FLT_UNDERFLOW: return "Float: Underflow";
	case EXCEPTION_INT_DIVIDE_BY_ZERO: return "Integer: Divide By Zero";
	case EXCEPTION_INT_OVERFLOW: return "Integer: Overflow";
	case EXCEPTION_PRIV_INSTRUCTION: return "Priv Instruction";
	case EXCEPTION_IN_PAGE_ERROR: return "In Page Error";
	case EXCEPTION_ILLEGAL_INSTRUCTION: return "Illegal Instruction";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "Non-continuable Exception";
	case EXCEPTION_STACK_OVERFLOW: return "Stack Overflow";
	case EXCEPTION_INVALID_DISPOSITION: return "Invalid Disposition";
	case EXCEPTION_GUARD_PAGE: return "Guard Page";
	case EXCEPTION_INVALID_HANDLE: return "Invalid Handle";
	case 0xC0000194L: return "Possible Deadlock";
	}

	return "Unknown";
}

bool CExceptionHandler::WriteMiniDump(_EXCEPTION_POINTERS * ExceptionInfo)
{
	// Get the application path
	String sMiniDumpPath(SharedUtility::GetAppPath());

	// Get the current system time
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	// Append the file name and system type to the application path string
	// TODO: Write dumps to their own 'crashdumps' folder and possibly client/server prefix/suffix?
	sMiniDumpPath.AppendF("crashdump-" MOD_VERSION_STRING "-%02d.%02d.%02d-%02d.%02d.%04d.dmp", systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wDay, systemTime.wMonth, systemTime.wYear);

	// Open the minidump file
	HANDLE hFile = CreateFileA(sMiniDumpPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL , NULL);

	// Did we open the minidump file?
	if(hFile)
	{
		// Create the minidump exception information
		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ThreadId = GetCurrentThreadId();
		exceptionInfo.ExceptionPointers = ExceptionInfo;
		exceptionInfo.ClientPointers = FALSE;

		// Write the minidump to the minidump file
		bool bWritten = (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, 
			&exceptionInfo, NULL, NULL) != 0);

		// Close the minidump file
		CloseHandle(hFile);

		return bWritten;
	}

	// Failed to open the minidump file
	return false;
}

void CExceptionHandler::WriteUnhandledExceptionReport(_EXCEPTION_POINTERS * ExceptionInfo)
{
	// Write the unhandled exception report start notice to the log file
	CLogFile::Printf("-------------- Unhandled Exception Report Start --------------\n");

	// Write the exception code and exception code string to the log file
	CLogFile::Printf("Exception code: 0x%p (%s)\n", ExceptionInfo->ExceptionRecord->ExceptionCode, 
		ExceptionCodeToString(ExceptionInfo->ExceptionRecord->ExceptionCode));

	// Write the exception address to the log file
	CLogFile::Printf("Exception address: 0x%p\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);

	// Create a tool help 32 process snapshot
	HANDLE hModuleSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

	if(hModuleSnapShot)
	{
		MODULEENTRY32 ModuleEntry;
		ModuleEntry.dwSize = sizeof(ModuleEntry);

		if(Module32First(hModuleSnapShot, &ModuleEntry))
		{
			// Enumerate through all modules
			while(Module32Next(hModuleSnapShot, &ModuleEntry))
			{
				// See if exception was within this module
				if((ExceptionInfo->ContextRecord->Eip >= (DWORD)ModuleEntry.modBaseAddr) && (ExceptionInfo->ContextRecord->Eip <= ((DWORD)ModuleEntry.modBaseAddr + ModuleEntry.modBaseSize)))
				{
					CLogFile::Printf("Exception module: %s (+0x%p)\n", ModuleEntry.szModule, (ExceptionInfo->ContextRecord->Eip - (DWORD)ModuleEntry.modBaseAddr));
					break;
				}
			}
		}
	}

	// Write the registers segment header
	CLogFile::Printf("Exception registers: \n");

	// If we have segments context information then write it to the log file
	if(ExceptionInfo->ContextRecord->ContextFlags & CONTEXT_SEGMENTS)
	{
		CLogFile::Printf("GS=0x%p FS=0x%p ES=0x%p DS=0x%p\n", ExceptionInfo->ContextRecord->SegGs, 
			ExceptionInfo->ContextRecord->SegFs, ExceptionInfo->ContextRecord->SegEs, 
			ExceptionInfo->ContextRecord->SegDs);
	}

	// If we have integer context information then write it to the log file
	if(ExceptionInfo->ContextRecord->ContextFlags & CONTEXT_INTEGER)
	{
		CLogFile::Printf("EDI=0x%p ESI=0x%p EBX=0x%p EDX=0x%p\n", ExceptionInfo->ContextRecord->Edi, 
			ExceptionInfo->ContextRecord->Esi, ExceptionInfo->ContextRecord->Ebx,
			ExceptionInfo->ContextRecord->Edx);
		CLogFile::Printf("ECX=0x%p EAX=0x%p\n", ExceptionInfo->ContextRecord->Ecx, 
			ExceptionInfo->ContextRecord->Eax);
	}

	// If we have control context information then write it to the log file
	if(ExceptionInfo->ContextRecord->ContextFlags & CONTEXT_CONTROL)
	{
		CLogFile::Printf("EBP=0x%p EIP=0x%p CS=0x%p EFLAGS=0x%p\n", ExceptionInfo->ContextRecord->Ebp, 
			ExceptionInfo->ContextRecord->Eip, ExceptionInfo->ContextRecord->SegCs, 
			ExceptionInfo->ContextRecord->EFlags);
		CLogFile::Printf("ESP=0x%p SS=0x%p\n", ExceptionInfo->ContextRecord->Esp, 
			ExceptionInfo->ContextRecord->SegSs);
	}

	// If we have a callback call it
	if(m_pfnCallback)
		m_pfnCallback(ExceptionInfo);

	// Write the unhandled exception report end notice to the log file
	CLogFile::Printf("-------------- Unhandled Exception Report End --------------\n");
}

long WINAPI CExceptionHandler::ExceptionHandler(_EXCEPTION_POINTERS * ExceptionInfo)
{
	// Write the exception report
	WriteUnhandledExceptionReport(ExceptionInfo);

#ifndef NIV_DEBUG
	// Write the minidump file
	WriteMiniDump(ExceptionInfo);

	// Terminate the current process
	TerminateProcess(GetCurrentProcess(), 0);

	return EXCEPTION_EXECUTE_HANDLER;
#else
	return EXCEPTION_CONTINUE_SEARCH;
#endif
}
#endif

void CExceptionHandler::Install()
{
	// Set the unhandled exception filter
#ifdef WIN32
	SetUnhandledExceptionFilter(ExceptionHandler);
#endif
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CSquirrel.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>
#include <squirrel/sqstate.h>
#include <squirrel/sqstdaux.h>
#include <squirrel/sqstdblob.h>
#include <squirrel/sqstdio.h>
#include <squirrel/sqstdmath.h>
#include <squirrel/sqstdstring.h>
#include <squirrel/sqstdsystem.h>
#include <squirrel/sqvm.h>
#include "natives/CServerNatives.h"

/************************************/
/* Some old shit from the other mod */
/************************************/
void printfunc(HSQUIRRELVM vm, const char *s,...)
{
	va_list vl;
	char tmp[256];
	va_start(vl, s);
	vsprintf(tmp, s, vl);
	va_end(vl);
	CLogFile::Printf("%s",tmp);
}

void errorfunc(HSQUIRRELVM vm, const char *s, ...)
{
	va_list args;
	char tmps[256];
	va_start(args, s);
	vsprintf(tmps, s, args);
	va_end(args);

	char *tmp = (char *)tmps;

	size_t offstart = 0, offend = 0;

	size_t len = strlen(tmp);
	for (size_t i = 0; i < len; ++i)
	{
		switch (tmp[i])
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			++offstart;
			break;
		default:
			i = len - 1;
			break;
		}
	}

	tmp += offstart;
	len -= offstart;

	for (size_t i = len - 1; i > 0; --i)
	{
		switch (tmp[i])
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			++offend;
			break;
		default:
			i = 1;
			break;
		}
	}

	tmp[len - offend] = '\0';

	CLogFile::Printf("%s\n", tmp);
}
/************************************/

extern CRootEntity* g_pRootEntity;

CSquirrel::CSquirrel(CResource* pResource)
{
	m_pResource = pResource;

	// create the VM
	m_pVM = sq_open(1024);
	if( m_pVM )
	{
		// Push the root table onto the stack
		sq_pushroottable(m_pVM);

		// Register the default error handles
		sqstd_seterrorhandlers(m_pVM);

		// Set the print and error functions
		sq_setprintfunc(m_pVM, printfunc, errorfunc);
		
		// Register the blob library
		sqstd_register_bloblib(m_pVM);

		// Register the input/out library
		sqstd_register_iolib(m_pVM);

		// Register the math library
		sqstd_register_mathlib(m_pVM);

		// Register the string library
		sqstd_register_stringlib(m_pVM);

		// Register the system library
		sqstd_register_systemlib(m_pVM);

		// 'root' global variable
		sq_pushroottable(m_pVM);
		sq_pushstring(m_pVM, "root", -1);
		sq_pushentity(m_pVM, g_pRootEntity);
		sq_newslot(m_pVM, -3, true);

		// 'resource' global variable
		sq_pushroottable(m_pVM);
		sq_pushstring(m_pVM, "resource", -1);
		sq_pushentity(m_pVM, pResource);
		sq_newslot(m_pVM, -3, true);

		// Register our own functions
		CServerNatives::LoadFunctions( this );
		CEntityNatives::LoadFunctions( this );
		CPlayerNatives::LoadFunctions( this );
		CResourceNatives::LoadFunctions( this );
		CEventNatives::LoadFunctions( this );
		CTimerNatives::LoadFunctions( this );
	}
}

CSquirrel::~CSquirrel()
{
	if(m_pVM)
	{
		// Pop the root table from the stack
		sq_pop(m_pVM, 1);

		// Close the squirrel VM
		sq_close(m_pVM);
	}
}

bool CSquirrel::LoadScript( String script )
{
	String scriptPath( "%s/%s", m_pResource->GetPath().Get(), script.Get() );
	if( SQ_FAILED( sqstd_dofile( m_pVM, scriptPath.Get(), SQFalse, SQTrue ) ) )
	{
		CLogFile::Printf("[%s] Failed to load file %s.\n", m_pResource->GetName().Get(), script.Get());
		return false;
	}
	CLogFile::Printf("[%s] Loaded file %s.\n", m_pResource->GetName().Get(), script.Get());
	return true;
}

bool CSquirrel::LoadScripts( std::list<String> scripts )
{
	for(std::list<String>::iterator iter = scripts.begin(); iter != scripts.end(); ++ iter)
		if( !LoadScript( *iter ) )
			return false;
	return true;
}

void CSquirrel::RegisterFunction(const char * szFunctionName, SQFUNCTION pfnFunction, int iParameterCount, const char * szFunctionTemplate, bool bPushRootTable)
{
	// Push the root table onto the stack if needed
	if(bPushRootTable)
		sq_pushroottable(m_pVM);

	// Push the function name onto the stack
	sq_pushstring(m_pVM, szFunctionName, -1);

	// Create a new function
	sq_newclosure(m_pVM, pfnFunction, 0);

	// Set the function parameter template and count
	if(iParameterCount != -1)
	{
		char szTemp[128];

		if(szFunctionTemplate)
			sprintf(szTemp, ".%s", szFunctionTemplate);

		sq_setparamscheck(m_pVM, (iParameterCount + 1), (szFunctionTemplate) ? szTemp : NULL);
	}

	// Create a new slot
	sq_createslot(m_pVM, -3);

	// Pop the root table from the stack if needed
	if(bPushRootTable)
		sq_pop(m_pVM, 1);
}

bool CSquirrel::Call(CEntity* pSource, SQObjectPtr& pFunction, CSquirrelArguments* pArguments)
{
	// Get the stack top
	int iTop = sq_gettop(m_pVM);

	// 'source' variable
	sq_pushroottable(m_pVM);
	sq_pushstring(m_pVM, "source", -1);
	if(pSource)
		sq_pushentity(m_pVM, pSource);
	else
		sq_pushnull(m_pVM);
	sq_newslot(m_pVM, -3, true);

	// push the root table onto the stack
	sq_pushroottable(m_pVM);

	// push all arguments
	if(pArguments)
		pArguments->push_to_vm(m_pVM);

	// Call the function
	SQObjectPtr res;
	bool success = m_pVM->Call(pFunction, (pArguments?pArguments->size():0)+1, m_pVM->_top-(pArguments?pArguments->size():0)-1, res, true);

	// Restore the stack top
	sq_settop(m_pVM, iTop);

	// delete the 'source' variable
	sq_pushroottable(m_pVM);
	sq_pushstring(m_pVM, "source", -1);
	sq_deleteslot(m_pVM, -2, false);

	// return value dependant on the function's return (if it's a boolean false, then this will return false - otherwise true)
	if( success )
		return res._type != OT_BOOL || ( res._unVal.nInteger == 0 ? false : true );
	else
		return false;
}

SQVM* CSquirrel::GetVM()
{
	return m_pVM;
}

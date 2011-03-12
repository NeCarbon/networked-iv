//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CSquirrel.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CResource;

class CSquirrel
{
public:
				CSquirrel(CResource* pResource);
				~CSquirrel();

	bool		LoadScript(String script);
	bool		LoadScripts(std::list<String> scripts);

	void		RegisterFunction(const char* szFunctionName, SQFUNCTION pfnFunction, int iParameterCount = -1, const char* szFunctionTemplate = NULL, bool bPushRootTable = false );
	bool		Call(CEntity* pEntity, SQObjectPtr& pFunction, CSquirrelArguments* pArguments);

	SQVM*		GetVM();
private:
	CResource*	m_pResource;
	SQVM*		m_pVM;
};

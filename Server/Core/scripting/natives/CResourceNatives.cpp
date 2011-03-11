//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CResourceNatives.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CResourceManager * g_pResourceManager;

void CResourceNatives::LoadFunctions(CSquirrel* pSquirrel)
{
	pSquirrel->RegisterFunction("getThisResource", GetResource, 0);
	pSquirrel->RegisterFunction("getResourceFromName", GetFromName, 1, "s");
	pSquirrel->RegisterFunction("getResourceName", GetName);
	pSquirrel->RegisterFunction("startResource", Start, 1, "p");
	pSquirrel->RegisterFunction("restartResource", Restart, 1, "p");
	pSquirrel->RegisterFunction("stopResource", Stop, 1, "p");
}

int CResourceNatives::GetResource(SQVM* pVM)
{
	CResource* pResource = g_pResourceManager->Get(pVM);
	assert( pResource );

	sq_pushentity(pVM, pResource);
	return 1;
}

int CResourceNatives::GetFromName(SQVM* pVM)
{
	const char* szText;
	sq_getstring(pVM,2,&szText);
	
	CResource* pResource = g_pResourceManager->Get(szText);
	if(pResource)
		sq_pushentity(pVM,pResource);
	else
	{
		// Try to load the resource if we didn't know of it
		if(g_pResourceManager->Load(szText,false))
		{
			pResource = g_pResourceManager->Get(szText);
			if(pResource)
				sq_pushentity(pVM,pResource);
			else
				sq_pushnull(pVM);
		}
		else
			sq_pushnull(pVM);
	}
	return 1;
}

int CResourceNatives::GetName(SQVM* pVM)
{
	CResource* pResource = NULL;
	if(sq_gettop(pVM)<2) // no parameter passed, use the current resource
		pResource = g_pResourceManager->Get(pVM);
	else
		pResource = sq_toresource(pVM,2);

	if(pResource)
		sq_pushstring(pVM,pResource->GetName().Get(),pResource->GetName().GetLength());
	else
		sq_pushnull(pVM);
	return 1;
}

int CResourceNatives::Start(SQVM* pVM)
{
	CResource* pResource = sq_toresource(pVM, 2);
	if(pResource)
		sq_pushbool(pVM, g_pResourceManager->Start(pResource));
	else
		sq_pushbool(pVM, false);
	return 1;
}

int CResourceNatives::Restart(SQVM* pVM)
{
	CResource* pResource = sq_toresource(pVM, 2);
	if(pResource)
		sq_pushbool(pVM, g_pResourceManager->Restart(pResource));
	else
		sq_pushbool(pVM, false);
	return 1;
}

int CResourceNatives::Stop(SQVM* pVM)
{
	CResource* pResource = sq_toresource(pVM, 2);
	if(pResource)
		sq_pushbool(pVM, g_pResourceManager->Stop(pResource));
	else
		sq_pushbool(pVM, false);
	return 1;
}

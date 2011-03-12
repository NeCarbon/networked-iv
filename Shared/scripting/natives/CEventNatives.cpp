//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CEntityNatives.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CResourceManager * g_pResourceManager;

void CEventNatives::LoadFunctions(CSquirrel* pSquirrel)
{
	pSquirrel->RegisterFunction("addEventHandler", AddHandler, 3, "spc");
	pSquirrel->RegisterFunction("removeEventHandler", RemoveHandler, 3, "spc");
	pSquirrel->RegisterFunction("callEvent", Call, -1, "sp");
}

int CEventNatives::AddHandler(SQVM* pVM)
{
	// get the event name
	const char* szEventName;
	sq_getstring(pVM, 2, &szEventName);

	// get the entity which it should effect
	CEntity* pEntity = sq_toentity(pVM, 3);

	// get the function to execute upon calling the event
	SQObject o;
	sq_getstackobj(pVM, 4, &o);

	// if we have a valid entity, use it
	if(pEntity && (o._type == OT_CLOSURE || o._type == OT_NATIVECLOSURE))
	{
		sEventData event;
		event.pFunction = o;

		// get the resource calling this function
		event.pResource = g_pResourceManager->Get(pVM);
		assert(event.pResource);

		// pass the result to the script
		sq_pushbool(pVM, pEntity->AddEvent(szEventName, event));
	}
	else
		sq_pushbool(pVM, false);

	return 1;
}

int CEventNatives::RemoveHandler(SQVM* pVM)
{
	// get the event name
	const char* szEventName;
	sq_getstring(pVM, 2, &szEventName);

	// get the entity which it should effect
	CEntity* pEntity = sq_toentity(pVM, 3);

	// get the function to execute upon calling the event
	SQObject o;
	sq_getstackobj(pVM, 4, &o);

	// if we have a valid entity, use it
	if(pEntity)
	{
		sEventData event;
		event.pFunction = o;

		// get the resource calling this function
		event.pResource = g_pResourceManager->Get(pVM);
		assert(event.pResource);

		// pass the result to the script
		sq_pushbool(pVM, pEntity->RemoveEvent(szEventName, event));
	}
	else
		sq_pushbool(pVM, false);

	return 1;
}

int CEventNatives::Call(SQVM* pVM)
{
	// we need enough parameters
	if(sq_gettop(pVM)<3)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// get the event name
	const char* szEventName;
	if(SQ_FAILED(sq_getstring(pVM, 2, &szEventName)))
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	// get the entity which it should effect
	CEntity* pEntity = sq_toentity(pVM, 3);

	// if we have a valid entity, use it
	if(pEntity)
	{
		// collect the parameters
		CSquirrelArguments* pArguments = new CSquirrelArguments();
		for(int i = 4; i <= sq_gettop(pVM); ++ i)
		{
			// push all arguments from the stack
			if(!pArguments->pushFromStack(pVM, i))
			{
				sq_pushbool(pVM, false);
				delete pArguments;
				return 1;
			}
		}
		sq_pushbool(pVM, pEntity->CallEvent(szEventName, pArguments));
		delete pArguments;
	}
	else
		sq_pushbool(pVM, false);

	return 1;
}

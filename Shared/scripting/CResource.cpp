//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CResource.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CRootEntity * g_pRootEntity;

CResource::CResource(String name) : CEntity(ENTITY_TYPE_RESOURCE, g_pRootEntity, "resource")
{
	m_bValidMeta = false;
	m_eState = STATE_ERROR;
	m_strName = name;
	m_pVM = NULL;
	m_pTimers = NULL;

	// Invalid Entity
	if(GetID() == INVALID_ENTITY_ID_LONG)
		return;

	// place all files are located in
#ifdef _SERVER

	m_strResourcePath = String("%sresources/%s", SharedUtility::GetAppPath(), name.Get());
#else
	m_strResourcePath = String("%sclient/resources/%s", SharedUtility::GetAppPath(), name.Get());
#endif
	// load the meta.xml
	if(!Reload())
		return;

	// it's valid after all and could be started
	m_bValidMeta = true;
	m_eState = STATE_LOADED;
}

CResource::~CResource()
{
	g_pRootEntity->RemoveEvents(this);
}

// Reloads the meta.xml
bool CResource::Reload()
{
	// it's now invalid
	m_bValidMeta = false;

	// clear our saved stuff
	m_scripts.clear();

	// Load all scripts in the meta.xml, create the xml instance
	CXML * pXML = new CXML();

	// Load the meta.xml file
	if(!pXML || !pXML->load(String( "%s/meta.xml", m_strResourcePath.Get())))
	{
		CLogFile::Printf("Unable to load resource %s - no meta.xml\n", GetName().Get());
		return false;
	}

	// Get the first script node
	if(pXML->findNode("script"))
	{
		while(true)
		{
			// Get the script name
			String strScript = pXML->nodeContent();

			// Make sure the name is valid and attempt to load the script
			if(strScript && !strScript.IsEmpty())
				m_scripts.push_back(strScript);

			// Attempt to load the next script node (if any)
			if(!pXML->nextNode())
				break;
		}
	}

	// Delete the xml instance
	SAFE_DELETE(pXML);

	// it's now valid
	m_bValidMeta = true;
	return true;
}

bool CResource::Start()
{
	// if it's not a loaded, ignore it
	if( GetState() != STATE_LOADED )
		return false;

	// create a squirrel VM
	m_pVM = new CSquirrel(this);
	if( !m_pVM )
	{
		CLogFile::Printf("[%s] Failed to create Squirrel VM.\n", GetName().Get());
		return false;
	}

	// create the timers manager
	m_pTimers = new CTimers(this);
	if( !m_pTimers )
	{
		CLogFile::Printf("[%s] Failed to create timers.\n", GetName().Get());
		return false;
	}

	// show a message
	CLogFile::Printf("[%s] Started resource.\n", GetName().Get());
	m_eState = STATE_RUNNING;

	// load all scripts, if at least one fails cancel the startup
	if( !m_pVM->LoadScripts( m_scripts ) )
	{
		m_eState = STATE_STARTUP_CANCELLED;
		Stop();
		return false;
	}

	// call 'resourceStart' event, if it fails then cancel the startup
	if(!CallEvent( "resourceStart", NULL, true, false))
	{
		m_eState = STATE_STARTUP_CANCELLED;
		Stop();
		return false;
	}
	return true;
}

bool CResource::Stop()
{
	// if it's not a valid resource, we can't stop it
	if( GetState() != STATE_RUNNING && GetState() != STATE_STARTUP_CANCELLED )
		return false;

	if( GetState() == STATE_RUNNING )
		CallEvent("resourceStop", NULL, true, false);

	// Remove all events added by this resource
	g_pRootEntity->RemoveEvents(this);

	// kill all timers
	SAFE_DELETE(m_pTimers);

	// remove all children of this resource
	DeleteAllChildren();

	// delete the VM
	SAFE_DELETE(m_pVM);

	// Show a message
	CLogFile::Printf("[%s] Stopped Resource.", GetName().Get());
	m_eState = STATE_LOADED;
	return true;
}

bool CResource::IsValidMeta()
{
	return m_bValidMeta;
}

void CResource::Process(DWORD dwTickCount)
{
	if( GetState() == STATE_RUNNING )
	{
		// Execute all outstanding timers
		m_pTimers->Process(dwTickCount);
	}
}

CResource::eResourceState CResource::GetState()
{
	return m_eState;
}

String CResource::GetName()
{
	return m_strName;
}

String CResource::GetPath()
{
	return m_strResourcePath;
}

CSquirrel* CResource::GetVM()
{
	return m_pVM;
}

CTimers* CResource::GetTimers()
{
	return m_pTimers;
}

bool CResource::SetParent(CEntity* pEntity)
{
	// we won't allow anyone to set our parent element
	return false;
}

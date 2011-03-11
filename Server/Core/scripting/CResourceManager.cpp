//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CResourceManager.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	m_pResources.clear();
}

CResource* CResourceManager::Get(String name)
{
	for(std::list<CResource*>::iterator iter = m_pResources.begin(); iter != m_pResources.end(); ++ iter)
		if( (*iter)->GetName() == name )
			return *iter;
	return NULL;
}

CResource* CResourceManager::Get(SQVM* pVM)
{
	for(std::list<CResource*>::iterator iter = m_pResources.begin(); iter != m_pResources.end(); ++ iter)
		if( (*iter)->GetVM() && (*iter)->GetVM()->GetVM() == pVM )
			return *iter;
	return NULL;
}

bool CResourceManager::Load(String name, bool start)
{
	// if a function with that name exists, we don't load it
	if(Get(name))
		return false;
	
	// create a new entity
	CResource* pResource = new CResource(name);

	// if it has no valid meta, stop it from loading
	if(pResource->IsValidMeta())
	{
		// save it into our resources list (this needs to be done -before- starting the resource or some scripting functions (requiring CResource* CResourceManager::Get(SQVM* pVM)) will fail)
		m_pResources.push_back(pResource);

		// attempt tot start it if we're told to
		if(!start || Start(pResource))
		{
			return true;
		}
		else
		{
			// failed to start, remove it
			m_pResources.remove(pResource);
			delete pResource;
			return false;
		}
	}
	else
	{
		// invalid meta file
		delete pResource;
		return false;
	}
}

bool CResourceManager::Start(CResource* pResource)
{
	// Check if we're scheduled for a stop or restart
	for(std::list<sResourceQueue>::iterator iter = m_pResourceQueue.begin(); iter != m_pResourceQueue.end(); ++ iter)
		if((*iter).pResource == pResource)
			return false;

	if(pResource && pResource->GetState() == CResource::STATE_LOADED)
		return pResource->Start();
	return false;
}

bool CResourceManager::Stop(CResource* pResource)
{
	if(pResource && pResource->GetState() == CResource::STATE_RUNNING)
	{
		// Check if we're scheduled for a stop or restart
		for(std::list<sResourceQueue>::iterator iter = m_pResourceQueue.begin(); iter != m_pResourceQueue.end(); ++ iter)
			if((*iter).pResource == pResource)
				return false;

		sResourceQueue sItem;
		sItem.pResource = pResource;
		sItem.eType = QUEUE_STOP;
		m_pResourceQueue.push_back(sItem);
		return true;
	}
	return false;
}

bool CResourceManager::Restart(CResource* pResource)
{
	if(pResource && pResource->GetState() == CResource::STATE_RUNNING)
	{
		// Check if we're scheduled for a stop or restart
		for(std::list<sResourceQueue>::iterator iter = m_pResourceQueue.begin(); iter != m_pResourceQueue.end(); ++ iter)
			if((*iter).pResource == pResource)
				return false;

		sResourceQueue sItem;
		sItem.pResource = pResource;
		sItem.eType = QUEUE_RESTART;
		m_pResourceQueue.push_back(sItem);
		return true;
	}
	return false;
}

void CResourceManager::Process(DWORD dwTickCount)
{
	for(std::list<CResource*>::iterator iter = m_pResources.begin(); iter != m_pResources.end(); ++ iter)
		(*iter)->Process(dwTickCount);

	// at this time, we shouldn't really be in any kind of resource stuff so deleting should be fine - calling stopResource(getThisResource()) would delete the Squirrel VM while being used
	if(m_pResourceQueue.size()>0)
	{
		for(std::list<sResourceQueue>::iterator iter = m_pResourceQueue.begin(); iter != m_pResourceQueue.end(); ++ iter)
		{
			switch( (*iter).eType )
			{
				case QUEUE_STOP:
					(*iter).pResource->Stop();
					break;
				case QUEUE_RESTART:
					if((*iter).pResource->Stop())
						(*iter).pResource->Start();
					break;
			}
		}
		m_pResourceQueue.clear();
	}
}

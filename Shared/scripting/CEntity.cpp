//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CEntity.cpp
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CResourceManager * g_pResourceManager;

std::deque<EntityID> CEntityIDs::m_EntityIDs;
CEntity* CEntityIDs::m_pEntitys[MAX_ENTITIES];

void CEntityIDs::Initalize()
{
	for(EntityID e=1;e<MAX_ENTITIES;++e)
	{
		m_EntityIDs.push_front(e);
	}
	memset(m_pEntitys,0,MAX_ENTITIES);
}

EntityID CEntityIDs::Pop(CEntity* pEntity)
{
	if(m_EntityIDs.size() > 0)
	{
		EntityID e = m_EntityIDs.back();
		m_EntityIDs.pop_back();
		m_pEntitys[e] = pEntity;
		return e;
	}
	return INVALID_ENTITY_ID_LONG;
}

void CEntityIDs::Push(CEntity* pEntity)
{
	if(pEntity->GetID() != INVALID_ENTITY_ID_LONG)
	{
		m_EntityIDs.push_back(pEntity->GetID());
		m_pEntitys[pEntity->GetID()] = NULL;
	}
}

//==========================================================================

CEntity* CEntityIDs::Get(EntityID e)
{
	if(e!=INVALID_ENTITY_ID_LONG && e>=1 && e<=MAX_ENTITIES)
	{
		return m_pEntitys[e];
	}
	return NULL;
}

CEntity::CEntity(eEntityType EntityType, CEntity* pParent, String strTag)
{
	m_ID = CEntityIDs::Pop(this);
	m_EntityType = EntityType;
	m_strTag = strTag;

	// set our parent entity
	m_pParent = NULL;
	SetParent(pParent);

#if 0 && defined(NIV_DEBUG)
	CLogFile::Printf("CEntity[%d]::CEntity(%d,0x%08x:%s,%s)\n",m_ID,EntityType,pParent,pParent?pParent->GetTag().Get():"[none]",strTag.Get());
#endif
}

CEntity::~CEntity()
{
#if 0 && defined(NIV_DEBUG)
	CLogFile::Printf("CEntity[%d]::~CEntity()\n",m_ID);
#endif

	// Remove all children elements
	DeleteAllChildren();

	// remove the parent
	SetParent(NULL);

	// push the entity ID back into the pool
	CEntityIDs::Push(this);
}

EntityID CEntity::GetID()
{
	return m_ID;
}

eEntityType CEntity::GetType()
{
	return m_EntityType;
}

String CEntity::GetTag()
{
	return m_strTag;
}

eEntityType CEntity::GetType(String strTag)
{
	if(strTag == "root")
		return ENTITY_TYPE_ROOT;
	else if(strTag == "player")
		return ENTITY_TYPE_PLAYER;
	else if(strTag == "resource")
		return ENTITY_TYPE_RESOURCE;
	else if(strTag == "timer")
		return ENTITY_TYPE_TIMER;

	return ENTITY_TYPE_CUSTOM;
}

CEntity* CEntity::GetParent()
{
	return m_pParent;
}

bool CEntity::SetParent(CEntity* pParent)
{
	if( m_pParent != pParent )
	{
		// do we have a parent?
		if( m_pParent )
		{
			// remove us from our parent
			m_pParent->m_pChildren.remove( this );
		}

		// set us as new parent
		m_pParent = pParent;

		// add us to our new parent if there's any
		if( pParent )
		{
			// add us to our new parent's child list
			m_pParent->m_pChildren.push_back( this );
		}
		return true;
	}
	return false;
}

bool CEntity::IsChildrenOf(CEntity* pEntity)
{
	if(pEntity == this)
		return true;

	if(m_pParent)
		return m_pParent->IsChildrenOf(pEntity);
	else
		return false;
}

bool CEntity::IsParentOf(CEntity* pEntity)
{
	return pEntity->IsChildrenOf(this);
}

void CEntity::DeleteAllChildren()
{
	while(m_pChildren.size() > 0)
	{
		// As the CEntity-destructor calls SetParent(NULL), we can skip erasing it from the list
		delete *(m_pChildren.begin());
	}
}

bool CEntity::AddEvent(String strEventName, sEventData sData)
{
	// search the Event
	std::map<String, std::list<sEventData> >::iterator iter = m_pEvents.find(strEventName);

	if(iter == m_pEvents.end())
	{
		// Event does not yet exist, create it
		std::pair<std::map<String, std::list<sEventData> >::iterator,bool> ret;
		ret = m_pEvents.insert(std::pair<String, std::list<sEventData> >(strEventName, std::list<sEventData>()));

		// error creating it 
		if(ret.second == false)
			return false;

		iter = ret.first;
	}
	else // Check if we already have this event with the same function - if so, cancel it
		for(std::list<sEventData>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++ iter2 )
			if(sData.pResource == (*iter2).pResource && sData.pFunction == (*iter2).pFunction)
				return false;

	iter->second.push_back(sData);
	return true;
}

bool CEntity::RemoveEvent(String strEventName, sEventData sData)
{
	// search the Event
	std::map<String, std::list<sEventData> >::iterator iter = m_pEvents.find(strEventName);

	if(iter != m_pEvents.end())
	{
		// This event exists for this entity (and, by this, has at least one function)
		for(std::list<sEventData>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++ iter2 )
		{
			// We do have this event that can be removed
			if(sData.pResource == (*iter2).pResource && sData.pFunction == (*iter2).pFunction)
			{
				// remove it
				iter->second.erase(iter2);

				// if there's no such event handlers for this event anymore, remove it at all.
				if(iter->second.size() == 0)
					m_pEvents.erase(iter);

				return true;
			}
		}
	}
	return false;
}

void CEntity::RemoveEvents(CResource* pCreator)
{
	// Check only if we have any kind of event
	if(m_pEvents.size()>0)
	{
		// loop through all events
		for(std::map<String, std::list<sEventData> >::iterator iter = m_pEvents.begin(); iter != m_pEvents.end();)
		{
			// loop through all attached event handlers
			for(std::list<sEventData>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end();)
			{
				// delete the event if it's created by pCreator
				if((*iter2).pResource == pCreator)
					iter->second.erase(iter2++);
				else
					iter2++;
			}

			// If there's no more added events for that event name, delete it
			if(iter->second.size()==0)
				m_pEvents.erase(iter++);
			else
				iter++;
		}
	}
	
	// Call it for all children
	for(std::list<CEntity*>::iterator iter = m_pChildren.begin(); iter != m_pChildren.end(); ++ iter)
		(*iter)->RemoveEvents(pCreator);
}

bool CEntity::CallEvent(String strEventName, CSquirrelArguments* pArguments, bool bCallForParent, bool bCallForChildren, CEntity* pSource)
{
	if( !pSource )
		pSource = this;

	// returns 'false' if one or more event calls return false
	bool bReturn = true;

	// search the Event within this entity
	std::map<String, std::list<sEventData> >::iterator iter = m_pEvents.find(strEventName);

	// event exists
	if(iter != m_pEvents.end())
	{
		// loop through all attached event handlers
		for(std::list<sEventData>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++ iter2 )
		{
			// call the according function for each one
			if(!(*iter2).pResource->GetVM()->Call(pSource, (*iter2).pFunction, pArguments))
			{
				// if the call failed, this returns false
				bReturn = false;
			}
		}
	}

	// Call recursively up to our root entity
	if(bCallForParent && m_pParent && !m_pParent->CallEvent(strEventName, pArguments, true, false, pSource))
		bReturn = false;

	// Call it for all children
	if(bCallForChildren)
		for(std::list<CEntity*>::iterator iter = m_pChildren.begin(); iter != m_pChildren.end(); ++ iter)
			if(!(*iter)->CallEvent(strEventName, pArguments, false, true, pSource))
				bReturn = false;

	return bReturn;
}


//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CEntity.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>
#include <squirrel/sqobject.h>

class CEntity;
class CEntityIDs
{
private:
	static std::deque<EntityID> m_EntityIDs;
	static CEntity* m_pEntitys[MAX_ENTITIES];
public:
	static void			Initalize( );

	static EntityID	Pop(CEntity* pEntity);
	static void			Push(CEntity* pEntity);

	static CEntity*	Get(EntityID e);
};

class CResource;
class CSquirrel;
class CSquirrelArguments;
struct sEventData
{
	CResource* pResource;
	SQObjectPtr pFunction;
};

class CEntity
{
public:
	CEntity(eEntityType EntityType, CEntity* pParent, String strTag);
	virtual ~CEntity();

	EntityID	       GetID();
	eEntityType	       GetType();
	String		       GetTag();

	CEntity *	       GetParent();
	bool		       SetParent(CEntity* pEntity);
	bool		       IsChildrenOf(CEntity* pEntity);
	bool		       IsParentOf(CEntity* pEntity);
	void		       DeleteAllChildren();
	
	static eEntityType GetType(String strTag);

	// Events stuff
	bool		AddEvent		(String strEventName, sEventData sData);
	bool		RemoveEvent		(String strEventName, sEventData sData);
	void		RemoveEvents	(CResource* pCreator);
	bool		CallEvent		(String strEventName, CSquirrelArguments* arguments, bool bCallForParent = true, bool bCallForChildren = true, CEntity* pSource = NULL);
private:
	EntityID	m_ID;
	eEntityType	m_EntityType;
	String		m_strTag;
	CEntity*	m_pParent;

protected:
	std::list<CEntity*> m_pChildren;
	std::map<String, std::list<sEventData>> m_pEvents;
};
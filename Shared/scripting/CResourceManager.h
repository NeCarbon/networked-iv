//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CResourceManager.h
// Project: Server
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CResourceManager
{
	enum eResourceQueueType
	{
		QUEUE_STOP,
		QUEUE_RESTART,
		QUEUE_UNLOAD
	};

	struct sResourceQueue
	{
		CResource* pResource;
		eResourceQueueType eType;
	};
public:
				CResourceManager(String strResourceDirectory);
				~CResourceManager();

	CResource*	Get(String name);
	CResource*	Get(SQVM* pVM);

	bool		Load(String strName, bool bStart = true);
	bool		Start(CResource* pResource);
	bool		Stop(CResource* pResource);
	bool		Restart(CResource* pResource);

	void		Process(unsigned long ulTime);
private:
	String                      m_strResourceDirectory;
	std::list<CResource*>		m_pResources;
	std::list<sResourceQueue>	m_pResourceQueue;
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPedTaskManager.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CIVPedTaskManager::CIVPedTaskManager(IVPedTaskManager * pPedTaskManager)
{
	SetPedTaskManager(pPedTaskManager);
}

void CIVPedTaskManager::SetPedTaskManager(IVPedTaskManager * pPedTaskManager)
{
	m_pPedTaskManager = pPedTaskManager;
}

IVPedTaskManager * CIVPedTaskManager::GetPedTaskManager()
{
	return m_pPedTaskManager;
}

void CIVPedTaskManager::SetTask(CIVTask * pTask, int iTaskPriority, bool bForceNewTask)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPedTaskManager__SetTask);
		IVPedTaskManager * pPedTaskManager = m_pPedTaskManager;
		IVTask * pGameTask = pTask->GetTask();
		_asm
		{
			push bForceNewTask
			push iTaskPriority
			push pGameTask
			mov ecx, pPedTaskManager
			call dwFunc
		}
	}
}

void CIVPedTaskManager::RemoveTask(int iTaskPriority)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		// Make sure its not the default task (We will crash without it)
		if(iTaskPriority != TASK_PRIORITY_DEFAULT)
			SetTask(NULL, iTaskPriority);
	}
}

CIVTask * CIVPedTaskManager::GetTask(int iTaskPriority)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		if(iTaskPriority < TASK_PRIORITY_MAX)
			return g_pClient->GetClientTaskManager()->GetClientTaskFromGameTask(m_pPedTaskManager->m_tasks[iTaskPriority]);
	}

	return NULL;
}

CIVTask * CIVPedTaskManager::GetTaskSecondary(int iType)
{
	// Do we have a valid ped task manager pointer?
	if(m_pPedTaskManager)
	{
		if(iType < TASK_SECONDARY_MAX)
			return g_pClient->GetClientTaskManager()->GetClientTaskFromGameTask(m_pPedTaskManager->m_secondaryTasks[iType]);
	}

	return NULL;
}

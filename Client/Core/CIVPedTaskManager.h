//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPedTaskManager.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define FUNC_CPedTaskManager__SetTask 0x9E58B0

// From Multi Theft Auto
// Priority Task Types
enum eTaskPriority
{
	TASK_PRIORITY_PHYSICAL_RESPONSE = 0,
	TASK_PRIORITY_EVENT_RESPONSE_TEMP,
	TASK_PRIORITY_EVENT_RESPONSE_NONTEMP,
	TASK_PRIORITY_PRIMARY,
	TASK_PRIORITY_DEFAULT,
	TASK_PRIORITY_MAX
};

// From Multi Theft Auto
// Secondary Task Types
enum eTaskSecondary
{
	TASK_SECONDARY_ATTACK = 0,              // want duck to be after attack
	TASK_SECONDARY_DUCK,                    // because attack controls ducking movement
	TASK_SECONDARY_SAY,
	TASK_SECONDARY_FACIAL_COMPLEX,
	TASK_SECONDARY_PARTIAL_ANIM,
	TASK_SECONDARY_IK,
	TASK_SECONDARY_MAX
};

class IVTask;
class CIVTask;

class IVPedTaskManager
{
public:
	IVTask * m_tasks[TASK_PRIORITY_MAX];
	IVTask * m_secondaryTasks[TASK_SECONDARY_MAX];
	PAD(IVTask, pad0, (3 * 4)); // 3 * (IVTask *) (Natural Motion Perhaps?)?
	// TODO: Find size
};

class CIVPedTaskManager
{
private:
	IVPedTaskManager * m_pPedTaskManager;

public:
	CIVPedTaskManager(IVPedTaskManager * pPedTaskManager);

	void               SetPedTaskManager(IVPedTaskManager * pPedTaskManager);
	IVPedTaskManager * GetPedTaskManager();

	// Priority tasks
	void               SetTask(CIVTask * pTask, int iTaskPriority, bool bForceNewTask = false);
	void               RemoveTask(int iTaskPriority);
	CIVTask          * GetTask(int iTaskPriority);

	// Secondary tasks
	CIVTask          * GetTaskSecondary(int iType);
};

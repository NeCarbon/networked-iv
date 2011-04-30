//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: IVTasks.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CIVTaskComplexNewGetInVehicle::CIVTaskComplexNewGetInVehicle(CIVVehicle * pVehicle, int a3, int a4, unsigned int a5, float a6) : CIVTask()
{
	// Add ourselves to the client task manager
	g_pClient->GetClientTaskManager()->AddTask(this);

	// Create the task
	Create();

	// Call the task constructor
	IVVehicle * pGameVehicle = pVehicle->GetVehicle();
	IVTask * pTask = GetTask();
#define FUNC_CTaskComplexNewGetInVehicle__CTaskComplexNewGetInVehicle_7 0xA2BCF0
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CTaskComplexNewGetInVehicle__CTaskComplexNewGetInVehicle_7);
	_asm
	{
		push a6
		push a5
		push a4
		push a3
		push pGameVehicle
		mov ecx, pTask
		call dwFunc
	}
}

CIVTaskComplexNewExitVehicle::CIVTaskComplexNewExitVehicle(CIVVehicle * pVehicle, int a3, int a4, int a5) : CIVTask()
{
	// Add ourselves to the client task manager
	g_pClient->GetClientTaskManager()->AddTask(this);

	// Create the task
	Create();

	// Call the task constructor
	IVVehicle * pGameVehicle = pVehicle->GetVehicle();
	IVTask * pTask = GetTask();
#define FUNC_CTaskComplexNewExitVehicle__CTaskComplexNewExitVehicle_7 0xA2C920
	DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CTaskComplexNewExitVehicle__CTaskComplexNewExitVehicle_7);
	_asm
	{
		push a5
		push a4
		push a3
		push pGameVehicle
		mov ecx, pTask
		call dwFunc
	}
}

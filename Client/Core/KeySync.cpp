//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: KeySync.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

IVPed         * m_pPed = NULL;
IVVehicle     * m_pKeySyncIVVehicle = NULL;
unsigned int    m_uiLocalPlayerIndex = 0;
IVPad           m_localPad;
Matrix          m_matLocalCameraMatrix;
IVPlayerInfo  * __pPlayerInfo = NULL;
bool            bInLocalContext = true;

extern CClient * g_pClient;

CCam * GetGameCam()
{
	return (CCam *)((void **)(COffsets::VAR_CameraUnknown) + 0xC);
}

void SetGameCameraMatrix(Matrix * matMatrix)
{
	CCam * pCam = GetGameCam();

	if(pCam)
	{
		memcpy(&pCam->m_data1.m_matMatrix.vecRight, &matMatrix->vecRight, sizeof(CVector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecFront, &matMatrix->vecFront, sizeof(CVector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecUp, &matMatrix->vecUp, sizeof(CVector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecPosition, &matMatrix->vecPosition, sizeof(CVector3));
	}
}

void GetGameCameraMatrix(Matrix * matMatrix)
{
	CCam * pCam = GetGameCam();

	if(pCam)
	{
		memcpy(&matMatrix->vecRight, &pCam->m_data1.m_matMatrix.vecRight, sizeof(CVector3));
		memcpy(&matMatrix->vecFront, &pCam->m_data1.m_matMatrix.vecFront, sizeof(CVector3));
		memcpy(&matMatrix->vecUp, &pCam->m_data1.m_matMatrix.vecUp, sizeof(CVector3));
		memcpy(&matMatrix->vecPosition, &pCam->m_data1.m_matMatrix.vecPosition, sizeof(CVector3));
	}
}

/*bool bRecordHistory = false;
bool bRecordHistory2 = false;*/

void ContextSwitch(IVPed * pPed, bool bPost)
{
	// Do we have a valid ped pointer?
	if(pPed)
	{
		// Get the game pad
		CIVPad * pPad = g_pClient->GetGame()->GetPad();

		/*CContextData * pTestContextInfo = CContextDataManager::GetContextData((IVPlayerPed *)pPed);
		CIVPad * pTestPad = g_pClient->GetGame()->GetPad();
		bool bLocalPlayer = ((IVPlayerPed *)pPed == CPools::GetPlayerInfoFromIndex(0)->m_pPlayerPed);
		if(!bLocalPlayer && pTestContextInfo)
		{
			pTestPad = pTestContextInfo->GetPad();
		}
		CClientPadState curPad;
		CClientPadState lasPad;
		pTestPad->GetCurrentClientPadState(curPad);
		pTestPad->GetLastClientPadState(lasPad);
		if(curPad.byteKeys[INPUT_MELEE_KICK] && !lasPad.byteKeys[INPUT_MELEE_KICK])
		{
			CLogFile::Printf("(%d) Melee kick start hold\n", bLocalPlayer);
			if(bLocalPlayer)
				bRecordHistory = true;
			else
				bRecordHistory2 = true;
		}
		if(bRecordHistory || bRecordHistory2)
		{
			CLogFile::Printf("(%d) Current history taken at %d is %d\n", bLocalPlayer, pTestPad->GetPad()->m_padData[INPUT_MELEE_KICK].m_pHistory->m_historyItems[pTestPad->GetPad()->m_padData[INPUT_MELEE_KICK].m_byteHistoryIndex].m_dwLastUpdateTime, pTestPad->GetPad()->m_padData[INPUT_MELEE_KICK].m_pHistory->m_historyItems[pTestPad->GetPad()->m_padData[INPUT_MELEE_KICK].m_byteHistoryIndex].m_byteValue);
		}
		if(!curPad.byteKeys[INPUT_MELEE_KICK] && lasPad.byteKeys[INPUT_MELEE_KICK])
		{
			CLogFile::Printf("(%d) Melee kick end hold\n", bLocalPlayer);
			if(bLocalPlayer)
				bRecordHistory = false;
			else
				bRecordHistory2 = false;
		}*/

		// Is this not the local player ped?
		if((IVPlayerPed *)pPed != CPools::GetPlayerInfoFromIndex(0)->m_pPlayerPed)
		{
			if(!bPost && !bInLocalContext)
			{
				CLogFile::Printf("Not switching due to not being in local context!\n");
				return;
			}

			if(bPost && bInLocalContext)
			{
				CLogFile::Printf("Not switching due to being in local context!\n");
				return;
			}

			// Get the remote players context info
			CContextData * pContextInfo = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

			// Do we have a valid context info?
			if(pContextInfo)
			{
				//CLogFile::SetUseCallback(false);
				//CLogFile::Printf("ContextSwitch(0x%p, %d) (Player Ped %d)\n", pPed, bPost, pContextInfo->GetPlayerInfo()->GetPlayerNumber());
				//CLogFile::SetUseCallback(true);

				if(!bPost)
				{
					// Store the local players index
					m_uiLocalPlayerIndex = CPools::GetLocalPlayerIndex();

					// Store the local players pad
					memcpy(&m_localPad, pPad->GetPad(), sizeof(IVPad));

					// Store the local players camera matrix
					GetGameCameraMatrix(&m_matLocalCameraMatrix);

					// Swap the local player index with the remote players index
					CPools::SetLocalPlayerIndex(pContextInfo->GetPlayerInfo()->GetPlayerNumber());

					// Set the history values
					for(int i = 0; i < INPUT_COUNT; i++)
					{
						CPadData * pPadData = &pContextInfo->GetPad()->GetPad()->m_padData[i];

						if(pPadData->m_pHistory)
						{
							pPadData->m_byteHistoryIndex++;

							if(pPadData->m_byteHistoryIndex >= MAX_HISTORY_ITEMS)
								pPadData->m_byteHistoryIndex = 0;

							pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_byteValue = pContextInfo->GetPad()->GetPad()->m_padData[i].m_byteLastValue;
							pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_dwLastUpdateTime = g_pClient->GetGame()->GetTime();
						}
					}
			
					// Swap the local players pad with the remote players pad
					memcpy(pPad->GetPad(), pContextInfo->GetPad()->GetPad(), sizeof(IVPad));

					// Swap the local players camera matrix with the remote players camera matrix
					//SetGameCameraMatrix(pContextInfo->GetCameraMatrix());

					// Flag ourselves as no longer in local context
					bInLocalContext = false;
				}
				else
				{
					// Restore the local players camera matrix
					SetGameCameraMatrix(&m_matLocalCameraMatrix);

					// Restore the local players pad
					memcpy(pPad->GetPad(), &m_localPad, sizeof(IVPad));

					// Restore the local players index
					CPools::SetLocalPlayerIndex(m_uiLocalPlayerIndex);

					// Flag ourselves as back in local context
					bInLocalContext = true;
				}
			}
			else
				CLogFile::Printf("ContextSwitch Warning: Invalid Player Ped\n");
		}
	}
}

void _declspec(naked) CPlayerPed__ProcessInput_Hook()
{
	_asm
	{
		mov m_pPed, ecx
		pushad
	}

	//CLogFile::Printf("CPlayerPed__ProcessInput Pre\n");
	ContextSwitch(m_pPed, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlayerPed__ProcessInput
		pushad
	}

	ContextSwitch(m_pPed, true);
	//CLogFile::Printf("CPlayerPed__ProcessInput Post\n");

	_asm
	{
		popad
		retn
	}
}

void _declspec(naked) CAutomobile_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CAutomobile__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		retn
	}
}

void _declspec(naked) CBike_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBike__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		retn
	}
}

void _declspec(naked) CBoat_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBoat__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		retn
	}
}

void _declspec(naked) CTrain_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CTrain__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		retn
	}
}

void _declspec(naked) CHeli_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CHeli__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		retn
	}
}

void _declspec(naked) CPlane_ProcessInput_Hook()
{
	_asm
	{
		mov m_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlane__ProcessInput
		pushad
	}

	ContextSwitch(m_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		retn
	}
}

DWORD dwEaxSave;
DWORD dwEcxSave;
DWORD dwCTaskComplexAimAndThrowProjectile__ControlSubTask;
DWORD dwCTaskComplexGangDriveby__ControlSubTask;
DWORD dwCTaskComplexGun__ControlSubTask;

void _declspec(naked) CTaskComplexAimAndThrowProjectile__ControlSubTask_Hook()
{
	_asm
	{
		mov dwEaxSave, eax
		mov eax, [esp+0Ch+4]
		mov m_pPed, eax
		mov eax, dwEaxSave
		pushad
	}

	CLogFile::Printf("CTaskComplexAimAndThrowProjectile__ControlSubTask Pre\n");
	ContextSwitch(m_pPed, false);

	_asm
	{
		popad
		call dwCTaskComplexAimAndThrowProjectile__ControlSubTask
		pushad
	}

	ContextSwitch(m_pPed, true);
	CLogFile::Printf("CTaskComplexAimAndThrowProjectile__ControlSubTask Post\n");

	_asm
	{
		popad
		retn 4
	}
}

void _declspec(naked) CTaskComplexGangDriveby__ControlSubTask_Hook()
{
	_asm
	{
		mov dwEaxSave, eax
		mov eax, [esp+8+4]
		mov m_pPed, eax
		mov eax, dwEaxSave
		pushad
	}

	CLogFile::Printf("CTaskComplexGangDriveby__ControlSubTask Pre\n");
	ContextSwitch(m_pPed, false);

	_asm
	{
		popad
		call dwCTaskComplexGangDriveby__ControlSubTask
		pushad
	}

	ContextSwitch(m_pPed, true);
	CLogFile::Printf("CTaskComplexGangDriveby__ControlSubTask Post\n");

	_asm
	{
		popad
		retn 4
	}
}

void _declspec(naked) CTaskComplexGun__ControlSubTask_Hook()
{
	_asm
	{
		mov dwEcxSave, ecx
		mov eax, [esp+4]
		mov m_pPed, eax
		//pushad
	}

	CLogFile::Printf("CTaskComplexGun__ControlSubTask Pre\n");
	ContextSwitch(m_pPed, false);

	_asm
	{
		//popad
		mov ecx, dwEcxSave
		push m_pPed
		call dwCTaskComplexGun__ControlSubTask
		mov eax, dwEaxSave
		//pushad
	}

	ContextSwitch(m_pPed, true);
	CLogFile::Printf("CTaskComplexGun__ControlSubTask Post\n");

	_asm
	{
		//popad
		mov eax, dwEaxSave
		retn 4
	}
}

DWORD dwTaskHookFunc;
void _declspec(naked) TaskHookFunc()
{
	_asm
	{
		mov dwEcxSave, ecx
		mov eax, [esp+4]
		mov m_pPed, eax
		//pushad
	}

	ContextSwitch(m_pPed, false);

	_asm
	{
		//popad
		push m_pPed
		mov ecx, dwEcxSave
		call dwTaskHookFunc
		//pushad
	}

	ContextSwitch(m_pPed, true);

	_asm
	{
		//popad
		retn 4
	}
}

DWORD dwCTaskSimpleFireGun__SetPedPosition;

void _declspec(naked) CTaskSimpleFireGun__SetPedPosition_Hook()
{
	_asm
	{
		mov dwEcxSave, ecx
		mov eax, [esp+4]
		mov m_pPed, eax
		//pushad
	}

	//CLogFile::Printf("CTaskSimpleFireGun__SetPedPositionk Pre\n");
	ContextSwitch(m_pPed, false);

	_asm
	{
		//pushad
		push m_pPed
		mov ecx, dwEcxSave
		call dwCTaskSimpleFireGun__SetPedPosition
		//popad
	}

	ContextSwitch(m_pPed, true);
	//CLogFile::Printf("CTaskSimpleFireGun__SetPedPositionk Post\n");

	_asm
	{
		//popad
		retn 4
	}
}

void InstallKeySyncHooks()
{
	// CPlayerPed::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CPlayerPed__VFTable + 0x74), (DWORD)CPlayerPed__ProcessInput_Hook);

	// CAutomobile::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CAutomobile__VFTable + 0x74), (DWORD)CAutomobile_ProcessInput_Hook);

	// CBike::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CBike__VFTable + 0x74), (DWORD)CBike_ProcessInput_Hook);

	// CBoat::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CBoat__VFTable + 0x74), (DWORD)CBoat_ProcessInput_Hook);

	// CTrain::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CTrain__VFTable + 0x74), (DWORD)CTrain_ProcessInput_Hook);

	// CHeli::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CHeli__VFTable + 0x74), (DWORD)CHeli_ProcessInput_Hook);

	// CPlane::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CPlane__VFTable + 0x74), (DWORD)CPlane_ProcessInput_Hook);

	// CTaskComplexAimAndThrowProjectile::ControlSubTask
	/*dwCTaskComplexAimAndThrowProjectile__ControlSubTask = (g_pClient->GetBaseAddress() + 0xCABB10);
	CPatcher::InstallMethodPatch((g_pClient->GetBaseAddress() + 0xDCAEDC), (DWORD)CTaskComplexAimAndThrowProjectile__ControlSubTask_Hook);

	// CTaskComplexGangDriveby::ControlSubTask
	dwCTaskComplexGangDriveby__ControlSubTask = (g_pClient->GetBaseAddress() + 0xA98180);
	CPatcher::InstallMethodPatch((g_pClient->GetBaseAddress() + 0xD8EF0C), (DWORD)CTaskComplexGangDriveby__ControlSubTask_Hook);

	// CTaskComplexGun::ControlSubTask
	dwCTaskComplexGun__ControlSubTask = (g_pClient->GetBaseAddress() + 0xA61440);
	CPatcher::InstallMethodPatch((g_pClient->GetBaseAddress() + 0xD8C4D4), (DWORD)CTaskComplexGun__ControlSubTask_Hook);*/
	// CTaskSimpleFireGun::SetPedPosition
	//dwCTaskSimpleFireGun__SetPedPosition = (g_pClient->GetBaseAddress() + 0xCCA0E0);
	//CPatcher::InstallMethodPatch((g_pClient->GetBaseAddress() + 0xDCD8C0), (DWORD)CTaskSimpleFireGun__SetPedPosition_Hook);

	//dwTaskHookFunc = (g_pClient->GetBaseAddress() + 0xCC8140);
	//CPatcher::InstallMethodPatch((g_pClient->GetBaseAddress() + 0xDCD6C0), (DWORD)TaskHookFunc);

	// CTaskComplexPlayerGun::ControlSubTask
	// This might call CTaskComplexGun::ControlSubTask so i'll leave it for now ;)
	//0xD80A3C

	// CTaskComplexPlayIdles::ControlSubTask = 0xC3 to disable idle anims?

	// CPlayerPed::GetPad
	//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x9BE5D0), (DWORD)CPlayerPed__GetPad_Hook);
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: KeySync.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

IVPed * m_pPed;
IVVehicle * m_pKeySyncIVVehicle;
unsigned int m_uiLocalPlayerIndex;
CClientPadState m_localClientPadState;
Matrix m_matLocalCameraMatrix;
//BYTE m_byteEnterExitVehicleKey[2];

extern CClient * g_pClient;

IVPad * GetGamePad()
{
	return (IVPad *)COffsets::VAR_Pads;
}

void ResetGamePadState()
{
	SetGamePadState(&CClientPadState());
}

void SetGamePadState(CClientPadState * padState)
{
	IVPad * pPad = GetGamePad();

	if(pPad)
	{
		for(int i = 0; i < INPUT_MAX; i++)
		{
			pPad->m_padData[i].m_byteUnknown6 = padState->byteCurrentKeys[i];
			pPad->m_padData[i].m_byteUnknown7 = padState->bytePreviousKeys[i];
		}
	}
}

void GetGamePadState(CClientPadState * padState)
{
	IVPad * pPad = GetGamePad();

	if(pPad)
	{
		for(int i = 0; i < INPUT_MAX; i++)
		{
			padState->byteCurrentKeys[i] = pPad->m_padData[i].m_byteUnknown6;
			padState->bytePreviousKeys[i] = pPad->m_padData[i].m_byteUnknown7;
		}
	}
}

CCam * GetGameCam()
{
	return (CCam *)((void **)(COffsets::VAR_CameraUnknown) + 0xC);
}

void SetGameCameraMatrix(Matrix * matMatrix)
{
	CCam * pCam = GetGameCam();

	if(pCam)
	{
		memcpy(&pCam->m_data1.m_matMatrix.vecRight, &matMatrix->vecRight, sizeof(Vector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecFront, &matMatrix->vecFront, sizeof(Vector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecUp, &matMatrix->vecUp, sizeof(Vector3));
		memcpy(&pCam->m_data1.m_matMatrix.vecPosition, &matMatrix->vecPosition, sizeof(Vector3));
	}
}

void GetGameCameraMatrix(Matrix * matMatrix)
{
	CCam * pCam = GetGameCam();

	if(pCam)
	{
		memcpy(&matMatrix->vecRight, &pCam->m_data1.m_matMatrix.vecRight, sizeof(Vector3));
		memcpy(&matMatrix->vecFront, &pCam->m_data1.m_matMatrix.vecFront, sizeof(Vector3));
		memcpy(&matMatrix->vecUp, &pCam->m_data1.m_matMatrix.vecUp, sizeof(Vector3));
		memcpy(&matMatrix->vecPosition, &pCam->m_data1.m_matMatrix.vecPosition, sizeof(Vector3));
	}
}

IVPlayerInfo * __pPlayerInfo = NULL;
BYTE byteValue[INPUT_MAX];
bool bInLocalContext = true;

void ContextSwitch(IVPed * pPed, bool bPost)
{
	if(pPed)
	{
		if((IVPlayerPed *)pPed == CPools::GetPlayerInfoFromIndex(0)->m_pPlayerPed)
		{
			/*if(!bUsingLocalKeys)
			{
				SetGamePadState(&m_localClientPadState);
				bUsingLocalKeys = true;
			}*/
			// Local player ped
			//CLogFile::Printf("ContextSwitch(0x%p, %d) (Local Player Ped)\n", pPed, bPost);

			// Get the local players keys
			/*CClientPadState padState;
			GetGamePadState(&padState);

			// Don't process enter/exit vehicle key as we handle vehicle entry/exit ourselves
			if(!bPost)
			{
				m_byteEnterExitVehicleKey[0] = padState.byteCurrentKeys[GTA_KEY_ENTEREXIT_VEHICLE];
				m_byteEnterExitVehicleKey[1] = padState.bytePreviousKeys[GTA_KEY_ENTEREXIT_VEHICLE];
				padState.byteCurrentKeys[GTA_KEY_ENTEREXIT_VEHICLE] = 0;
				padState.bytePreviousKeys[GTA_KEY_ENTEREXIT_VEHICLE] = 0;
			}
			else
			{
				padState.byteCurrentKeys[GTA_KEY_ENTEREXIT_VEHICLE] = m_byteEnterExitVehicleKey[0];
				padState.bytePreviousKeys[GTA_KEY_ENTEREXIT_VEHICLE] = m_byteEnterExitVehicleKey[1];
			}

			// Set the local players keys
			SetGamePadState(&padState);*/
		}
		else
		{
			CContextData * pContextInfo = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

			CLogFile::Printf("ContextSwitch(0x%p, %d) (Player Ped %d)\n", pPed, bPost, pContextInfo->GetPlayerInfo()->GetPlayerNumber());

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

			if(pContextInfo)
			{
				if(!bPost)
				{
					if(!bInLocalContext)
						CLogFile::Printf("Not in local context when we should be!\n");

					// TODO: Context switch current/desired move states?
					// Context switch the local player id with the remote players
					m_uiLocalPlayerIndex = CPools::GetLocalPlayerIndex();
					// NOTE: Without the local player ped checks for key/camera tasks e.t.c. disabled, the below will
					// stop other players responding to key/camera stuff altogether
					CPools::SetLocalPlayerIndex(pContextInfo->GetPlayerInfo()->GetPlayerNumber());

					// Context switch the local players keys with the remote players
					GetGamePadState(&m_localClientPadState);
					SetGamePadState(pContextInfo->GetPadState());
					IVPad * pGamePad = GetGamePad();
					for(int i = 0; i < INPUT_MAX; i++)
					{
						if(pGamePad->m_padData[i].m_pUnknown)
						{
							byteValue[i] = pGamePad->m_padData[i].m_pUnknown->m_unknown[pGamePad->m_padData[i].m_byteUnknown8].m_byteUnknown0;
							pGamePad->m_padData[i].m_pUnknown->m_unknown[pGamePad->m_padData[i].m_byteUnknown8].m_byteUnknown0 = pContextInfo->GetPadState()->byteCurrentKeys[i];
						}
					}

					// Context switch the local players cam matrix with the remote players
					GetGameCameraMatrix(&m_matLocalCameraMatrix);
					SetGameCameraMatrix(pContextInfo->GetCameraMatrix());

					bInLocalContext = false;
				}
				else
				{
					if(bInLocalContext)
						CLogFile::Printf("In local context when we shouldn't be!\n");

					// Restore the local players cam matrix
					SetGameCameraMatrix(&m_matLocalCameraMatrix);

					// Restore the local players keys
					SetGamePadState(&m_localClientPadState);
					IVPad * pGamePad = GetGamePad();
					for(int i = 0; i < INPUT_MAX; i++)
					{
						if(pGamePad->m_padData[i].m_pUnknown)
						{
							pGamePad->m_padData[i].m_pUnknown->m_unknown[pGamePad->m_padData[i].m_byteUnknown8].m_byteUnknown0 = byteValue[i];
							byteValue[i] = 0;
						}
					}

					// Restore the local player id
					CPools::SetLocalPlayerIndex(m_uiLocalPlayerIndex);

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

BYTE GetCurrentGameControlValue(BYTE byteControlId)
{
	CClientPadState padState;
	GetGamePadState(&padState);
	//return (padState.byteKeys1[byteControlId] ^ padState.byteKeys2[byteControlId]);
	return padState.byteCurrentKeys[byteControlId];
}

BYTE GetPreviousGameControlValue(BYTE byteControlId)
{
	CClientPadState padState;
	GetGamePadState(&padState);
	//return (padState.byteKeys1[byteControlId] ^ padState.byteKeys2[byteControlId]);
	return padState.bytePreviousKeys[byteControlId];
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
	dwCTaskSimpleFireGun__SetPedPosition = (g_pClient->GetBaseAddress() + 0xCCA0E0);
	CPatcher::InstallMethodPatch((g_pClient->GetBaseAddress() + 0xDCD8C0), (DWORD)CTaskSimpleFireGun__SetPedPosition_Hook);

	//dwTaskHookFunc = (g_pClient->GetBaseAddress() + 0xCC8140);
	//CPatcher::InstallMethodPatch((g_pClient->GetBaseAddress() + 0xDCD6C0), (DWORD)TaskHookFunc);

	// CTaskComplexPlayerGun::ControlSubTask
	// This might call CTaskComplexGun::ControlSubTask so i'll leave it for now ;)
	//0xD80A3C

	// CTaskComplexPlayIdles::ControlSubTask = 0xC3 to disable idle anims?

	// CPlayerPed::GetPad
	//CPatcher::InstallJmpPatch((g_pClient->GetBaseAddress() + 0x9BE5D0), (DWORD)CPlayerPed__GetPad_Hook);
}

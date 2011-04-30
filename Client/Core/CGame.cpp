//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CGame.cpp
// Project: Client
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//          Contains public domain code from xliveless by listener
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CGame::CGame()
{
	// Initialize the game offsets
	COffsets::Initialize(g_pClient->GetBaseAddress());

	// Make sure we have a valid game version
	if(COffsets::GetVersion() == GAME_VERSION_UNKNOWN)
	{
		MessageBox(NULL, "Unknown game version detected. Networked: IV currently only supports game version 1.0.7.0", "Unknown game version", NULL);
		CLogFile::Printf("Unknown game version detected (N:IV only support 1.0.7.0)!\n");
		ExitProcess(0);
	}

	// Initialize the patches
	CPatches::Initialize();

	// Set the game load callback
	CPatches::SetGameLoadCallback(GameLoadCallback_Static);

	// Set the game process callback
	CPatches::SetGameProcessCallback(GameProcessCallback_Static);

	// Install key sync hooks
	InstallKeySyncHooks();

	// Create the streaming instance
	m_pStreaming = new CStreaming();

	if(!m_pStreaming)
	{
		CLogFile::Printf("Failed to create streaming instance!\n");
		ExitProcess(0);
	}

	// Initialize the model infos
	for(int i = 0; i < NUM_ModelInfos; i++)
		m_modelInfos[i].SetIndex(i);

	// Disable invalid models
	m_modelInfos[125].SetIndex(-1); // Ingot (FIX)
	m_modelInfos[180].SetIndex(-1); // Uranus (FIX)
	m_modelInfos[191].SetIndex(-1); // Hellfury (FIX)
	m_modelInfos[195].SetIndex(-1); // Zombieb (FIX)

	// Initialize the weapon infos
	for(int i = 0; i < NUM_WeaponInfos; i++)
	{
		m_weaponInfos[i].SetWeaponType((eWeaponType)i);
		m_weaponInfos[i].SetWeaponInfo((IVWeaponInfo *)((g_pClient->GetBaseAddress() + ARRAY_WeaponInfos) + (i * sizeof(IVWeaponInfo))));
	}
}

CGame::~CGame()
{
	// Delete the pad instance
	SAFE_DELETE(m_pPad);

	// Shutdown the pools
	CPools::Shutdown();

	// Delete the streaming instance
	SAFE_DELETE(m_pStreaming);

	// Shutdown the patches
	CPatches::Shutdown();
}

CIVModelInfo * CGame::GetModelInfo(int iModelIndex)
{
	if(iModelIndex < NUM_ModelInfos && iModelIndex >= 0 && m_modelInfos[iModelIndex].IsValid())
		return &m_modelInfos[iModelIndex];

	return NULL;
}

CIVWeaponInfo * CGame::GetWeaponInfo(eWeaponType weaponType)
{
	if(weaponType < NUM_WeaponInfos && weaponType >= 0)
		return &m_weaponInfos[weaponType];

	return NULL;
}

void CGame::GameLoadCallback_Static()
{
	g_pClient->GetGame()->GameLoadCallback();
}

void CGame::GameLoadCallback()
{
	// Initialize the pools
	CPools::Init();

	// Create the pad instance
	g_pClient->GetGame()->m_pPad = new CIVPad((IVPad *)COffsets::VAR_Pads);

	// Initialize the model infos

	// Call the CClient game load callback
	g_pClient->OnGameLoad();
}

void CGame::GameProcessCallback_Static()
{
	g_pClient->GetGame()->GameProcessCallback();
}

void CGame::GameProcessCallback()
{
	// Call the CClient game process callback
	g_pClient->OnGameProcess();
}

// NOTE: Find out if the time is unsigned
// TODO: Some sort of 'show loading...' option
void CGame::FadeScreen(eFadeType fadeType, int iTime)
{
	if(fadeType == FADE_TYPE_IN_UNHACKED)
		InvokeNative<void *>(0x5F9218C3, iTime); // DO_SCREEN_FADE_IN_UNHACKED
	else if(fadeType == FADE_TYPE_IN)
		InvokeNative<void *>(0x4D72200, iTime); // DO_SCREEN_FADE_IN
	else if(fadeType == FADE_TYPE_OUT_UNHACKED)
		InvokeNative<void *>(0x42D250A7, iTime); // DO_SCREEN_FADE_OUT_UNHACKED
	else if(fadeType == FADE_TYPE_OUT)
		InvokeNative<void *>(0x65DE621C, iTime); // DO_SCREEN_FADE_OUT
}

void CGame::ConvertRotationMatrixToEulerAngles(Matrix& matRotation, CVector3& vecRotation)
{
	// Couldn't find the games function for this so using this for now
	// From http://www.geometrictools.com/LibFoundation/Mathematics/Wm4Matrix3.inl.html
	float fTest = matRotation.vecRight.fZ;

	if(fTest < 1.0f)
	{
		if(fTest > -1.0f)
		{
			vecRotation = CVector3(atan2(-matRotation.vecFront.fZ, matRotation.vecUp.fZ), asin(matRotation.vecRight.fZ), 
				atan2(-matRotation.vecRight.fY, matRotation.vecRight.fX));
			return/* true*/;
		}

		// WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
		vecRotation = CVector3(-atan2(matRotation.vecFront.fX, matRotation.vecFront.fY), -HALF_PI, 0.0f);
	}
	else
	{
		// WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
		vecRotation = CVector3(atan2(matRotation.vecFront.fX, matRotation.vecFront.fY), HALF_PI, 0.0f);
	}

	return/* false*/;
}

void CGame::ConvertEulerAnglesToRotationMatrix(CVector3& vecRotation, Matrix& matRotation)
{
	Matrix34 matGameRotation;
	matGameRotation.FromMatrix(&matRotation);
	Matrix34 * pMatGameRotation = &matGameRotation;
	CVector3 vecGameRotation;
	CVector3 * pVecGameRotation = &vecGameRotation;
	DWORD dwFunc = COffsets::FUNC_CMatrix__ConvertFromEulerAngles;
	_asm
	{
		push pVecGameRotation
		mov ecx, pMatGameRotation
		call dwFunc
	}
	vecRotation = vecGameRotation;
}

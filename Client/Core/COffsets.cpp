//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: COffsets.cpp
// Project: Client
// Author(s): mabako
//            jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

eGameVersion COffsets::m_GameVersion = GAME_VERSION_UNKNOWN;

unsigned int COffsets::FUNC_GetModelFromHash = 0;
unsigned int COffsets::FUNC_RequestResource = 0;
unsigned int COffsets::VAR_FileTypeWdrIndex = 0;
unsigned int COffsets::FUNC_LoadRequestedResources = 0;
unsigned int COffsets::FUNC_HasResourceLoaded = 0;
unsigned int COffsets::VAR_VehicleFactory = 0;
unsigned int COffsets::FUNC_CVehicleFactory__Create = 0;
unsigned int COffsets::FUNC_CVehicleFactory__Delete = 0;
unsigned int COffsets::FUNC_CMatrix__ConvertFromEulerAngles = 0;
unsigned int COffsets::VAR_Pads = 0;
unsigned int COffsets::VAR_CameraUnknown = 0;
unsigned int COffsets::VAR_CPlayerPed__VFTable = 0;
unsigned int COffsets::VAR_CAutomobile__VFTable = 0;
unsigned int COffsets::VAR_CBike__VFTable = 0;
unsigned int COffsets::VAR_CBoat__VFTable = 0;
unsigned int COffsets::VAR_CTrain__VFTable = 0;
unsigned int COffsets::VAR_CHeli__VFTable = 0;
unsigned int COffsets::VAR_CPlane__VFTable = 0;
unsigned int COffsets::FUNC_CPlayerPed__ProcessInput = 0;
unsigned int COffsets::FUNC_CAutomobile__ProcessInput = 0;
unsigned int COffsets::FUNC_CBike__ProcessInput = 0;
unsigned int COffsets::FUNC_CBoat__ProcessInput = 0;
unsigned int COffsets::FUNC_CTrain__ProcessInput = 0;
unsigned int COffsets::FUNC_CHeli__ProcessInput = 0;
unsigned int COffsets::FUNC_CPlane__ProcessInput = 0;
unsigned int COffsets::FUNC_CVehicle__SetEngineOn = 0;
unsigned int COffsets::FUNC_CVehicle__RefreshColours = 0;
unsigned int COffsets::VAR_NumLoadingScreens = 0;
unsigned int COffsets::VAR_FirstLoadingScreenDuration = 0;
unsigned int COffsets::VAR_FirstLoadingScreenType = 0;

void COffsets::Initialize(unsigned int uiBase)
{
	m_GameVersion = GAME_VERSION_UNKNOWN;
	unsigned int * uiTestAddr = (unsigned int *)(uiBase + 0x608C34);

	switch(*uiTestAddr)
	{
	case 0x1006E857: // 1.0.7.0
		m_GameVersion = GAME_VERSION_7;

		FUNC_GetModelFromHash                = (uiBase + 0x98AAE0);
		FUNC_RequestResource                 = (uiBase + 0x832C40);
		VAR_FileTypeWdrIndex                 = (uiBase + 0x15F73A0);
		FUNC_LoadRequestedResources          = (uiBase + 0x832C20);
		FUNC_HasResourceLoaded               = (uiBase + 0x832DD0);
		VAR_VehicleFactory                   = (uiBase + 0x15F5514);
		FUNC_CVehicleFactory__Create         = (uiBase + 0x443D60);
		FUNC_CVehicleFactory__Delete         = (uiBase + 0x973AA0);
		FUNC_CMatrix__ConvertFromEulerAngles = (uiBase + 0x42F8A0);
		VAR_Pads                             = (uiBase + 0x10FB818);
		VAR_CameraUnknown                    = (uiBase + 0xF21A6C);
		VAR_CPlayerPed__VFTable              = (uiBase + 0xD819C4);
		VAR_CAutomobile__VFTable             = (uiBase + 0xDB3C3C);
		VAR_CBike__VFTable                   = (uiBase + 0xDB4064);
		VAR_CBoat__VFTable                   = (uiBase + 0xDCA0E4);
		VAR_CTrain__VFTable                  = (uiBase + 0xD78D4C);
		VAR_CHeli__VFTable                   = (uiBase + 0xD6B1CC);
		VAR_CPlane__VFTable                  = (uiBase + 0xD86104);
		FUNC_CPlayerPed__ProcessInput        = (uiBase + 0x9C0B00);
		FUNC_CAutomobile__ProcessInput       = (uiBase + 0xBFE870);
		FUNC_CBike__ProcessInput             = (uiBase + 0xC05C30);
		FUNC_CBoat__ProcessInput             = (uiBase + 0xCA8C60);
		FUNC_CTrain__ProcessInput            = (uiBase + 0x94CE70);
		FUNC_CHeli__ProcessInput             = (uiBase + 0x8B9290);
		FUNC_CPlane__ProcessInput            = (uiBase + 0xA1B080);
		FUNC_CVehicle__SetEngineOn           = (uiBase + 0x9D3600);
		FUNC_CVehicle__RefreshColours        = (uiBase + 0x9C52E0);
		VAR_NumLoadingScreens                = (uiBase + 0x18A8258);
		VAR_FirstLoadingScreenDuration       = (uiBase + 0x18A8F40);
		VAR_FirstLoadingScreenType           = (uiBase + 0x18A8F48);
		break;
	}
}

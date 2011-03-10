//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: COffsets.h
// Project: Client
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

enum eGameVersion
{
	GAME_VERSION_UNKNOWN,
	GAME_VERSION_7,
};

class COffsets
{
private:
	static eGameVersion m_GameVersion;

public:
	static void         Initialize(unsigned int uiBase);
	static eGameVersion GetVersion() { return m_GameVersion; }

	static unsigned int FUNC_GetModelFromHash;
	static unsigned int FUNC_RequestResource;
	static unsigned int VAR_FileTypeWdrIndex;
	static unsigned int FUNC_LoadRequestedResources;
	static unsigned int FUNC_HasResourceLoaded;
	static unsigned int VAR_VehicleFactory;
	static unsigned int FUNC_CVehicleFactory__Create;
	static unsigned int FUNC_CVehicleFactory__Delete;
	static unsigned int FUNC_CMatrix__ConvertFromEulerAngles;
	static unsigned int VAR_Pads;
	static unsigned int VAR_CameraUnknown;
	static unsigned int VAR_CPlayerPed__VFTable;
	static unsigned int VAR_CAutomobile__VFTable;
	static unsigned int VAR_CBike__VFTable;
	static unsigned int VAR_CBoat__VFTable;
	static unsigned int VAR_CTrain__VFTable;
	static unsigned int VAR_CHeli__VFTable;
	static unsigned int VAR_CPlane__VFTable;
	static unsigned int FUNC_CPlayerPed__ProcessInput;
	static unsigned int FUNC_CAutomobile__ProcessInput;
	static unsigned int FUNC_CBike__ProcessInput;
	static unsigned int FUNC_CBoat__ProcessInput;
	static unsigned int FUNC_CTrain__ProcessInput;
	static unsigned int FUNC_CHeli__ProcessInput;
	static unsigned int FUNC_CPlane__ProcessInput;
	static unsigned int FUNC_CVehicle__SetEngineOn;
	static unsigned int FUNC_CVehicle__RefreshColours;
	static unsigned int VAR_NumLoadingScreens;
	static unsigned int VAR_FirstLoadingScreenDuration;
	static unsigned int VAR_FirstLoadingScreenType;
};

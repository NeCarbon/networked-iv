//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CGame.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define MODEL_PLAYER 0x6F0783F5

#define NATIVE_SET_CHAR_HEALTH 0x575E2880
#define NATIVE_GET_CHAR_HEALTH 0x4B6C2256
#define NATIVE_CHANGE_PLAYER_MODEL 0x232F1A85
#define NATIVE_SET_CHAR_DEFAULT_COMPONENT_VARIATION 0x4FB30DB6
#define NATIVE_SET_CHAR_COORDINATES 0x689D0F5F
#define NATIVE_GIVE_WEAPON_TO_CHAR 0x3E90416
#define NATIVE_REMOVE_WEAPON_FROM_CHAR 0x2485231E
#define NATIVE_REMOVE_ALL_CHAR_WEAPONS 0x6BA520F0
#define NATIVE_SET_CURRENT_CHAR_WEAPON 0x6CF44DD6
#define NATIVE_GET_CURRENT_CHAR_WEAPON 0x5AB8289F
#define NATIVE_SET_CHAR_AMMO 0x437D247E
#define NATIVE_GET_AMMO_IN_CHAR_WEAPON 0x23E140A9
#define NATIVE_CREATE_CAR 0x2F1D6843
#define NATIVE_DELETE_CAR 0x7F71342D
//#define NATIVE_SET_CAR_HEALTH 0x49B6525C
#define NATIVE_SET_ENGINE_HEALTH 0x3F413561
//#define NATIVE_GET_CAR_HEALTH 0x4D417CD3
#define NATIVE_GET_ENGINE_HEALTH 0x2B0A05E0
#define NATIVE_SET_CAR_COORDINATES_NO_OFFSET 0x12D64378
#define NATIVE_WARP_CHAR_INTO_CAR 0x73D3504A
#define NATIVE_WARP_CHAR_INTO_CAR_AS_PASSENGER 0x172376FE

enum eResourceType
{
	RESOURCE_TYPE_WDR
};

enum eFadeType
{
	FADE_TYPE_IN_UNHACKED,
	FADE_TYPE_IN,
	FADE_TYPE_OUT_UNHACKED,
	FADE_TYPE_OUT
};

class CGame
{
public:
	CGame();
	~CGame();

	void         ApplyPatches();
	unsigned int GetResourceTypeIndex(eResourceType fileType);
	int          GetModelIndexFromHash(DWORD dwModelHash);
	int          RequestModel(DWORD dwModelHash);
	void         LoadRequestedModels();
	bool         HasModelLoaded(DWORD dwModelHash);
	void         UnloadModel(DWORD dwModelHash);
	int          LoadModel(DWORD dwModelHash);
	void         FadeScreen(eFadeType fadeType, int iTime);
	void         ConvertRotationMatrixToEulerAngles(Matrix * matRotation, Vector3 * vecRotation);
	void         ConvertEulerAnglesToRotationMatrix(Vector3 * vecRotation, Matrix * matRotation);
};

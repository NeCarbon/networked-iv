//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPatches.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

typedef void (* GameLoadCallback_t)();
typedef void (* GameProcessCallback_t)();

class CPatches
{
public:
	static void Initialize();
	static void Shutdown();

	static void SetGameLoadCallback(GameLoadCallback_t pfnGameLoadCallback);
	static void SetGameProcessCallback(GameProcessCallback_t pfnGameProcessCallback);
};

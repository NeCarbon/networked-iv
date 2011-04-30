//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CStreaming.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

enum eResourceType
{
	RESOURCE_TYPE_WDR, // Model
	RESOURCE_TYPE_WAD // Animation
};

class CStreaming
{
public:
	unsigned int      GetResourceTypeIndex(eResourceType fileType);
	int               GetModelIndexFromHash(DWORD dwModelHash);
	void              RequestModel(int iModelIndex, DWORD dwFlags = 0x1A);
	void              LoadRequestedModels();
	bool              HasModelLoaded(int iModelIndex);
	void              UnloadModel(DWORD dwModelHash);
};

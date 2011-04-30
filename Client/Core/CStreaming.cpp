//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CStreaming.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

unsigned int CStreaming::GetResourceTypeIndex(eResourceType resourceType)
{
	switch(resourceType)
	{
	case RESOURCE_TYPE_WDR:
		return *(unsigned int *)(COffsets::VAR_FileTypeWdrIndex);
	}

	return 0;
}

int CStreaming::GetModelIndexFromHash(DWORD dwModelHash)
{
	DWORD dwFunc = COffsets::FUNC_GetModelFromHash;
	int iModelIndex = -1;
	int * pModelIndex = &iModelIndex;
	_asm
	{
		push pModelIndex
		push dwModelHash
		call dwFunc
		add esp, 8
	}
	return iModelIndex;
}

void CStreaming::RequestModel(int iModelIndex, DWORD dwFlags)
{
	DWORD dwFunc = COffsets::FUNC_RequestResource;
	unsigned int uiFileTypeIndex = GetResourceTypeIndex(RESOURCE_TYPE_WDR);
	_asm
	{
		push dwFlags
		push uiFileTypeIndex
		push iModelIndex
		call dwFunc
		add esp, 0Ch
	}
	// TODO: Do something with return value?
}

void CStreaming::LoadRequestedModels()
{
	DWORD dwFunc = COffsets::FUNC_LoadRequestedResources;
	_asm
	{
		push 1
		call dwFunc
		add esp, 4
	}
}

bool CStreaming::HasModelLoaded(int iModelIndex)
{
	DWORD dwFunc = COffsets::FUNC_HasResourceLoaded;
	unsigned int uiFileTypeIndex = GetResourceTypeIndex(RESOURCE_TYPE_WDR);
	bool bLoaded = false;
	_asm
	{
		push uiFileTypeIndex
		push iModelIndex
		call dwFunc
		add esp, 8
		mov bLoaded, al
	}
	return bLoaded;
}

void CStreaming::UnloadModel(DWORD dwModelHash)
{
	// TODO
}

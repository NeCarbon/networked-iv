//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVModelInfo.cpp
// Project: Client
// Author(s): jenksta
//            Multi Theft Auto Team
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CIVModelInfo::CIVModelInfo()
{
	m_iModelIndex = -1;
}

CIVModelInfo::CIVModelInfo(int iModelIndex)
{
	SetIndex(iModelIndex);
}

CIVModelInfo::~CIVModelInfo()
{
	
}

IVBaseModelInfo * CIVModelInfo::GetModelInfo()
{
	if(m_iModelIndex >= 0 && m_iModelIndex < NUM_ModelInfos)
		return *(IVBaseModelInfo **)((g_pClient->GetBaseAddress() + ARRAY_ModelInfos) + (m_iModelIndex * 4));

	return NULL;
}

void CIVModelInfo::SetIndex(int iModelIndex)
{
	m_iModelIndex = iModelIndex;
}

BYTE CIVModelInfo::GetType()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
	{
		DWORD dwFunc = pModelInfo->m_VFTable->GetType;
		BYTE byteType = 0;
		_asm
		{
			mov ecx, pModelInfo
			call dwFunc
			mov byteType, al
		}
		return byteType;
	}

	return 0;
}

bool CIVModelInfo::IsLoaded()
{
	if(IsValid())
		return g_pClient->GetGame()->GetStreaming()->HasModelLoaded(m_iModelIndex);

	return false;
}

void CIVModelInfo::Load(bool bWaitForLoad)
{
	// Is the model invalid?
	if(!IsValid())
		return;

	// Are we already loaded?
	if(IsLoaded())
		return;

	CLogFile::Printf("Reference count before load is %d", GetReferenceCount());

	// Request the model
	g_pClient->GetGame()->GetStreaming()->RequestModel(m_iModelIndex);

	CLogFile::Printf("Reference count after request is %d", GetReferenceCount());

	// Do we need to wait for the model load?
	if(bWaitForLoad)
	{
		// Load all requested models
		g_pClient->GetGame()->GetStreaming()->LoadRequestedModels();

		CLogFile::Printf("Reference count before load is %d", GetReferenceCount());

		// Loop until the model is loaded
		while(!IsLoaded())
			Sleep(10);
	}

	CLogFile::Printf("Reference count after all is %d", GetReferenceCount());
}

void CIVModelInfo::Unload()
{
	// Is the model invalid?
	if(!IsValid())
		return;

	// Are we not already loaded?
	if(!IsLoaded())
		return;

	// TODO:
}

DWORD CIVModelInfo::GetHash()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
		return pModelInfo->dwHash;

	return NULL;
}

void CIVModelInfo::AddReference()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
		pModelInfo->dwReferenceCount++;
}

void CIVModelInfo::RemoveReference()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
		pModelInfo->dwReferenceCount--;
}

DWORD CIVModelInfo::GetReferenceCount()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
		return pModelInfo->dwReferenceCount;

	return NULL;
}

WORD CIVModelInfo::GetAnimIndex()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(pModelInfo)
		return pModelInfo->wAnimIndex;

	return NULL;
}

bool CIVModelInfo::IsAutomobile()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_AUTOMOBILE);

	return false;
}

bool CIVModelInfo::IsBike()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_BIKE);

	return false;
}

bool CIVModelInfo::IsBoat()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_BOAT);

	return false;
}

bool CIVModelInfo::IsTrain()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_TRAIN);

	return false;
}

bool CIVModelInfo::IsHeli()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_HELI);

	return false;
}

bool CIVModelInfo::IsPlane()
{
	IVBaseModelInfo * pModelInfo = GetModelInfo();

	if(IsVehicle() && pModelInfo)
		return (pModelInfo->dwVehicleType == VEHICLE_TYPE_PLANE);

	return false;
}

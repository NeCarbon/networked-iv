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
	m_pModelInfo = NULL;
	m_iModelIndex = -1;
}

CIVModelInfo::CIVModelInfo(int iModelIndex)
{
	SetIndex(iModelIndex);
}

CIVModelInfo::~CIVModelInfo()
{
	
}

void CIVModelInfo::SetIndex(int iModelIndex)
{
	m_pModelInfo = g_pClient->GetGame()->GetModelInfoFromIndex(iModelIndex);
	m_iModelIndex = iModelIndex;
}

bool CIVModelInfo::IsValid()
{
	return (g_pClient->GetGame()->GetModelInfoFromIndex(m_iModelIndex) != NULL);
}

bool CIVModelInfo::IsLoaded()
{
	return g_pClient->GetGame()->HasModelLoaded(m_iModelIndex);
}

void CIVModelInfo::Load(bool bWaitForLoad)
{
	// Are we already loaded?
	if(IsLoaded())
		return;

	// Request the model
	g_pClient->GetGame()->RequestModel(m_iModelIndex);

	// Do we need to wait for the model load?
	if(bWaitForLoad)
	{
		// Load all requested models
		g_pClient->GetGame()->LoadRequestedModels();

		// Loop until the model is loaded
		while(!IsLoaded())
			Sleep(5);
	}
}

void CIVModelInfo::Unload()
{
	// Are we not already loaded?
	if(!IsLoaded())
		return;

	// TODO:
}

DWORD CIVModelInfo::GetHash()
{
	if(m_pModelInfo)
		return m_pModelInfo->dwHash;

	return NULL;
}

WORD CIVModelInfo::GetAnimIndex()
{
	if(m_pModelInfo)
		return m_pModelInfo->wAnimIndex;

	return NULL;
}

bool CIVModelInfo::IsAutomobile()
{
	if(m_pModelInfo)
		return (m_pModelInfo->dwVehicleType == VEHICLE_TYPE_AUTOMOBILE);

	return false;
}

bool CIVModelInfo::IsBike()
{
	if(m_pModelInfo)
		return (m_pModelInfo->dwVehicleType == VEHICLE_TYPE_BIKE);

	return false;
}

bool CIVModelInfo::IsBoat()
{
	if(m_pModelInfo)
		return (m_pModelInfo->dwVehicleType == VEHICLE_TYPE_BOAT);

	return false;
}

bool CIVModelInfo::IsTrain()
{
	if(m_pModelInfo)
		return (m_pModelInfo->dwVehicleType == VEHICLE_TYPE_TRAIN);

	return false;
}

bool CIVModelInfo::IsHeli()
{
	if(m_pModelInfo)
		return (m_pModelInfo->dwVehicleType == VEHICLE_TYPE_HELI);

	return false;
}

bool CIVModelInfo::IsPlane()
{
	if(m_pModelInfo)
		return (m_pModelInfo->dwVehicleType == VEHICLE_TYPE_PLANE);

	return false;
}

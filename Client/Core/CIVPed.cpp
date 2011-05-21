//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPed.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CIVPed::CIVPed() : CIVPhysical()
{
	m_pPedTaskManager = NULL;
}

CIVPed::CIVPed(IVPed * pPed) : CIVPhysical(pPed)
{
	m_pPedTaskManager = NULL;
	SetPed(pPed);
}

CIVPed::~CIVPed()
{
	CIVPhysical::~CIVPhysical();
}

void CIVPed::SetPed(IVPed * pPed)
{
	SetPhysical(pPed);

	// Delete the ped task manager if we have one
	if(m_pPedTaskManager)
		delete m_pPedTaskManager;

	// Do we have a valid ped pointer?
	if(pPed)
	{
		// Create the ped task manager
		m_pPedTaskManager = new CIVPedTaskManager((IVPedTaskManager *)&(pPed->m_pPedIntelligence->m_pPedTaskManager), this);
	}
	else
	{
		// Set the ped task manager
		m_pPedTaskManager = NULL;
	}
}

IVPed * CIVPed::GetPed()
{
	return (IVPed *)GetPhysical();
}

void CIVPed::SetType(ePedType type)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_pPedBase->m_type = type;
}

ePedType CIVPed::GetType()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_pPedBase->m_type;

	return PED_TYPE_PLAYER;
}

void CIVPed::SetPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_pPlayerInfo = pPlayerInfo;
}

IVPlayerInfo * CIVPed::GetPlayerInfo()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_pPlayerInfo;

	return NULL;
}

bool CIVPed::IsInVehicle()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return ((pPed->m_byteUnknown & 4) != 0);

	return false;
}

void CIVPed::SetCurrentHeading(float fCurrentHeading)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_fCurrentHeading = fCurrentHeading;
}

float CIVPed::GetCurrentHeading()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_fCurrentHeading;

	return false;
}

void CIVPed::SetDesiredHeading(float fDesiredHeading)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_fDesiredHeading = fDesiredHeading;
}

float CIVPed::GetDesiredHeading()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_fDesiredHeading;

	return false;
}

void CIVPed::SetCurrentVehicle(IVVehicle * pVehicle)
{
	IVPed * pPed = GetPed();

	if(pPed)
		pPed->m_pCurrentVehicle = pVehicle;
}

IVVehicle * CIVPed::GetCurrentVehicle()
{
	IVPed * pPed = GetPed();

	if(pPed)
		return pPed->m_pCurrentVehicle;

	return NULL;
}

void CIVPed::SetDucking(bool bDucking, int iUnknown)
{
	IVPed * pPed = GetPed();

	if(pPed)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPed__SetDucking);
		_asm
		{
			push iUnknown
			push bDucking
			mov ecx, pPed
			call dwFunc
		}
	}
}

bool CIVPed::IsDucking()
{
	IVPed * pPed = GetPed();

	if(pPed)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPed__IsDucking);
		bool bDucking = false;
		_asm
		{
			mov ecx, pPed
			call dwFunc
			mov bDucking, al
		}
		return bDucking;
	}

	return false;
}

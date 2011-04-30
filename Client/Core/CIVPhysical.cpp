//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPhysical.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CIVPhysical::CIVPhysical() : CIVDynamicEntity()
{

}

CIVPhysical::CIVPhysical(IVPhysical * pPhysical) : CIVDynamicEntity(pPhysical)
{
	
}

CIVPhysical::~CIVPhysical()
{
	CIVDynamicEntity::~CIVDynamicEntity();
}

void CIVPhysical::SetPhysical(IVPhysical * pPhysical)
{
	SetDynamicEntity(pPhysical);
}

IVPhysical * CIVPhysical::GetPhysical()
{
	return (IVPhysical *)GetDynamicEntity();
}

void CIVPhysical::SetMoveSpeed(CVector3 * vecMoveSpeed)
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPhysical__SetMoveSpeed_7);
		_asm
		{
			push vecMoveSpeed
			mov ecx, pPhysical
			call dwFunc
		}
	}
}

void CIVPhysical::GetMoveSpeed(CVector3 * vecMoveSpeed)
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPhysical__GetMoveSpeed_7);
		_asm
		{
			push vecMoveSpeed
			mov ecx, pPhysical
			call dwFunc
		}
	}
}

void CIVPhysical::SetTurnSpeed(CVector3 * vecTurnSpeed)
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPhysical__SetTurnSpeed_7);
		_asm
		{
			push vecTurnSpeed
			mov ecx, pPhysical
			call dwFunc
		}
	}
}

void CIVPhysical::GetTurnSpeed(CVector3 * vecTurnSpeed)
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();
	
	if(pPhysical)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CPhysical__GetTurnSpeed_7);
		_asm
		{
			push vecTurnSpeed
			mov ecx, pPhysical
			call dwFunc
		}
	}
}

void CIVPhysical::SetLastDamageEntity(IVEntity * pLastDamageEntity)
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
		pPhysical->m_pLastDamageEntity = pLastDamageEntity;
}

IVEntity * CIVPhysical::GetLastDamageEntity()
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
		return pPhysical->m_pLastDamageEntity;

	return NULL;
}

void CIVPhysical::SetHealth(float fHealth)
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();

	if(pPhysical)
	{
		DWORD dwFunc = ((IVPhysicalVFTable *)pPhysical->m_VFTable)->SetHealth;
		_asm
		{
			push 1
			push fHealth
			mov ecx, pPhysical
			call dwFunc
		}
	}
}

float CIVPhysical::GetHealth()
{
	IVPhysical * pPhysical = (IVPhysical *)GetEntity();
	float fHealth = 0.0f;

	if(pPhysical)
	{
		DWORD dwFunc = ((IVPhysicalVFTable *)pPhysical->m_VFTable)->GetHealth;
		_asm
		{
			mov ecx, pPhysical
			call dwFunc
			mov fHealth, eax
		}
	}

	return fHealth;
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVVehicle.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CIVVehicle::CIVVehicle() : CIVPhysical()
{

}

CIVVehicle::CIVVehicle(IVVehicle * pVehicle) : CIVPhysical(pVehicle)
{
	
}

CIVVehicle::~CIVVehicle()
{
	CIVPhysical::~CIVPhysical();
}

void CIVVehicle::SetVehicle(IVVehicle * pVehicle)
{
	SetPhysical(pVehicle);
}

IVVehicle * CIVVehicle::GetVehicle()
{
	return (IVVehicle *)GetPhysical();
}

void CIVVehicle::SetAlarmDuration(WORD wDuration)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_wAlarm = wDuration;
}

WORD CIVVehicle::GetAlarmDuration()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_wAlarm;

	return 0;
}

void CIVVehicle::SetDriver(IVPed * pPed)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_pDriver = pPed;
}

IVPed * CIVVehicle::GetDriver()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_pDriver;

	return NULL;
}

void CIVVehicle::SetPassenger(BYTE bytePassengerSeatId, IVPed * pPassenger)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bytePassengerSeatId < 8)
			pVehicle->m_pPassengers[bytePassengerSeatId] = pPassenger;
	}
}

IVPed * CIVVehicle::GetPassenger(BYTE bytePassengerSeatId)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bytePassengerSeatId < 8)
			return pVehicle->m_pPassengers[bytePassengerSeatId];
	}

	return NULL;
}

void CIVVehicle::SetColors(unsigned char ucColor1, unsigned char ucColor2, unsigned char ucColor3, unsigned char ucColor4)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		pVehicle->m_byteColors[0] = ucColor1;
		pVehicle->m_byteColors[1] = ucColor2;
		pVehicle->m_byteColors[2] = ucColor3;
		pVehicle->m_byteColors[3] = ucColor4;

		DWORD dwFunc = COffsets::FUNC_CVehicle__RefreshColours;
		_asm
		{
			mov ecx, pVehicle
			call dwFunc
		}
	}
}
void CIVVehicle::GetColors(unsigned char * ucColor1, unsigned char * ucColor2, unsigned char * ucColor3, unsigned char * ucColor4)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		*ucColor1 = pVehicle->m_byteColors[0];
		*ucColor2 = pVehicle->m_byteColors[1];
		*ucColor3 = pVehicle->m_byteColors[2];
		*ucColor4 = pVehicle->m_byteColors[3];
	}
}

void CIVVehicle::SetMaxPassengers(BYTE byteMaxPassengers)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_byteMaxPassengers = byteMaxPassengers;
}

BYTE CIVVehicle::GetMaxPasssengers()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_byteMaxPassengers;

	return 0;
}

void CIVVehicle::SetSirenState(bool bStatus)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bStatus)
		{
			if(*(BYTE *)(pVehicle + 0xF6E) & 0x20)
				*(BYTE *)(pVehicle + 0xF69) = *(BYTE *)(pVehicle + 0xF69) & 0x9F | 0x10;
		}
		else
			*(BYTE *)(pVehicle + 0xF69) &= 0x8Fu;
	}
}

bool CIVVehicle::GetSirenState()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		char v2;
		bool result;
		char v4;

		int v1 = (int)(pVehicle + 0xF64);
		if(!(*(BYTE *)(v1 + 0xA) & 0x20) || (v2 = *(BYTE *)(v1 + 5), !(v2 & 0x10)) 
			|| (v4 = (v2 & 0x20) == 0, result = 1, !v4))
			result = 0;

		return result;
	}

	return false;
}

void CIVVehicle::SetEngineHealth(float fEngineHealth)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fEngineHealth = fEngineHealth;
}

float CIVVehicle::GetEngineHealth()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fEngineHealth;

	return 0.0f;
}

void CIVVehicle::SetDirtLevel(float fDirtLevel)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fDirtLevel = fDirtLevel;
}

float CIVVehicle::GetDirtLevel()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fDirtLevel;

	return 0.0f;
}

void CIVVehicle::SetPetrolTankHealth(float fPetrolTankHealth)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fPetrolTankHealth = fPetrolTankHealth;
}

float CIVVehicle::GetPetrolTankHealth()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fPetrolTankHealth;

	return 0.0f;
}

void CIVVehicle::SetDoorLockState(DWORD dwDoorLockState)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_dwDoorLockState = dwDoorLockState;
}

DWORD CIVVehicle::GetDoorLockState()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_dwDoorLockState;

	return 0;
}

void CIVVehicle::SetEngineStatus(bool bStatus, bool bUnknown)
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bStatus)
		{
			DWORD dwFunc = COffsets::FUNC_CVehicle__SetEngineOn;
			_asm
			{
				push bUnknown
				mov ecx, pVehicle
				call dwFunc
			}
		}
		else
		{
			*(BYTE *)(pVehicle + 0xF64) &= 0xE7;
			*(BYTE *)(pVehicle + 0x1344) = 0;
		}
	}
}

bool CIVVehicle::GetEngineStatus()
{
	// Do we have a valid vehicle pointer?
	IVVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		// TODO
		return false;

	return false;
}

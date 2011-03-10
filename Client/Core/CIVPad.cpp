//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVPad.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CIVPad::CIVPad()
{
	m_pPad = NULL;
}

CIVPad::CIVPad(IVPad * pPad)
{
	m_pPad = pPad;
}

CIVPad::~CIVPad()
{

}

void CIVPad::SetPad(IVPad * pPad)
{
	m_pPad = pPad;
}

IVPad * CIVPad::GetPad()
{
	return m_pPad;
}

void CIVPad::SetIsUsingController(bool bIsUsingController)
{
	if(m_pPad)
		m_pPad->m_bIsUsingController = bIsUsingController;
}

bool CIVPad::GetIsUsingController()
{
	if(m_pPad)
		return m_pPad->m_bIsUsingController;

	return false;
}

void CIVPad::SetLastUpdateTime(DWORD dwLastUpdateTime)
{
	if(m_pPad)
		m_pPad->m_dwLastUpdateTime = dwLastUpdateTime;
}

DWORD CIVPad::GetLastUpdateTime()
{
	if(m_pPad)
		return m_pPad->m_dwLastUpdateTime;

	return 0;
}

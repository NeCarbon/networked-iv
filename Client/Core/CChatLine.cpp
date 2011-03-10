//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CChatLine.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CChatLine::CChatLine(CFont * pFont)
{
	m_pFont = pFont;
	Reset();
}

CChatLine::~CChatLine()
{
	
}

void CChatLine::Reset()
{
	m_bActive = false;
	m_dwColor = 0;
	memset(m_szText, 0, CHAT_LINE_LEN);
}

void CChatLine::Draw(float fPosX, float fPosY)
{
	// Are we active?
	if(m_bActive)
	{
		// Draw our tex
		m_pFont->Draw(m_szText, 0xFF000000, fPosX-1, fPosY);
		m_pFont->Draw(m_szText, 0xFF000000, fPosX+1, fPosY);
		m_pFont->Draw(m_szText, 0xFF000000, fPosX, fPosY-1);
		m_pFont->Draw(m_szText, 0xFF000000, fPosX, fPosY+1);
		m_pFont->Draw(m_szText, m_dwColor, fPosX, fPosY);
	}
}

void CChatLine::SetActive(bool bActive)
{
	m_bActive = bActive;
}

bool CChatLine::IsActive()
{
	return m_bActive;
}

void CChatLine::SetColor(DWORD dwColor)
{
	m_dwColor = dwColor;
}

DWORD CChatLine::GetColor()
{
	return m_dwColor;
}

void CChatLine::SetText(char * szText)
{
	strcpy(m_szText, szText);
}

char * CChatLine::GetText()
{
	return m_szText;
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CChatLine.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define CHAT_LINE_LEN 128

class CChatLine
{
private:
	bool    m_bActive;
	CFont * m_pFont;
	DWORD   m_dwColor;
	char    m_szText[CHAT_LINE_LEN + 1];

public:
	CChatLine(CFont * pFont);
	~CChatLine();

	void   Reset();
	void   Draw(float fPosX, float fPosY);
	void   SetActive(bool bActive);
	bool   IsActive();
	void   SetColor(DWORD dwColor);
	DWORD  GetColor();
	void   SetText(char * szText);
	char * GetText();
};

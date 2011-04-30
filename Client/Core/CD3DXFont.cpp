//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CD3DXFont.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CD3DXFont::CD3DXFont(IDirect3DDevice9 * pD3DDevice, int iHeight, int iWidth, int iWeight, char * szFontName)
{
	m_pFont = NULL;
	m_fHeight = (float)iHeight;
	// width = 0
	// weight = FW_BOLD
	// (Device, Height, Width, Weight, MipLevels (0 = autogen mipmaps), Italic, CharSet, OutputPrecision, 
	// Quality, PitchAndFamily, pFaceName, ppFont)
	// Create the font
	D3DXCreateFont(pD3DDevice, iHeight, iWidth, iWeight, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE), szFontName, &m_pFont);
}

CD3DXFont::~CD3DXFont()
{

}

void CD3DXFont::Draw(const char * szText, DWORD dwColor, float fPosX, float fPosY)
{
	// Does the font exist?
	if(m_pFont)
	{
		// Create the position rect
		RECT rPosition;
		SetRect(&rPosition, (LONG)fPosX, (LONG)fPosY, 0, 0);

		// Draw the text
		m_pFont->DrawTextA(NULL, szText, -1, &rPosition, DT_NOCLIP, dwColor);
	}
}

float CD3DXFont::GetHeight()
{
	return m_fHeight;
}

float CD3DXFont::GetTextExtent(const char * szText)
{
	// Does the font exist?
	if(m_pFont)
	{
		RECT rPosition;
		m_pFont->DrawText(NULL, szText, -1, &rPosition, DT_CALCRECT, 0xFF000000);
		return (float)(rPosition.right - rPosition.left);
	}

	return 0.0f;
}

void CD3DXFont::OnDeviceLost()
{
	// Does the font exist?
	if(m_pFont)
		m_pFont->OnLostDevice();
}

void CD3DXFont::OnDeviceReset()
{
	// Does the font exist?
	if(m_pFont)
		m_pFont->OnResetDevice();
}

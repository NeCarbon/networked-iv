//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CD3DXFont.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CD3DXFont : public CFont
{
private:
	ID3DXFont * m_pFont;
	float       m_fHeight;

public:
	CD3DXFont(IDirect3DDevice9 * pD3DDevice, int iHeight, int iWidth, int iWeight, char * szFontName);
	~CD3DXFont();

	void  Draw(char * szText, DWORD dwColor, float fPosX, float fPosY);
	float GetHeight();
	float GetTextExtent(char * szText);
	void  OnDeviceLost();
	void  OnDeviceReset();
};

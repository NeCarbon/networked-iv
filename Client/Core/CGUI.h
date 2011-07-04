//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CGUI.h
// Project: Client
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include "StdInc.h"

class CGUI
{
public:
	 CGUI(IDirect3DDevice9* pDevice);
	~CGUI();

	void Render();

	void SetScreenSize(int iWidth, int iHeight);

private:
	// DX-based renderer
	Gwen::Renderer::DirectX9* m_pRenderer;

	// Skin
	Gwen::Skin::Simple skin;

	// For the IV:MP Menu
	Gwen::Controls::Canvas* m_pClientBase;

	// For the server's custom menus
	Gwen::Controls::Canvas* m_pCustomBase;
};

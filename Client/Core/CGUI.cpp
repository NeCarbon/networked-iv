//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CGUI.cpp
// Project: Client
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include "StdInc.h"

using namespace Gwen;

CGUI::CGUI(IDirect3DDevice9* pDevice)
{
	// Create the renderer
	m_pRenderer = new Renderer::DirectX9( pDevice );

	// Set the skin
	// TODO: Should use the image-based variation
	skin.SetRender( m_pRenderer );

	// Create the root for the IV:MP Menu
	m_pClientBase = new Controls::Canvas( &skin );
	
	// Default screen size
	SetScreenSize(1024, 768);

	// Example code
	Controls::WindowControl* pWindow = new Controls::WindowControl( m_pClientBase );
	pWindow->SetTitle("Important Information");
	pWindow->SetSize(200, 100);
	pWindow->SetPos(500, 500);
	pWindow->SetClosable(false);
}

CGUI::~CGUI()
{
	SAFE_DELETE(m_pClientBase);
	SAFE_DELETE(m_pRenderer);
}

void CGUI::Render()
{
	m_pClientBase->RenderCanvas();
}

void CGUI::SetScreenSize(int iWidth, int iHeight)
{
	CLogFile::Printf("Changing GUI resolution to %dx%d", iWidth, iHeight);
	m_pClientBase->SetSize(iWidth, iHeight);
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CGUIView.h
// Project: Client
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include "StdInc.h"

class CGUIView
{
public:
	 CGUIView(Gwen::Renderer::DirectX9* pRenderer);
	~CGUIView();

	void Render();
	bool ProcessInput(MSG msg);

	void SetScreenSize(int iWidth, int iHeight);

	Gwen::Controls::Canvas* GetCanvas() { return m_pCanvas; }

private:
	Gwen::Renderer::DirectX9* m_pRenderer;

	// The skin
	Gwen::Skin::Simple skin;

	// Base Canvas to draw on
	Gwen::Controls::Canvas* m_pCanvas;

	// Input handler
	Gwen::Input::Windows* m_pInput;
};

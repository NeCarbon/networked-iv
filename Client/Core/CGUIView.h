//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CGUIView.h
// Project: Client
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include "StdInc.h"

#define MOUSE_DEBUG 1

class CGUIView
{
public:
	 CGUIView(Gwen::Renderer::DirectX9* pRenderer);
	~CGUIView();

	void Render();
	bool ProcessInput(UINT message, LPARAM lParam, WPARAM wParam);

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

#ifdef MOUSE_DEBUG
	// Button that pretends to be a mouse in lack of an image for it
	Gwen::Controls::Button* m_pHelper;
#endif
};

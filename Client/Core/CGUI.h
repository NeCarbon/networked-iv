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
	// Different GUI modes
	enum eGUIView { GUI_IVMP = 0, GUI_SERVER = 1, GUI_NONE = 2 };

	 CGUI(IDirect3DDevice9* pDevice);
	~CGUI();

	void Render();
	bool ProcessInput(UINT message, LPARAM lParam, WPARAM wParam);

	void SetScreenSize(int iWidth, int iHeight);
	void GetScreenSize(int* iWidth, int* iHeight);

	eGUIView GetView();
	void SetView(eGUIView view);
	void ClearView(eGUIView view);

	Gwen::Controls::Canvas* GetCanvas(eGUIView view);
private:
	// DX-based renderer
	Gwen::Renderer::DirectX9* m_pRenderer;

	CGUIView* m_pActiveView;
	CGUIView* m_pViews[GUI_NONE];

	// Screen size
	int m_iScreenWidth;
	int m_iScreenHeight;
};

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
	m_pActiveView = NULL;
	memset(m_pViews, 0, sizeof(m_pViews));

	// Create the renderer
	m_pRenderer = new Renderer::DirectX9( pDevice );

	// Set the default screen size
	m_iScreenWidth = 1024;
	m_iScreenHeight = 768;

	// Clear all contents
	ClearView(GUI_IVMP);
	ClearView(GUI_SERVER);

	// Create the main menu
	new CMainMenu(GetCanvas(GUI_IVMP));

	// Finally, set the view
	// TODO: Should be GUI_NONE unless the main menu shows
	SetView(GUI_IVMP);
}

CGUI::~CGUI()
{
	// Deletes all views
	for(int i = 0; i < GUI_NONE; ++ i)
		SAFE_DELETE(m_pViews[i]);
}

void CGUI::Render()
{
	// Do we have an active view?
	if(m_pActiveView)
	{
		// Render its content
		m_pActiveView->Render();
	}
}

bool CGUI::ProcessInput(UINT message, LPARAM lParam, WPARAM wParam)
{
	// Do we have an active view?
	if(m_pActiveView)
	{
		// Pass the message
		return m_pActiveView->ProcessInput(message, lParam, wParam);
	}

	return false;
}

CGUI::eGUIView CGUI::GetView()
{
	// No view - always none
	if(!m_pActiveView)
		return GUI_NONE;

	// Loop through views
	for(int i = 0; i < GUI_NONE; ++ i)
		if(m_pViews[i] == m_pActiveView)
			return (eGUIView)i;
	return GUI_NONE;
}

void CGUI::SetView(eGUIView view)
{
	CLogFile::Printf("GUI - Active view %d -> %d", GetView(), view);
	if(view < GUI_NONE)
		m_pActiveView = m_pViews[view];
	else
		m_pActiveView = NULL;
}

void CGUI::ClearView(eGUIView view)
{
	// Ignore it if we try to clear a non-existant view
	if(view >= GUI_NONE)
		return;

	CLogFile::Printf("Clearing view %d", view);

	// Set the GUI to show nothing if this view is deleted
	if(m_pActiveView && m_pActiveView == m_pViews[view])
		SetView(GUI_NONE);

	// Delete the old view
	SAFE_DELETE(m_pViews[view]);

	// Create a new view
	CGUIView* pView = new CGUIView(m_pRenderer);

	// Update the screen size
	pView->SetScreenSize(m_iScreenWidth, m_iScreenHeight);

	// Store the view
	m_pViews[view] = pView;
}

void CGUI::SetScreenSize(int iWidth, int iHeight)
{
	// Update all views
	for(int i = 0; i < GUI_NONE; ++ i)
		m_pViews[i]->SetScreenSize(iWidth, iHeight);

	// Store the resolution
	m_iScreenWidth = iWidth;
	m_iScreenHeight = iHeight;
}

void CGUI::GetScreenSize(int* iWidth, int* iHeight)
{
	// Store the resolution
	*iWidth = m_iScreenWidth;
	*iHeight = m_iScreenHeight;
}

Gwen::Controls::Canvas* CGUI::GetCanvas(eGUIView view)
{
	if(view < GUI_NONE)
		return m_pViews[view]->GetCanvas();
	return NULL;
}

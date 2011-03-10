//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CWindowSubclass.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

bool    CWindowSubclass::m_bSubclassed = false;
WNDPROC CWindowSubclass::m_wWndProc;

extern CClient * g_pClient;

LRESULT APIENTRY CWindowSubclass::WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Does our chat window exist?
	if(g_pClient->GetChatWindow())
	{
		// Give this input to our chat window
		if(g_pClient->GetChatWindow()->HandleUserInput(uMsg, (DWORD)wParam))
		{
			// The chat window handled it
			return 0;
		}
	}

	// Return this input back to windows
	return CallWindowProc(m_wWndProc, hWnd, uMsg, wParam, lParam);
}

void CWindowSubclass::Subclass(HWND hWnd)
{
	if(!m_bSubclassed)
	{
		m_wWndProc = SubclassWindow(hWnd, WndProc_Hook);
		m_bSubclassed = true;
	}
}

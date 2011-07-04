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
	// NOTE: This is bad, but the only way i could get it working
	// Are we focused?
	bool bFocused = (GetForegroundWindow() == hWnd);

	// Have we gained focus?
	if(bFocused && !g_pClient->IsFocused())
	{
		// Set the client focused flag
		g_pClient->SetFocused(true);
		CLogFile::Printf("Gained window focus");
	}
	// Have we lost focus?
	else if(!bFocused && g_pClient->IsFocused())
	{
		// Set the client focused flag
		g_pClient->SetFocused(false);
		CLogFile::Printf("Lost window focus");
	}

	// Show/hide the mouse
	//ShowCursor(!bFocused);

	// Are we focused?
	if(bFocused)
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

		/*
		CGUI* pGUI = g_pClient->GetGUI();
		CLogFile::Printf("%p", pGUI);
		if(pGUI)
		{
			MSG msg;
			msg.hwnd = hWnd;
			msg.lParam = lParam;
			msg.wParam = wParam;
			msg.message = uMsg;

			pGUI->ProcessInput(msg);
		}
		*/
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

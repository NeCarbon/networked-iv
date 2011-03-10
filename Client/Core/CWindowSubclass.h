//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CWindowSubclass.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CWindowSubclass
{
private:
	static bool    m_bSubclassed;
	static WNDPROC m_wWndProc;

	static LRESULT APIENTRY WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	static void             Subclass(HWND hWnd);
};

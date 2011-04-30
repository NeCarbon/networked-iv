//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CDirectInput8Hook.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CDirectInput8Hook
{
private:
	bool						m_bInstalled;

	typedef HRESULT          (WINAPI * DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);
	static DirectInput8Create_t m_pDirectInput8Create;

	static HRESULT WINAPI		 DirectInput8Create_Hook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);

public:
	void                       Install();
	void                       Uninstall();
};

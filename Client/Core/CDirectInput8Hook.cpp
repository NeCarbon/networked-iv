//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CDirectInput8Hook.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CDirectInput8Hook::DirectInput8Create_t CDirectInput8Hook::m_pDirectInput8Create;

HRESULT WINAPI CDirectInput8Hook::DirectInput8Create_Hook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{
	CLogFile::Printf("DirectInput8Create Called");

	// Call the real 'DirectInput8Create'
	HRESULT hr = m_pDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);

	// If the call succeeded create our proxy device
	if(SUCCEEDED(hr))
	{
		IDirectInput8 * pDInput = (IDirectInput8 *)*ppvOut;
		*ppvOut = new CDirectInput8Proxy(pDInput);
	}
	else
		CLogFile::Printf("DirectInput8Create Call Failed!");

	return hr;
}

void CDirectInput8Hook::Install()
{
	if(!m_bInstalled)
	{
		// Detour 'DirectInput8Create' inside dinput8.dll
		m_pDirectInput8Create = (DirectInput8Create_t)CPatcher::InstallDetourPatch("dinput8.dll", 
			"DirectInput8Create", (DWORD)DirectInput8Create_Hook);

		m_bInstalled = true;
	}
}

void CDirectInput8Hook::Uninstall()
{
	if(m_bInstalled)
	{
		// Remove the detour of 'DirectInput8Create' inside dinput8.dll
		CPatcher::UninstallDetourPatch(m_pDirectInput8Create, (DWORD)DirectInput8Create_Hook);

		m_bInstalled = false;
	}
}

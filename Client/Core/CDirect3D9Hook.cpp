//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CDirect3D9Hook.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CDirect3D9Hook::Direct3DCreate9_t CDirect3D9Hook::m_pfnDirect3DCreate9;

IDirect3D9 * WINAPI CDirect3D9Hook::Direct3DCreate9_Hook(UINT SDKVersion)
{
	CLogFile::Printf("Direct3DCreate9 Called\n");

	// Call the real 'Direct3DCreate9'
	IDirect3D9 * pD3D = m_pfnDirect3DCreate9(SDKVersion);

	// If the call succeeded return our proxy device
	if(pD3D)
	{
		return new CDirect3D9Proxy(pD3D);
	}

	CLogFile::Printf("Direct3DCreate9 Call Failed!\n");

	return NULL;
}

void CDirect3D9Hook::Install()
{
	if(!m_bInstalled)
	{
		// Detour 'Direct3DCreate9' inside d3d9.dll
		m_pfnDirect3DCreate9 = (Direct3DCreate9_t)CPatcher::InstallDetourPatch("d3d9.dll", "Direct3DCreate9", 
			(DWORD)Direct3DCreate9_Hook);

		m_bInstalled = true;
	}
}

void CDirect3D9Hook::Uninstall()
{
	if(m_bInstalled)
	{
		// Remove the detour of 'Direct3DCreate9' inside d3d9.dll
		CPatcher::UninstallDetourPatch(m_pfnDirect3DCreate9, (DWORD)Direct3DCreate9_Hook);

		m_bInstalled = false;
	}
}

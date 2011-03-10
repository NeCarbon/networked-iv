//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CDirect3D9Hook.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CDirect3D9Hook
{
private:
	bool				     m_bInstalled;

	typedef IDirect3D9 *	 (WINAPI * Direct3DCreate9_t)(UINT);
	static Direct3DCreate9_t m_pfnDirect3DCreate9;

	static IDirect3D9 * WINAPI Direct3DCreate9_Hook(UINT SDKVersion);

public:
	void                       Install();
	void                       Uninstall();
};

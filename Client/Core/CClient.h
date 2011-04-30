//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClient.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CClient
{
private:
	CGame                 * m_pGame;
	CXLiveHook            * m_pXLiveHook;
	CDirect3D9Hook        * m_pDirect3D9Hook;
	CDirectInput8Hook     * m_pDirectInput8Hook;
	CClientNetworkManager * m_pNetworkManager;
	CClientPlayerManager  * m_pPlayerManager;
	CClientVehicleManager * m_pVehicleManager;
	CStreamer             * m_pStreamer;
	CFont                 * m_pFont;
	CChatWindow           * m_pChatWindow;
	CClientTaskManager    * m_pClientTaskManager;
	unsigned int            m_uiBaseAddress;
	String                  m_strIp;
	unsigned short          m_usPort;
	String                  m_strPassword;
	String                  m_strNick;
	bool                    m_bInputState;
	bool                    m_bFocused;

public:
	CGame                 * GetGame() { return m_pGame; }
	CXLiveHook            * GetXLiveHook() { return m_pXLiveHook; }
	CDirect3D9Hook        * GetDirect3D9Hook() { return m_pDirect3D9Hook; }
	CClientNetworkManager * GetNetworkManager() { return m_pNetworkManager; }
	CClientPlayerManager  * GetPlayerManager() { return m_pPlayerManager; }
	CClientVehicleManager * GetVehicleManager() { return m_pVehicleManager; }
	CStreamer             * GetStreamer() { return m_pStreamer; }
	CFont                 * GetFont() { return m_pFont; }
	CChatWindow           * GetChatWindow() { return m_pChatWindow; }
	CClientTaskManager    * GetClientTaskManager() { return m_pClientTaskManager; }
	unsigned int            GetBaseAddress() { return m_uiBaseAddress; }
	String                  GetNick() { return m_strNick; }
	void                    SetInputState(bool bInputState) { m_bInputState = bInputState; }
	bool                    GetInputState() { return m_bInputState; }
	void                    SetFocused(bool bFocused);
	bool                    IsFocused() { return m_bFocused; }

	bool                    OnLoad();
	void                    OnUnload();
	static void             LogFileCallback(const char * szBuffer);
	static void             ExceptionHandlerCallback(_EXCEPTION_POINTERS * ExceptionInfo);
	void                    OnD3DCreateDevice(IDirect3DDevice9 * pD3DDevice);
	void                    OnD3DLostDevice(IDirect3DDevice9 * pD3DDevice);
	void                    OnD3DResetDevice(IDirect3DDevice9 * pD3DDevice);
	void                    OnD3DBeginScene(IDirect3DDevice9 * pD3DDevice);
	void                    OnD3DEndScene(IDirect3DDevice9 * pD3DDevice);
	void                    OnGameLoad();
	void                    OnGameProcess();
};

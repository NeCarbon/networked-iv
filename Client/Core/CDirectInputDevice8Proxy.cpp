//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CDirectInputDevice8Proxy.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

#if 0
bool m_bDisableEscape = false;
int m_iStartLoadTicks = 0;
char cOldReturn = 0;
#endif

CDirectInputDevice8Proxy::CDirectInputDevice8Proxy(IDirectInput8 * dinput, IDirectInputDevice8 * dinputdevice, eDIDeviceType DeviceType)
{
	m_pDI = dinput;
	m_pDIDevice = dinputdevice;
	m_DeviceType = DeviceType;
}

/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return m_pDIDevice->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE CDirectInputDevice8Proxy::AddRef()
{
	return m_pDIDevice->AddRef();
}

ULONG STDMETHODCALLTYPE CDirectInputDevice8Proxy::Release()
{
	ULONG uRet = m_pDIDevice->Release();

	// If the reference count is 0 delete ourselves
	if(uRet == 0)
		delete this;

	return uRet;
}

/*** IDirectInputDevice8A methods ***/
HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetCapabilities(LPDIDEVCAPS p0)
{
	return m_pDIDevice->GetCapabilities(p0);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumObjects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetProperty(REFGUID rguid, LPDIPROPHEADER p1)
{
	return m_pDIDevice->GetProperty(rguid, p1);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::SetProperty(REFGUID rguid, LPCDIPROPHEADER p1)
{
	return m_pDIDevice->SetProperty(rguid, p1);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::Acquire()
{
	return m_pDIDevice->Acquire();
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::Unacquire()
{
	return m_pDIDevice->Unacquire();
}	

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetDeviceState(DWORD p0, LPVOID p1)
{
	HRESULT hResult = m_pDIDevice->GetDeviceState(p0, p1);

	if(!g_pClient->GetInputState())
	{
		// If the inputstate is disabled clear the buffer
		memset(p1, 0, p0);
	}

#if 0
	if(m_DeviceType == DIDEVICE_TYPE_KEYBOARD)
	{
		// TODO: Some kind of system to assign a keypress (down, up, held) to a function
		// TODO: Some kind of system to disable/fully enable single keys
		/*
		char m_szKeysEnabled[256];
		for(int i = 0; i < 256; i++)
		{
		m_szKeysEnabled[i] = 1;
		}
		m_szKeysEnabled[DIK_ESCAPE] = 0;
		m_szKeysEnabled[DIK_ESCAPE] = 1;
		*/
		/*
		for(int i = 0; i < p0; i++)
		{
		if(keyBuffer[i] && !m_szKeysEnabled[i])
		{
		keyBuffer[i] = 0;
		}
		}
		*/
		char * keyBuffer = (char*)p1;

#if 0
		if(m_bDisableEscape && (keyBuffer[DIK_ESCAPE] & 0x80))
		{
			if(Game::GetState() == GAME_STATE_INGAME && !Game::GameIsPaused())
				Game::GamePause();
			/*else
				Game::GameUnpause();*/
		}
		// Don't let the game receive the escape key
		keyBuffer[DIK_ESCAPE] = 0;
#endif

		if(m_bDisableEscape)
			// Don't let the game receive the escape key
			keyBuffer[DIK_ESCAPE] = 0;

		if(CGame::GetState() == GAME_STATE_NONE)
		{
			if(!CGame::IsMenuActive())
			{
				cOldReturn = !cOldReturn;
				keyBuffer[DIK_RETURN] = cOldReturn;
			}
			else
			{
				CGame::SetInputState(false);
				CGame::SetState(GAME_STATE_MAIN_MENU);
			}
		}

		if(CGame::GetState() == GAME_STATE_LOADING)
		{
			cOldReturn = !cOldReturn;
			keyBuffer[DIK_RETURN] = cOldReturn;
		}
	}
#endif

	return hResult;
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetDeviceData(DWORD p0, LPDIDEVICEOBJECTDATA p1, LPDWORD p2, DWORD p3)
{
	HRESULT hResult = m_pDIDevice->GetDeviceData(p0, p1, p2, p3);

	if(!g_pClient->GetInputState())
	{
		// If the input state is disabled clear the buffer
		memset(p1, 0, (p0 * (*p2)));
	}

	return hResult;
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::SetDataFormat(LPCDIDATAFORMAT p0)
{
	return m_pDIDevice->SetDataFormat(p0);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::SetEventNotification(HANDLE p0)
{
	return m_pDIDevice->SetEventNotification(p0);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::SetCooperativeLevel(HWND p0, DWORD p1)
{
	return m_pDIDevice->SetCooperativeLevel(p0, p1);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA p0, DWORD p1, DWORD p2)
{
	return m_pDIDevice->GetObjectInfo(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetDeviceInfo(LPDIDEVICEINSTANCEA p0)
{
	return m_pDIDevice->GetDeviceInfo(p0);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::RunControlPanel(HWND p0, DWORD p1)
{
	return m_pDIDevice->RunControlPanel(p0, p1);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::Initialize(HINSTANCE p0, DWORD p1, REFGUID rguid)
{
	return m_pDIDevice->Initialize(p0, p1, rguid);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::CreateEffect(REFGUID rguid, LPCDIEFFECT p1, LPDIRECTINPUTEFFECT * p2, LPUNKNOWN p3)
{
	return m_pDIDevice->CreateEffect(rguid, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::EnumEffects(LPDIENUMEFFECTSCALLBACKA p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumEffects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetEffectInfo(LPDIEFFECTINFOA p0, REFGUID rguid)
{
	return m_pDIDevice->GetEffectInfo(p0, rguid);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetForceFeedbackState(LPDWORD p0)
{
	return m_pDIDevice->GetForceFeedbackState(p0);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::SendForceFeedbackCommand(DWORD p0)
{
	return m_pDIDevice->SendForceFeedbackCommand(p0);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumCreatedEffectObjects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::Escape(LPDIEFFESCAPE p0)
{
	return m_pDIDevice->Escape(p0);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::Poll()
{
	return m_pDIDevice->Poll();
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::SendDeviceData(DWORD p0, LPCDIDEVICEOBJECTDATA p1, LPDWORD p2, DWORD p3)
{
	return m_pDIDevice->SendDeviceData(p0, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::EnumEffectsInFile(LPCSTR p0, LPDIENUMEFFECTSINFILECALLBACK p1, LPVOID p2, DWORD p3)
{
	return m_pDIDevice->EnumEffectsInFile(p0, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::WriteEffectToFile(LPCSTR p0, DWORD p2, LPDIFILEEFFECT p3, DWORD p4)
{
	return m_pDIDevice->WriteEffectToFile(p0, p2, p3, p4);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::BuildActionMap(LPDIACTIONFORMATA p0, LPCSTR p1, DWORD p2)
{
	return m_pDIDevice->BuildActionMap(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::SetActionMap(LPDIACTIONFORMATA p0, LPCSTR p1, DWORD p2)
{
	return m_pDIDevice->SetActionMap(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE CDirectInputDevice8Proxy::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA p0)
{
	return m_pDIDevice->GetImageInfo(p0);
}

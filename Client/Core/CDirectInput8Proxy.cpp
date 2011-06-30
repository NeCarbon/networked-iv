//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CDirectInput8Proxy.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CDirectInput8Proxy::CDirectInput8Proxy(IDirectInput8 * dinput)
{
	m_dinput = dinput;
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return m_dinput->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE CDirectInput8Proxy::AddRef()
{
	return m_dinput->AddRef();
}

ULONG STDMETHODCALLTYPE CDirectInput8Proxy::Release()
{
	ULONG uRet = m_dinput->Release();

	// If the reference count is 0 delete ourselves
	if(uRet == 0)
		delete this;

	return uRet;
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A * lplpDirectInputDevice, LPUNKNOWN pUknOuter)
{
	CLogFile::Printf("CDirectInput8Proxy::CreateDevice Called");

	// Create the dinput device
	HRESULT hr = m_dinput->CreateDevice(rguid, lplpDirectInputDevice, pUknOuter);

	if(SUCCEEDED(hr))
	{
		CLogFile::Printf("CDirectInput8Proxy::CreateDevice Call Succeeded");

		// Get the device type
		eDIDeviceType type = DIDEVICE_TYPE_UNKNOWN;

		if(rguid == GUID_SysMouse)
			type = DIDEVICE_TYPE_MOUSE;
		else if(rguid == GUID_SysKeyboard)
			type = DIDEVICE_TYPE_KEYBOARD;
		else if(rguid == GUID_Joystick)
			type = DIDEVICE_TYPE_JOYSTICK;

		// Create the proxy device
		*lplpDirectInputDevice = new CDirectInputDevice8Proxy(this, *lplpDirectInputDevice, type);
	}
	else
		CLogFile::Printf("CDirect3D9Proxy::CreateDevice Call Failed!");

	return hr;
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_dinput->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::GetDeviceStatus(REFGUID rguidInstance)
{
	return m_dinput->GetDeviceStatus(rguidInstance);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return m_dinput->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::Initialize(HINSTANCE hinst, DWORD dwVersion)
{
	return m_dinput->Initialize(hinst, dwVersion);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance)
{
	return m_dinput->FindDevice(rguidClass, ptszName, pguidInstance);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_dinput->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData)
{
	return m_dinput->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}

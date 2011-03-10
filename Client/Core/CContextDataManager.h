//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CContextDataManager.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CContextData
{
private:
	CIVPlayerInfo * m_pPlayerInfo;
	CIVPlayerPed  * m_pPlayerPed;
	CClientPadState m_padState;
	Matrix          m_matCameraMatrix;

public:
	CContextData(CIVPlayerInfo * pPlayerInfo)
	{
		m_pPlayerInfo = pPlayerInfo;
		m_pPlayerPed = NULL;
		m_matCameraMatrix.Identity();
	}

	~CContextData()
	{

	}

	void              SetPlayerInfo(CIVPlayerInfo * pPlayerInfo) { m_pPlayerInfo = pPlayerInfo; }
	CIVPlayerInfo *   GetPlayerInfo() { return m_pPlayerInfo; }
	void              SetPlayerPed(CIVPlayerPed * pPlayerPed) { m_pPlayerPed = pPlayerPed; }
	CIVPlayerPed  *   GetPlayerPed() { return m_pPlayerPed; }
	CClientPadState * GetPadState() { return &m_padState; }
	Matrix        *   GetCameraMatrix() { return &m_matCameraMatrix; }
};

class CContextDataManager
{
private:
	static std::list<CContextData *> m_contextDataList;

public:
	CContextDataManager();
	~CContextDataManager();

	static CContextData * CreateContextData(CIVPlayerInfo * pPlayerInfo);
	static void           DestroyContextData(CContextData * pContextInfo);
	static CContextData * GetContextData(BYTE bytePlayerNumber);
	static CContextData * GetContextData(CIVPlayerInfo * pPlayerInfo);
	static CContextData * GetContextData(IVPlayerInfo * pPlayerInfo);
	static CContextData * GetContextData(CIVPlayerPed * pPlayerPed);
	static CContextData * GetContextData(IVPlayerPed * pPlayerPed);
};

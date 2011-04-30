//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServer.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CServer : public CServerInterface
{
private:
	bool               m_bActive;
	bool               m_bShowFPS;
	unsigned long      m_ulLastFPSUpdateTime;
	unsigned long      m_ulFrameCount;
	unsigned long      m_ulFramesPerSecond;
	CMutex             m_inputQueueMutex; // Mutex for m_inputQueue
	std::queue<String> m_inputQueue;
	CServerLister    * m_pServerLister;

	void        ProcessInputQueue();

public:
	CServer();
	~CServer();

	bool        OnLoad();
	void        Process();
	void        OnUnload();
	void        SetActive(bool bActive);
	bool        IsActive();
	void        AddToInputQueue(String strString);
	void        SetTitle(String strTitle);
	String      GetConfigString(String strKey, String strDefaultValue);
	int         GetConfigInteger(String strKey, int iDefaultValue);
	float       GetConfigFloat(String strKey, float fDefaultValue);
	bool        GetConfigBoolean(String strKey, bool bDefaultValue);
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerLister.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CServerLister
{
private:
	unsigned short m_usPort;
	CHttpClient  * m_pHttpClient;
	unsigned long  m_ulLastUpdate;
	String         m_strUpdatePath;
	bool           m_bWaitingForData;
	bool           m_bSentFailedMessage;
	bool           m_bSentListedMessage;
	bool           m_bSentBannedMessage;

public:
	CServerLister(unsigned short usPort);
	~CServerLister();

	void Process();
};

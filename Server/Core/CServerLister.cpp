//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CServerLister.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#define SERVER_LIST_TIMEOUT 50000 // 50 seconds
#define SERVER_LIST_HOST "jenksta.iv-multiplayer.com"
#define SERVER_LIST_PATH "/masterlist/add.php?port=%d"

CServerLister::CServerLister(unsigned short usPort)
{
	// Set the port
	m_usPort = usPort;

	// Create the http client instance
	m_pHttpClient = new CHttpClient();

	// Set the http client host
	m_pHttpClient->SetHost(SERVER_LIST_HOST);

	// Reset the last update tick
	m_ulLastUpdate = 0;

	// Prepare the update path string
	m_strUpdatePath.Format(SERVER_LIST_PATH, usPort);

	m_bWaitingForData = false;
	m_bSentListedMessage = false;
	m_bSentBannedMessage = false;
}

CServerLister::~CServerLister()
{
	// Delete the http client instance
	SAFE_DELETE(m_pHttpClient);
}

void CServerLister::Process()
{
	// Get the current time
	unsigned long ulTime = SharedUtility::GetTime();

	// Is it time for a server list update?
	if((ulTime - m_ulLastUpdate) >= SERVER_LIST_TIMEOUT)
	{
		// Is this the first post?
		if(!m_bSentListedMessage && !m_bSentBannedMessage)
			CLogFile::Printf("Posting server to server list.\n");

		// Post the update
		if(m_pHttpClient->Post(true, m_strUpdatePath))
			m_bWaitingForData = true;
		else
			CLogFile::Printf("Failed to post server to server list (%s)!\n", m_pHttpClient->GetLastErrorString().C_String());

		// Set the last update time
		m_ulLastUpdate = ulTime;
	}

	if(m_bWaitingForData)
	{
		// Process the http client
		m_pHttpClient->Process();

		// Check if we have got a response from our server listing
		if(m_pHttpClient->GotData())
		{
			// We have a response, check it
			if(!m_pHttpClient->GetData()->Compare("Added"))
			{
				// Server listed successfully
				if(!m_bSentListedMessage)
				{
					CLogFile::Printf("Server posted to server list successfully!\n");
					m_bSentListedMessage = true;
				}

				if(m_bSentBannedMessage)
					m_bSentBannedMessage = false;
			}
			else if(!m_pHttpClient->GetData()->Compare("Banned"))
			{
				// This server is banned from listing
				if(!m_bSentBannedMessage)
				{
					CLogFile::Printf("Failed to post server to server list (You are banned from listing your server)!\n");
					m_bSentBannedMessage = true;
				}

				if(m_bSentListedMessage)
					m_bSentListedMessage = false;
			}

			m_bWaitingForData = false;
		}
	}
}

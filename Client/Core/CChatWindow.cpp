//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CChatWindow.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CChatWindow::CChatWindow(CFont * pFont)
{
	m_bEnabled = false;
	m_pFont = pFont;

	// Create all the chat lines
	for(int i = 0; i < NUM_CHAT_LINES; i++)
	{
		m_pChatLines[i] = new CChatLine(pFont);
	}

	m_iCurrentPageScroll = 0;
	m_uiTotalMessages = 0;
	m_bInputEnabled = false;
	memset(m_szCurrentInput, 0, CHAT_LINE_LEN);
	m_bLogMessages = false;
}

CChatWindow::~CChatWindow()
{
	// Delete all the chat lines
	for(int i = 0; i < NUM_CHAT_LINES; i++)
	{
		SAFE_DELETE(m_pChatLines[i]);
	}
}

void CChatWindow::Draw()
{
	// Make sure we are enabled
	if(m_bEnabled)
	{
		float fX = 20;
		float fY = 20;

		// Loop through all chat lines
		for(int i = (((m_iCurrentPageScroll + 1) * NUM_CHAT_LINES_PER_PAGE) - 1); i >= 0; i--)
		{
			// Is this chat line active?
			if(m_pChatLines[i]->IsActive())
			{
				// Draw this chat line
				m_pChatLines[i]->Draw(fX, fY);
			}

			// Increment the y position
			fY += 18; // font height + 2
		}

		// Do we have any input text?
		if(m_bInputEnabled)
		{
			// Format the input text
			static char szBuffer[CHAT_LINE_LEN + 4];
			sprintf(szBuffer, "> %s", m_szCurrentInput);

			// Draw the input text
			m_pFont->Draw(szBuffer, 0xFF000000, (fX - 1), fY);
			m_pFont->Draw(szBuffer, 0xFF000000, (fX + 1), fY);
			m_pFont->Draw(szBuffer, 0xFF000000, fX, (fY - 1));
			m_pFont->Draw(szBuffer, 0xFF000000, fX, (fY + 1));
			m_pFont->Draw(szBuffer, 0xFFFFFFFF, fX, fY);
		}
	}
}

void CChatWindow::OutputMessage(DWORD dwColor, const char * szFormat, ...)
{
	// Collect the arguments
	va_list vaArgs;
	char szBuffer[CHAT_LINE_LEN + 1];
	va_start(vaArgs, szFormat);
	vsprintf(szBuffer, szFormat, vaArgs);
	va_end(vaArgs);

	// Move all chat messages up
	for(int i = (NUM_CHAT_LINES - 1); i > 0; i--)
	{
		// Is the chat line below this active?
		if(m_pChatLines[i - 1]->IsActive())
		{
			// Copy it to this chat line
			memcpy(m_pChatLines[i], m_pChatLines[i - 1], sizeof(CChatLine));
		}
		else
		{
			// Is this chat line active?
			if(m_pChatLines[i]->IsActive())
			{
				// Reset this chat line
				m_pChatLines[i]->Reset();
			}
		}
	}

	// Set the current chat line
	m_pChatLines[0]->SetActive(true);
	m_pChatLines[0]->SetColor(dwColor);
	m_pChatLines[0]->SetText(szBuffer);

	// Write the message to the log file
	if(m_bLogMessages)
	{
		bool bUseCallback = CLogFile::GetUseCallback();
		CLogFile::SetUseCallback(false);
		CLogFile::Printf("[ChatMsg:0x%p] %s\n", dwColor, szBuffer);
		CLogFile::SetUseCallback(bUseCallback);
	}

	// Increment the total messages count
	m_uiTotalMessages++;
}

void CChatWindow::Enable()
{
	m_bEnabled = true;
}

void CChatWindow::Disable()
{
	m_bEnabled = false;
}

bool CChatWindow::IsEnabled()
{
	return m_bEnabled;
}

void CChatWindow::ScrollPageUp()
{
	// Have we not reached the page upper limit yet?
	if(m_iCurrentPageScroll < (NUM_CHAT_PAGES - 1))
	{
		// Do we have any text on the page above?
		if(m_uiTotalMessages > (unsigned int)((m_iCurrentPageScroll + 1) * NUM_CHAT_LINES_PER_PAGE))
		{
			// Increment the current page scroll
			m_iCurrentPageScroll++;
		}
	}
}

void CChatWindow::ScrollPageDown()
{
	// Have we not reached the page lower yet?
	if(m_iCurrentPageScroll > 0)
	{
		// Decrement the current page scroll
		m_iCurrentPageScroll--;
	}
}

void CChatWindow::EnableInput()
{
	m_bInputEnabled = true;
}

void CChatWindow::DisableInput()
{
	m_bInputEnabled = false;
}

bool CChatWindow::IsInputEnabled()
{
	return m_bInputEnabled;
}

bool CChatWindow::CapInputBuffer(size_t sOffset)
{
	// Make sure sOffset is valid
	if(sOffset < strlen(m_szCurrentInput))
	{
		// Null terminate at sOffset
		m_szCurrentInput[sOffset] = '\0';

		return true;
	}

	// sOffset is invalid
	return false;
}

CClientVehicle * pTempVehicle = NULL;

void CChatWindow::ProcessInput()
{
	// Is there any input?
	size_t sLen = strlen(m_szCurrentInput);
	
	if(sLen > 0)
	{
		// Is it a command (first char is a /)?
		if(m_szCurrentInput[0] == '/')
		{
			// Check if we have a registered command for it
			char * szCommand = (m_szCurrentInput + 1);

			g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Entered command '%s'", szCommand);

			// TODO: return if processed here
			// NOTE: HARDCODED FOR NOW
			// TODO: CClientCommandHandler
			if(!strcmp(szCommand, "q") || !strcmp(szCommand, "quit") || !strcmp(szCommand, "exit"))
			{
				g_pClient->GetNetworkManager()->Disconnect();
				TerminateProcess(GetCurrentProcess(), 0);
				return;
			}
			else if(!strcmp(szCommand, "vehicle"))
			{
				if(!pTempVehicle)
				{
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Creating vehicle at your position...");
#define MODEL_SULTANRS 0xEE6024BC
					// Create vehicle instance
					pTempVehicle = new CClientVehicle(g_pClient->GetGame()->GetModelIndexFromHash(MODEL_SULTANRS));

					// Set the vehicle as can be streamed in
					pTempVehicle->SetCanBeStreamedIn(true);

					// Get local player position
					Vector3 vecPosition;
					g_pClient->GetPlayerManager()->GetLocalPlayer()->GetPosition(vecPosition);

					// Prevent vehicle from landing on player
					vecPosition.fX += 4;

					// Set vehicle position
					pTempVehicle->SetPosition(vecPosition);
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Created vehicle at your position!");
				}
				else
				{
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Deleting vehicle...");
					SAFE_DELETE(pTempVehicle);
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Deleted vehicle!");
				}
				return;
			}
			else if(!strcmp(szCommand, "ak47"))
			{
#define WEAPON_AK47 14
				g_pClient->GetPlayerManager()->GetLocalPlayer()->GiveWeapon(WEAPON_AK47, 9999);
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "AK-47 given to local player");
				return;
			}
			else if(!strcmp(szCommand, "grenade"))
			{
#define WEAPON_GRENADE 4
				g_pClient->GetPlayerManager()->GetLocalPlayer()->GiveWeapon(WEAPON_GRENADE, 9999);
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Grenades given to local player");
				return;
			}
			else if(!strcmp(szCommand, "molotov"))
			{
#define WEAPON_MOLOTOV 5
				g_pClient->GetPlayerManager()->GetLocalPlayer()->GiveWeapon(WEAPON_MOLOTOV, 9999);
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Molotovs given to local player");
				return;
			}
		}

		// Do we have a valid network manager instance?
		if(g_pClient->GetNetworkManager())
		{
			// It's either chat or an unregistered command so send it to the server
			CBitStream bitStream;

			// If it's a command write a 1 and don't send the /, else 0
			String strInput;

			if(m_szCurrentInput[0] == '/')
			{
				bitStream.Write1();
				strInput.Set(m_szCurrentInput + 1);
			}
			else
			{
				bitStream.Write0();
				strInput.Set(m_szCurrentInput);
			}

			// Write the input
			bitStream.Write(strInput);

			// Send it to the server
			// NOTE: We send chat/commands on ordering channel 1 instead of 0
			g_pClient->GetNetworkManager()->RPC(RPC_CHAT_INPUT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, 1);
		}
	}
}

bool CChatWindow::HandleUserInput(unsigned int uMsg, DWORD dwChar)
{
	// Was it a key release?
	if(uMsg == WM_KEYUP)
	{
		// Was it the page up key?
		if(dwChar == VK_PRIOR)
		{
			// Scroll the page up
			ScrollPageUp();

			// Return true to indicate we handled it
			return true;
		}
		// Was it the page down key?
		else if(dwChar == VK_NEXT)
		{
			// Scroll the page down
			ScrollPageDown();

			// Return true to indicate we handled it
			return true;
		}
		// Was it the escape key?
		else if(dwChar == VK_ESCAPE)
		{
			// Is the input enabled?
			if(m_bInputEnabled)
			{
				// Disable input
				DisableInput();

				// Return true to indicate we handled it
				return true;
			}
		}
		// Was it the return key?
		else if(dwChar == VK_RETURN)
		{
			// Is the input enabled?
			if(m_bInputEnabled)
			{
				// Process input buffer
				ProcessInput();

				// Clear input buffer
				CapInputBuffer(0);

				// Disable input
				DisableInput();

				// Return true to indicate we handled it
				return true;
			}
		}
	}
	// Was it a key press?
	else if(uMsg == WM_KEYDOWN)
	{
		// Was it the backspace key?
		if(dwChar == VK_BACK)
		{
			// Is the input enabled?
			if(m_bInputEnabled)
			{
				// Is there any input to delete?
				size_t sLen = strlen(m_szCurrentInput);

				if(sLen > 0)
				{
					// Cap input buffer at last char
					CapInputBuffer(sLen - 1);

					// Return true to indicate we handled it
					return true;
				}
			}
		}
	}
	// Was it a character?
	else if(uMsg == WM_CHAR)
	{
		// Was it the 't' or 'T' key?
		if(dwChar == 't' || dwChar == 'T')
		{
			// Is the input disabled?
			if(!m_bInputEnabled)
			{
				// Enable input
				EnableInput();

				// Return true to indicate we handled it
				return true;
			}
		}

		// Is the input enabled?
		if(m_bInputEnabled)
		{
			// Is the char valid for chat input?
			if(dwChar >= ' ')
			{
				// Do we have space in the input?
				size_t sLen = strlen(m_szCurrentInput);

				if(sLen < (CHAT_LINE_LEN - 1))
				{
					// Add this char to the input
					m_szCurrentInput[sLen] = (char)dwChar;

					// Null terminate the input
					m_szCurrentInput[sLen + 1] = '\0';

					// Return true to indicate we handled it
					return true;
				}
			}
		}
	}

	// Return false to indicate we didn't handle it
	return false;
}

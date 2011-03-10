//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CChatWindow.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define NUM_CHAT_PAGES 10
#define NUM_CHAT_LINES_PER_PAGE 10
#define NUM_CHAT_LINES (NUM_CHAT_PAGES * NUM_CHAT_LINES_PER_PAGE)
#define MESSAGE_INFO_COLOR D3DCOLOR_RGBA(0, 0, 255, 255)

class CChatWindow
{
private:
	bool         m_bEnabled;
	CFont *      m_pFont;
	CChatLine *  m_pChatLines[NUM_CHAT_LINES];
	int          m_iCurrentPageScroll;
	unsigned int m_uiTotalMessages;
	bool         m_bInputEnabled;
	char         m_szCurrentInput[CHAT_LINE_LEN + 1];
	bool         m_bLogMessages;

public:
	CChatWindow(CFont * pFont);
	~CChatWindow();

	void Draw();
	void OutputMessage(DWORD dwColor, const char * szFormat, ...);
	void Enable();
	void Disable();
	bool IsEnabled();
	void ScrollPageUp();
	void ScrollPageDown();
	void EnableInput();
	void DisableInput();
	bool IsInputEnabled();
	bool CapInputBuffer(size_t sOffset);
	void ProcessInput();
	bool HandleUserInput(unsigned int uMsg, DWORD dwChar);
	void SetLogMessages(bool bLogMessages) { m_bLogMessages = bLogMessages; }
	bool GetLogMessages() { return m_bLogMessages; }
};

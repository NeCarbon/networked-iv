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
		m_pChatLines[i] = new CChatLine(pFont);

	m_iCurrentPageScroll = 0;
	m_uiTotalMessages = 0;
	m_bInputEnabled = false;
	m_strCurrentInput.SetLimit(CHAT_LINE_LEN);
	m_bLogMessages = false;
}

CChatWindow::~CChatWindow()
{
	// Delete all the chat lines
	for(int i = 0; i < NUM_CHAT_LINES; i++)
		SAFE_DELETE(m_pChatLines[i]);
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
			String strBuffer("> %s", m_strCurrentInput.Get());

			// Draw the input text
			m_pFont->Draw(strBuffer.Get(), 0xFF000000, (fX - 1), fY);
			m_pFont->Draw(strBuffer.Get(), 0xFF000000, (fX + 1), fY);
			m_pFont->Draw(strBuffer.Get(), 0xFF000000, fX, (fY - 1));
			m_pFont->Draw(strBuffer.Get(), 0xFF000000, fX, (fY + 1));
			m_pFont->Draw(strBuffer.Get(), 0xFFFFFFFF, fX, fY);
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
		CLogFile::Printf("[ChatMsg:0x%p] %s", dwColor, szBuffer);
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
	g_pClient->SetInputState(false);
}

void CChatWindow::DisableInput()
{
	m_bInputEnabled = false;
	g_pClient->SetInputState(true);
}

bool CChatWindow::IsInputEnabled()
{
	return m_bInputEnabled;
}

bool CChatWindow::CapInputBuffer(size_t sOffset)
{
	// Make sure sOffset is valid
	if(sOffset < m_strCurrentInput.GetLength())
	{
		// Null terminate at sOffset
		m_strCurrentInput.Truncate(sOffset);
		return true;
	}

	// sOffset is invalid
	return false;
}

CClientVehicle * pTempVehicle = NULL;
int iFlag = 1;

void CChatWindow::ProcessInput()
{
	// Is there any input?
	if(!m_strCurrentInput.IsEmpty())
	{
		// Is it a command (first char is a /)?
		bool bIsCommand = (m_strCurrentInput.GetChar(0) == COMMAND_CHAR);

		if(bIsCommand)
		{
			// Get the end of the command
			size_t sCommandEnd = m_strCurrentInput.Find(" ");

			// If we have an invalid end use the end of the string
			if(sCommandEnd == String::nPos)
				sCommandEnd = m_strCurrentInput.GetLength();

			// Get the command name
			String strCommand = m_strCurrentInput.SubStr(1, (sCommandEnd - 1));

			// Get the command parameters
			String strParameters;

			// Do we have any parameters?
			if(sCommandEnd < m_strCurrentInput.GetLength())
				strParameters = m_strCurrentInput.SubStr((sCommandEnd + 1), m_strCurrentInput.GetLength());

			CLogFile::Printf("Entered input %s (Command: %s) (Parameters: %s)", m_strCurrentInput.Get(), 
				strCommand.Get(), strParameters.Get());

			// Check if we have a registered command for it
			// TODO: return if processed here
			// NOTE: HARDCODED FOR NOW
			// TODO: CClientCommandHandler
			if(strCommand == "q" || strCommand == "quit" || strCommand == "exit")
			{
				ExitProcess(0);
				return;
			}
			else if(strCommand == "kill")
			{
				CClientPlayer * pLocalPlayer = g_pClient->GetPlayerManager()->GetLocalPlayer();
				pLocalPlayer->Kill();
			}
			else if(strCommand == "ikill")
			{
				CClientPlayer * pLocalPlayer = g_pClient->GetPlayerManager()->GetLocalPlayer();
				pLocalPlayer->Kill(true);
			}
			else if(strCommand == "light")
			{
				CClientVehicle * pVehicle = g_pClient->GetPlayerManager()->GetLocalPlayer()->GetVehicle();

				if(pVehicle)
				{
					IVVehicle * pIVVehicle = pVehicle->GetGameVehicle()->GetVehicle();
					if(iFlag > 1)
						UNSET_BIT(pIVVehicle->m_byteUnknownFlags1, (iFlag / 2));
					CLogFile::Printf("Turning on current vehicle lights with flag %d", iFlag);
					SET_BIT(pIVVehicle->m_byteUnknownFlags1, iFlag);
					iFlag *= 2;
				}
			}
			else if(strCommand == "model")
			{
				if(!strParameters.IsEmpty())
				{
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Changing player model...");
					// Set the local player model
					int iModelIndex = strParameters.ToInteger();
					g_pClient->GetPlayerManager()->GetLocalPlayer()->SetModel(iModelIndex);
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Player model changed (Model %d)!", iModelIndex);
				}
			}
			else if(strCommand == "vehicle")
			{
				if(pTempVehicle)
				{
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Deleting vehicle...");
					SAFE_DELETE(pTempVehicle);
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Deleted vehicle!");
				}

				if(!strParameters.IsEmpty())
				{
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Creating vehicle at your position...");
					// Create vehicle instance
					int iModelIndex = strParameters.ToInteger();
					pTempVehicle = new CClientVehicle(iModelIndex);

					// Set the vehicle as can be streamed in
					pTempVehicle->SetCanBeStreamedIn(true);

					// Get local player position
					CVector3 vecPosition;
					g_pClient->GetPlayerManager()->GetLocalPlayer()->GetPosition(vecPosition);

					// Prevent vehicle from landing on player
					vecPosition.fX += 4;

					// Set vehicle position
					pTempVehicle->SetPosition(vecPosition);
					g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Created vehicle at your position (Model %d)!", iModelIndex);
				}

				return;
			}
			else if(strCommand == "pistol")
			{
				IVWeaponInfo * pIVWeaponInfo = g_pClient->GetGame()->GetWeaponInfo(WEAPON_TYPE_PISTOL)->GetWeaponInfo();
				IVWeaponInfo * pIVWeaponInfoRocket = g_pClient->GetGame()->GetWeaponInfo(WEAPON_TYPE_ROCKET)->GetWeaponInfo();
				eWeaponType weaponType = pIVWeaponInfo->m_weaponType;
				eWeaponSlot slot = pIVWeaponInfo->m_slot;
				DWORD dwModelHash = pIVWeaponInfo->m_dwModelHash;
				DWORD dwAnimGroup = pIVWeaponInfo->m_dwAnimGroup;
				DWORD dwAnimMeleeGroup1 = pIVWeaponInfo->m_dwAnimMeleeGroup1;
				DWORD dwAnimMeleeGroup2 = pIVWeaponInfo->m_dwAnimMeleeGroup2;
				memcpy(pIVWeaponInfo, pIVWeaponInfoRocket, sizeof(IVWeaponInfo));
				pIVWeaponInfo->m_weaponType = weaponType;
				pIVWeaponInfo->m_slot = slot;
				pIVWeaponInfo->m_dwModelHash = dwModelHash;
				pIVWeaponInfo->m_dwAnimGroup = dwAnimGroup;
				pIVWeaponInfo->m_dwAnimMeleeGroup1 = dwAnimMeleeGroup1;
				pIVWeaponInfo->m_dwAnimMeleeGroup2 = dwAnimMeleeGroup2;
				/*pIVWeaponInfo->m_wClipSize = 9;
				pIVWeaponInfo->m_dwTimeBetweenShots = 10;
				pIVWeaponInfo->m_wDamageBase = 80;
				pIVWeaponInfo->m_dwReloadTime = 500;
				pIVWeaponInfo->m_dwFastReloadTime = 500;
				pIVWeaponInfo->m_dwCrouchReloadTime = 500;
				pIVWeaponInfo->m_fWeaponRange = 2000.0f;
				pIVWeaponInfo->m_damageType = WEAPON_DAMAGE_TYPE_EXPLOSIVE;
				pIVWeaponInfo->m_fireType = WEAPON_FIRE_TYPE_PROJECTILE;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_HEAVY;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_CREATE_VISIBLE_ORDNANCE;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_2HANDED;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_KEEP_CAMERA_BEHIND;
				pIVWeaponInfo->m_fPhysicsForce = 1000.0f;
				pIVWeaponInfo->m_dwProjectTypeToCreate = g_pClient->GetGame()->GetWeaponInfo(WEAPON_TYPE_ROCKET)->GetWeaponInfo()->m_dwProjectTypeToCreate;
				pIVWeaponInfo->m_vecProjectileOffset = CVector3(0.54f, 0.0f, 0.055f);
				pIVWeaponInfo->m_vecProjectileRotOffset = CVector3(0.0f, 0.0f, -1.571f);*/
				pIVWeaponInfo->m_dwFlags = 0;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_GUN;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_CAN_AIM;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_CAN_FREE_AIM;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_ANIM_RELOAD;
				pIVWeaponInfo->m_dwFlags |= WEAPON_FLAG_ANIM_CROUCH_FIRE;
				g_pClient->GetPlayerManager()->GetLocalPlayer()->GiveWeapon(WEAPON_TYPE_PISTOL, 9999);
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Pistol given to local player");
				return;
			}
			else if(strCommand == "ak47")
			{
				g_pClient->GetPlayerManager()->GetLocalPlayer()->GiveWeapon(WEAPON_TYPE_AK47, 9999);
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "AK-47 given to local player");
				return;
			}
			else if(strCommand == "grenade")
			{
				g_pClient->GetPlayerManager()->GetLocalPlayer()->GiveWeapon(WEAPON_TYPE_GRENADE, 9999);
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Grenades given to local player");
				return;
			}
			else if(strCommand == "molotov")
			{
				g_pClient->GetPlayerManager()->GetLocalPlayer()->GiveWeapon(WEAPON_TYPE_MOLOTOV, 9999);
				g_pClient->GetChatWindow()->OutputMessage(MESSAGE_INFO_COLOR, "Molotovs given to local player");
				return;
			}
		}

		// Do we have a valid network manager instance?
		if(g_pClient->GetNetworkManager())
		{
			// It's either chat or an unregistered command so send it to the server
			CBitStream bitStream;

			// If it's a command don't send the command char
			String strInput;

			if(bIsCommand)
				strInput = (m_strCurrentInput.Get() + 1);
			else
				strInput = m_strCurrentInput;

			// Write if it is a command
			bitStream.WriteBit(bIsCommand);

			// Write the input
			bitStream.Write(strInput);

			// Send it to the server
			g_pClient->GetNetworkManager()->RPC(RPC_CHAT_INPUT, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, PACKET_CHANNEL_INPUT);
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
				if(!m_strCurrentInput.IsEmpty())
				{
					// Cap input buffer at last char
					CapInputBuffer(m_strCurrentInput.GetLength() - 1);

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
				// Add this char to the input
				m_strCurrentInput += (char)dwChar;

				// Return true to indicate we handled it
				return true;
			}
		}
	}

	// Return false to indicate we didn't handle it
	return false;
}

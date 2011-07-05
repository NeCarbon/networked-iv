//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CMainMenu.h
// Project: Client
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include "StdInc.h"

CMainMenu::CMainMenu(Gwen::Controls::Canvas* pCanvas) : Gwen::Controls::PanelListPanel(pCanvas)
{
	// Change it to no padding
	SetPadding( Gwen::Padding( 0, 0 ) );

	// Set the default position
	SetPos( 10, 300 );

	// Set the default size
	SetSize( 150, 300 );

	// Set it to vertical display
	SetVertical( );

	// Menu buttons
	Gwen::Controls::Button* pConnect =  new Gwen::Controls::Button( this );
	pConnect->SetText( "Connect" );
	pConnect->SetSize( 150, 30 );
	
	Gwen::Controls::Button* pSettings =  new Gwen::Controls::Button( this );
	pSettings->SetText( "Settings" );
	pSettings->SetSize( 150, 30 );
	
	Gwen::Controls::Button* pAbout =  new Gwen::Controls::Button( this );
	pAbout->SetText( "About" );
	pAbout->SetSize( 150, 30 );

	Gwen::Controls::Button* pExit =  new Gwen::Controls::Button( this );
	pExit->SetText( "Exit" );
	pExit->SetSize( 150, 30 );
	pExit->onPress.Add( this, &CMainMenu::Exit );
}

void CMainMenu::Exit( Gwen::Controls::Base* pControl )
{
	ExitProcess(0);
}

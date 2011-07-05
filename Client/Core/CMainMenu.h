//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CMainMenu.h
// Project: Client
// Author(s): mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include "StdInc.h"

class CMainMenu : public Gwen::Controls::PanelListPanel
{
public:
	CMainMenu(Gwen::Controls::Canvas* pCanvas);

	void Exit(Gwen::Controls::Base* pControl);
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CConfig.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CConfig
{
private:
	CXML * m_pXML;

public:
	CConfig();
	~CConfig();

	CXML * GetXML();
	bool   Open(String strFileName);
	bool   GetValueAsString(String strKey, String strDefaultValue, String * strValue);
	bool   GetValueAsInteger(String strKey, int iDefaultValue, int * iValue);
	bool   GetValueAsFloat(String strKey, float fDefaultValue, float * fValue);
	bool   GetValueAsBoolean(String strKey, bool bDefaultValue, bool * bValue);
};

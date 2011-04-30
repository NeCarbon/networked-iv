//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CConfig.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CConfig::CConfig()
{
	m_pXML = new CXML();
}

CConfig::~CConfig()
{
	SAFE_DELETE(m_pXML);
}

CXML * CConfig::GetXML()
{
	return m_pXML;
}

bool CConfig::Open(String strFileName)
{
	return m_pXML->load(strFileName);
}

bool CConfig::GetValueAsString(String strKey, String strDefaultValue, String * strValue)
{
	if(m_pXML->findNode(strKey.C_String()))
	{
		strValue->Set(m_pXML->nodeContent());
		m_pXML->nodeToRoot();
		return true;
	}

	strValue->Set(strDefaultValue);
	return false;
}

bool CConfig::GetValueAsInteger(String strKey, int iDefaultValue, int * iValue)
{
	if(m_pXML->findNode(strKey.C_String()))
	{
		*iValue = atoi(m_pXML->nodeContent());
		m_pXML->nodeToRoot();
		return true;
	}

	*iValue = iDefaultValue;
	return false;
}

bool CConfig::GetValueAsFloat(String strKey, float fDefaultValue, float * fValue)
{
	if(m_pXML->findNode(strKey.C_String()))
	{
		*fValue = (float)atof(m_pXML->nodeContent());
		m_pXML->nodeToRoot();
		return true;
	}

	*fValue = fDefaultValue;
	return false;
}

bool CConfig::GetValueAsBoolean(String strKey, bool bDefaultValue, bool * bValue)
{
	if(m_pXML->findNode(strKey.C_String()))
	{
		if(!strcmp(m_pXML->nodeContent(), "true"))
		{
			*bValue = true;
		}
		else if(!strcmp(m_pXML->nodeContent(), "false"))
		{
			*bValue = false;
		}
		else
		{
			*bValue = (atoi(m_pXML->nodeContent()) != 0);
		}

		m_pXML->nodeToRoot();
		return true;
	}

	*bValue = bDefaultValue;
	return false;
}

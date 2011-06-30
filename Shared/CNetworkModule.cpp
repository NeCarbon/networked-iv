//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetworkModule.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CLibrary *                  CNetworkModule::m_pLibrary;
GetNetServerInterface_t     CNetworkModule::m_pfnGetNetServerInterface;
DestroyNetServerInterface_t CNetworkModule::m_pfnDestroyNetServerInterface;
GetNetClientInterface_t     CNetworkModule::m_pfnGetNetClientInterface;
DestroyNetClientInterface_t CNetworkModule::m_pfnDestroyNetClientInterface;

CNetworkModule::CNetworkModule()
{

}

CNetworkModule::~CNetworkModule()
{

}

bool CNetworkModule::Init()
{
	// Create the library instance
	m_pLibrary = new CLibrary();

	// Get the net module path
	String strPath("%s" NETWORK_MODULE_NAME DEBUG_SUFFIX LIBRARY_EXTENSION, SharedUtility::GetAppPath());

	// Load the net module
	if(!m_pLibrary->Load(strPath.C_String()))
	{
		CLogFile::Printf("Failed to load network module!");
		return false;
	}

	// Verify the net module version
	if(!VerifyVersion(NETWORK_MODULE_VERSION))
	{
		CLogFile::Printf("Invalid net module version!");
		return false;
	}

	// Get pointers to the net module functions
	m_pfnGetNetServerInterface = (GetNetServerInterface_t)m_pLibrary->GetProcedureAddress("GetNetServerInterface");
	m_pfnDestroyNetServerInterface = (DestroyNetServerInterface_t)m_pLibrary->GetProcedureAddress("DestroyNetServerInterface");
	m_pfnGetNetClientInterface = (GetNetClientInterface_t)m_pLibrary->GetProcedureAddress("GetNetClientInterface");
	m_pfnDestroyNetClientInterface = (DestroyNetClientInterface_t)m_pLibrary->GetProcedureAddress("DestroyNetClientInterface");

	// Verify the pointers to the net module functions
	if(!m_pfnGetNetServerInterface || !m_pfnDestroyNetServerInterface || !m_pfnGetNetClientInterface || 
		!m_pfnDestroyNetClientInterface)
	{
		CLogFile::Printf("Net module is corrupt!");
		return false;
	}

	return true;
}

void CNetworkModule::Shutdown()
{
	// Unload the net module
	m_pLibrary->Unload();

	// Delete the library instance
	SAFE_DELETE(m_pLibrary);
}

bool CNetworkModule::VerifyVersion(BYTE byteVersion)
{
	// Get a pointer to the VerifyVersion function
	VerifyVersion_t pfnVerifyVersion = (VerifyVersion_t)m_pLibrary->GetProcedureAddress("VerifyVersion");

	// Is the pointer invalid?
	if(!pfnVerifyVersion)
		return false;

	// Call the VerifyVersion function and return the result
	return pfnVerifyVersion(byteVersion);
}

CNetServerInterface * CNetworkModule::GetNetServerInterface()
{
	// Call the GetNetServerInterface function
	CNetServerInterface * pRakServer = m_pfnGetNetServerInterface();

	// Return the RakServer instance
	return pRakServer;
}

void CNetworkModule::DestroyNetServerInterface(CNetServerInterface * pNetServer)
{
	// Call the DestroyNetServerInterface function
	m_pfnDestroyNetServerInterface(pNetServer);
}

CNetClientInterface * CNetworkModule::GetNetClientInterface()
{
	// Call the GetNetClientInterface function
	CNetClientInterface * pRakClient = m_pfnGetNetClientInterface();

	// Return the RakClient instance
	return pRakClient;
}

void CNetworkModule::DestroyNetClientInterface(CNetClientInterface * pNetClient)
{
	// Call the DestroyNetClientInterface function
	m_pfnDestroyNetClientInterface(pNetClient);
}

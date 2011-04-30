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
GetBitStreamInterface1_t    CNetworkModule::m_pfnGetBitStreamInterface1;
GetBitStreamInterface2_t    CNetworkModule::m_pfnGetBitStreamInterface2;
GetBitStreamInterface3_t    CNetworkModule::m_pfnGetBitStreamInterface3;
DestroyBitStreamInterface_t CNetworkModule::m_pfnDestroyBitStreamInterface;

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
		CLogFile::Printf("Failed to load network module!\n");
		return false;
	}

	// Verify the net module version
	if(!VerifyVersion(NETWORK_MODULE_VERSION))
	{
		CLogFile::Printf("Invalid net module version!\n");
		return false;
	}

	// Get pointers to the net module functions
	m_pfnGetNetServerInterface = (GetNetServerInterface_t)m_pLibrary->GetProcedureAddress("GetNetServerInterface");
	m_pfnDestroyNetServerInterface = (DestroyNetServerInterface_t)m_pLibrary->GetProcedureAddress("DestroyNetServerInterface");
	m_pfnGetNetClientInterface = (GetNetClientInterface_t)m_pLibrary->GetProcedureAddress("GetNetClientInterface");
	m_pfnDestroyNetClientInterface = (DestroyNetClientInterface_t)m_pLibrary->GetProcedureAddress("DestroyNetClientInterface");
	m_pfnGetBitStreamInterface1 = (GetBitStreamInterface1_t)m_pLibrary->GetProcedureAddress("GetBitStreamInterface1");
	m_pfnGetBitStreamInterface2 = (GetBitStreamInterface2_t)m_pLibrary->GetProcedureAddress("GetBitStreamInterface2");
	m_pfnGetBitStreamInterface3 = (GetBitStreamInterface3_t)m_pLibrary->GetProcedureAddress("GetBitStreamInterface3");
	m_pfnDestroyBitStreamInterface = (DestroyBitStreamInterface_t)m_pLibrary->GetProcedureAddress("DestroyBitStreamInterface");

	// Verify the pointers to the net module functions
	if(!m_pfnGetNetServerInterface || !m_pfnDestroyNetServerInterface || !m_pfnGetNetClientInterface || 
		!m_pfnDestroyNetClientInterface || !m_pfnGetBitStreamInterface1 || !m_pfnGetBitStreamInterface2 || 
		!m_pfnGetBitStreamInterface3 || !m_pfnDestroyBitStreamInterface)
	{
		CLogFile::Printf("Net module is corrupt!\n");
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

CNetBitStreamInterface * CNetworkModule::GetBitStreamInterface1()
{
	// Call the GetBitStreamInterface1 function
	CNetBitStreamInterface * pBitStream = m_pfnGetBitStreamInterface1();

	// Return the BitStream instance
	return pBitStream;
}

CNetBitStreamInterface * CNetworkModule::GetBitStreamInterface2(const unsigned int initialBytesToAllocate)
{
	// Call the GetBitStreamInterface2 function
	CNetBitStreamInterface * pBitStream = m_pfnGetBitStreamInterface2(initialBytesToAllocate);

	// Return the BitStream instance
	return pBitStream;
}

CNetBitStreamInterface * CNetworkModule::GetBitStreamInterface3(unsigned char* _data, const unsigned int lengthInBytes, bool _copyData)
{
	// Call the GetBitStreamInterface3 function
	CNetBitStreamInterface * pBitStream = m_pfnGetBitStreamInterface3(_data, lengthInBytes, _copyData);

	// Return the BitStream instance
	return pBitStream;
}

void CNetworkModule::DestroyBitStreamInterface(CNetBitStreamInterface * pBitStream)
{
	// Call the DestroyNetClientInterface function
	m_pfnDestroyBitStreamInterface(pBitStream);
}

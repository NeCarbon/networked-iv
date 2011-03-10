//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetModule.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

typedef bool                     (* VerifyVersion_t)(BYTE byteVersion);
typedef CNetServerInterface *    (* GetNetServerInterface_t)();
typedef void                     (* DestroyNetServerInterface_t)(CNetServerInterface * pRakServer);
typedef CNetClientInterface *    (* GetNetClientInterface_t)();
typedef void                     (* DestroyNetClientInterface_t)(CNetClientInterface * pRakClient);
typedef CNetBitStreamInterface * (* GetBitStreamInterface1_t)();
typedef CNetBitStreamInterface * (* GetBitStreamInterface2_t)(const unsigned int initialBytesToAllocate);
typedef CNetBitStreamInterface * (* GetBitStreamInterface3_t)(unsigned char* _data, const unsigned int lengthInBytes, bool _copyData);
typedef void                     (* DestroyBitStreamInterface_t)(CNetBitStreamInterface * pBitStream);

class CNetModule
{
private:
	static CLibrary *                  m_pLibrary;
	static GetNetServerInterface_t     m_pfnGetNetServerInterface;
	static DestroyNetServerInterface_t m_pfnDestroyNetServerInterface;
	static GetNetClientInterface_t     m_pfnGetNetClientInterface;
	static DestroyNetClientInterface_t m_pfnDestroyNetClientInterface;
	static GetBitStreamInterface1_t    m_pfnGetBitStreamInterface1;
	static GetBitStreamInterface2_t    m_pfnGetBitStreamInterface2;
	static GetBitStreamInterface3_t    m_pfnGetBitStreamInterface3;
	static DestroyBitStreamInterface_t m_pfnDestroyBitStreamInterface;

public:
	CNetModule();
	~CNetModule();

	static bool                     Init();
	static void                     Shutdown();
	static bool                     VerifyVersion(BYTE byteVersion);
	static CNetServerInterface *    GetNetServerInterface();
	static void                     DestroyNetServerInterface(CNetServerInterface * pNetServer);
	static CNetClientInterface *    GetNetClientInterface();
	static void                     DestroyNetClientInterface(CNetClientInterface * pNetClient);
	static CNetBitStreamInterface * GetBitStreamInterface1();
	static CNetBitStreamInterface * GetBitStreamInterface2(const unsigned int initialBytesToAllocate);
	static CNetBitStreamInterface * GetBitStreamInterface3(unsigned char* _data, const unsigned int lengthInBytes, bool _copyData);
	static void                     DestroyBitStreamInterface(CNetBitStreamInterface * pBitStream);
};

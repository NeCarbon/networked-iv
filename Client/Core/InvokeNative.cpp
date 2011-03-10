//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: InvokeNative.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//          Contains zlib licensed code from ScriptHook by aru
//
//==========================================================================

#include <StdInc.h>

typedef void (_cdecl * NativeFunction_t)(scrNativeCallContext * pContext);

extern CClient * g_pClient;

void InvokeNativeInternal(unsigned int uHash, NativeContext * pContext)
{
	DWORD dwFunction = (g_pClient->GetBaseAddress() + FUNC_ScrVM__FindNativeAddress_7);
	DWORD dwNativeAddress = NULL;
	_asm
	{
		xor eax, eax
		push esi
		mov esi, uHash
		call dwFunction
		pop esi
		mov dwNativeAddress, eax
	}

	if(dwNativeAddress)
	{
		NativeFunction_t pfnNativeFunction = (NativeFunction_t)dwNativeAddress;
		pfnNativeFunction(pContext);
	}
}

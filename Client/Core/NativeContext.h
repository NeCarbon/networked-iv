//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: NativeContext.h
// Project: Client
// License: zlib licensed code from ScriptHook by aru
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define MAX_NATIVE_PARAMS 16
#define MAX_NATIVE_ARG_SIZE 4
#define VAR_ScrVM__ThreadPool_7 0x1983310

class NativeContext : public scrNativeCallContext
{
private:
	// Anything temporary that we need
	unsigned char m_TempStack[MAX_NATIVE_PARAMS * MAX_NATIVE_ARG_SIZE];

public:
	inline NativeContext()
	{
		m_pArgs = (DWORD)&m_TempStack;
		m_pReturn = (DWORD)&m_TempStack; // It's okay to point both args and return at 
										 // the same pointer. The game should handle this.
		m_nArgCount = 0;
		m_nDataCount = 0;
	}

	template <typename T>
	inline void Push(T value)
	{
		if(sizeof(T) > MAX_NATIVE_ARG_SIZE)
		{
			// We only accept sized 4 or less arguments... that means no double/f64 or large structs are allowed.
			throw "Argument has an invalid size";
		}
		else if(sizeof(T) < MAX_NATIVE_ARG_SIZE)
		{
			// Ensure we don't have any stray data
			*(unsigned int *)(m_TempStack + MAX_NATIVE_ARG_SIZE * m_nArgCount) = 0;
		}

		*(T *)(m_TempStack + MAX_NATIVE_ARG_SIZE * m_nArgCount) = value;
		m_nArgCount++;
	}

	template <typename T>
	inline T GetResult()
	{
		// Copy back any vector results
		while(m_nDataCount > 0)
		{
			m_nDataCount--;
			Vector3 * pVec3 = m_pOriginalData[m_nDataCount];
			Vector4 * pVec4 = &m_TemporaryData[m_nDataCount];
			pVec3->fX = pVec4->fX;
			pVec3->fY = pVec4->fY;
			pVec3->fZ = pVec4->fZ;
		}

		return *(T *)(m_TempStack);
	}
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: InvokeNative.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//          Contains zlib licensed code from ScriptHook by aru
//
//==========================================================================
// TODO: Clean up

#pragma once

#include <StdInc.h>

#define FUNC_ScrVM__FindNativeAddress_7 0x5A76D0

void InvokeNativeInternal(unsigned int uiHash, NativeContext * pContext);

template <typename ReturnType>
static ReturnType InvokeNative(unsigned int uHash)
{
	NativeContext cxt;
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

template <typename ReturnType, typename Type1>
static ReturnType InvokeNative(unsigned int uHash, Type1 P1)
{
	NativeContext cxt;
	cxt.Push(P1);
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

template <typename ReturnType, typename Type1, typename Type2>
static ReturnType InvokeNative(unsigned int uHash, Type1 P1, Type2 P2)
{
	NativeContext cxt;
	cxt.Push(P1);
	cxt.Push(P2);
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

template <typename ReturnType, typename Type1, typename Type2, typename Type3>
static ReturnType InvokeNative(unsigned int uHash, Type1 P1, Type2 P2, Type3 P3)
{
	NativeContext cxt;
	cxt.Push(P1);
	cxt.Push(P2);
	cxt.Push(P3);
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

template <typename ReturnType, typename Type1, typename Type2, typename Type3, typename Type4>
static ReturnType InvokeNative(unsigned int uHash, Type1 P1, Type2 P2, Type3 P3, Type4 P4)
{
	NativeContext cxt;
	cxt.Push(P1);
	cxt.Push(P2);
	cxt.Push(P3);
	cxt.Push(P4);
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

template <typename ReturnType, typename Type1, typename Type2, typename Type3, typename Type4, typename Type5>
static ReturnType InvokeNative(unsigned int uHash, Type1 P1, Type2 P2, Type3 P3, Type4 P4, Type5 P5)
{
	NativeContext cxt;
	cxt.Push(P1);
	cxt.Push(P2);
	cxt.Push(P3);
	cxt.Push(P4);
	cxt.Push(P5);
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

template <typename ReturnType, typename Type1, typename Type2, typename Type3, typename Type4, typename Type5, 
typename Type6>
static ReturnType InvokeNative(unsigned int uHash, Type1 P1, Type2 P2, Type3 P3, Type4 P4, Type5 P5, Type6 P6)
{
	NativeContext cxt;
	cxt.Push(P1);
	cxt.Push(P2);
	cxt.Push(P3);
	cxt.Push(P4);
	cxt.Push(P5);
	cxt.Push(P6);
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

template <typename ReturnType, typename Type1, typename Type2, typename Type3, typename Type4, typename Type5, 
typename Type6, typename Type7, typename Type8, typename Type9, typename Type10, typename Type11>
static ReturnType InvokeNative(unsigned int uHash, Type1 P1, Type2 P2, Type3 P3, Type4 P4, Type5 P5, Type6 P6, Type7 P7, 
						Type8 P8, Type9 P9, Type10 P10, Type11 P11)
{
	NativeContext cxt;
	cxt.Push(P1);
	cxt.Push(P2);
	cxt.Push(P3);
	cxt.Push(P4);
	cxt.Push(P5);
	cxt.Push(P6);
	cxt.Push(P7);
	cxt.Push(P8);
	cxt.Push(P9);
	cxt.Push(P10);
	cxt.Push(P11);
	InvokeNativeInternal(uHash, &cxt);
	return cxt.GetResult<ReturnType>();
}

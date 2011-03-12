//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: StdInc.h
// Project: Server.Launcher
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

// Default
#ifdef WIN32
#include <windows.h>
#else
#include <Linux.h>
#endif

#include <string.h>

// Shared
#include <Common.h>
#include <CString.h>
#include <SharedUtility.h>
#include <CLibrary.h>
#include <CMutex.h>
#include <CThread.h>
#include <CServerInterface.h>

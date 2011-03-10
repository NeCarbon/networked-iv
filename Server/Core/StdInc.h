//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: StdInc.h
// Project: Server
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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <list>
#include <map>
#include <queue>
#include <math.h>

// Shared
#include <Common.h>
#include <CString.h>
#include <SharedUtility.h>
#include <CPlayerSocket.h>
#include <CPacket.h>
#include <PacketIdentifiers.h>
#include <PacketPriorities.h>
#include <PacketReliabilities.h>
#include <RPCIdentifiers.h>
#include <CMath.h>
#include <CNetworkPadState.h>
#include <CNetBitStreamInterface.h>
#include <CBitStreamInterface.h>
#include <CBitStream.h>
#include <CPacketHandler.h>
#include <CRPCHandler.h>
#include <CNetStats.h>
#include <CNetServerInterface.h>
#include <CNetClientInterface.h>
#include <CLibrary.h>
#include <CNetModule.h>
#include <CLogFile.h>
#include <TinyXML/tinyxml.h>
#include <TinyXML/ticpp.h>
#include <CXML.h>
#include <CMutex.h>
#include <squirrel/squirrel.h>
#include <CExceptionHandler.h>
#include <CServerInterface.h>
#include <CHttpClient.h>

// Project
#include "CConfig.h"
#include "CServerLister.h"
#include "CServer.h"
#include "CServerRPCHandler.h"
#include "CServerPacketHandler.h"
#include "CNetworkManager.h"
#include "CEntity.h"
#include "CRootEntity.h"
#include "CPlayer.h"
#include "CPlayerManager.h"

// Scripting
#include "scripting/CSquirrelArguments.h"
#include "scripting/CSquirrel.h"
#include "scripting/CTimer.h"
#include "scripting/CTimers.h"
#include "scripting/CResource.h"
#include "scripting/CResourceManager.h"
#include "scripting/natives/NativesCommon.h"
#include "scripting/natives/CServerNatives.h"
#include "scripting/natives/CEntityNatives.h"
#include "scripting/natives/CPlayerNatives.h"
#include "scripting/natives/CResourceNatives.h"
#include "scripting/natives/CEventNatives.h"
#include "scripting/natives/CTimerNatives.h"

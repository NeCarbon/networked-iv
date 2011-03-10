//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: Common.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// Debug mode
//#define NIV_DEBUG 1

// If compiling in debug mode force mod debug mode
#ifdef _DEBUG
#undef NIV_DEBUG
#define NIV_DEBUG 1
#endif

// Mod name
#define MOD_NAME "Networked: IV"

// Mod os string
#ifdef WIN32
#define MOD_OS_STRING "Win32"
#else
#define MOD_OS_STRING "Linux"
#endif

// Mod debug version
#ifdef NIV_DEBUG
#define MOD_VER_DBG "-debug"
#else
#define MOD_VER_DBG
#endif

// Mod version string
#define MOD_VERSION_STRING "1.0-pre-alpha-1" MOD_VER_DBG

// Network module version
#define NETWORK_MODULE_VERSION 0x2

// Network version
#define NETWORK_VERSION 0x2

// Network tick rate
#define NETWORK_TICK_RATE 100

// Max player limit
#define PLAYER_MAX 32

// Max name length
#define NICK_MAX 32

// Type used for entity ids
// NOTE: Must be unsigned
typedef unsigned char EntityId;

// Invalid entity id value
#define INVALID_ENTITY_ID 0xFF

// Entity type enum
enum eEntityType
{
	ENTITY_TYPE_NONE,
	ENTITY_TYPE_ROOT,
	ENTITY_TYPE_PLAYER,
	ENTITY_TYPE_VEHICLE,
	ENTITY_TYPE_RESOURCE,
	ENTITY_TYPE_TIMER,
	ENTITY_TYPE_CUSTOM,
	ENTITY_TYPE_MAX
};

// brandnew and sexy: Element ID
typedef unsigned short EntityID;
#define MAX_ENTITIES           0xFFFE
#define INVALID_ENTITY_ID_LONG 0xFFFF

// Safe memory deletion macro
#define SAFE_DELETE(memory) if(memory) { delete memory; memory = NULL; }

// Simple Bit Manipulation
#define SET_BIT(a, b) a |= b
#define IS_BIT_SET(a, b) (a & b)
#define UNSET_BIT(a, b) a &= ~(b)

// Library Names
#define CLIENT_CORE_NAME "Client.Core"
#define CLIENT_LAUNCH_HELPER "Client.LaunchHelper"
#define SERVER_CORE_NAME "Server.Core"
#define NETWORK_MODULE_NAME "Net.Core"

// Library Exports
#ifdef EXPORT
#undef EXPORT
#endif
#ifdef WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

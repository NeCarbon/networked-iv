//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: Linux.h
// Project: Shared
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#ifndef WIN32

#include <unistd.h>
#include <limits.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>

#define MAX_PATH PATH_MAX
#define Sleep(ms) usleep((ms) * 1000)
#define ExitProcess(code) _exit(code)

typedef bool BOOL;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef char *PCHAR;
typedef void *PVOID;
typedef unsigned char BYTE;

#define THREAD_PRIORITY_NORMAL 0

#define stricmp strcasecmp
#define Sleep(ms) usleep(ms * 1000)

long GetTickCount();

#endif

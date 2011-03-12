//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: Linux.cpp
// Project: Shared
// Author(s): adamix
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>
#include <sys/time.h>

long GetTickCount()
{
        struct timeval tv;

        if(gettimeofday(&tv, NULL) != 0)
                return 0;

        return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}


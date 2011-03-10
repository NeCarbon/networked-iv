//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CLibrary.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================


#pragma once


#include <StdInc.h>


#ifdef WIN32
#define LIBRARY_EXTENSION ".dll"
#else
#define LIBRARY_EXTENSION ".so"
#endif


class CLibrary
{
private:
        void * m_pLibrary;


public:
        CLibrary();
        ~CLibrary();


        bool   Load(const char * szPath);
        bool   Unload();
        void * GetProcedureAddress(const char * szProcedureName);
        void * GetProcedureAddress(unsigned int uOrdinal);
};

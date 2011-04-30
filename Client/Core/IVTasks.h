//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: IVTasks.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CIVTaskComplexNewGetInVehicle : public CIVTask
{
public:
	CIVTaskComplexNewGetInVehicle(CIVVehicle * pVehicle, int a3, int a4, unsigned int a5, float a6);
};

class CIVTaskComplexNewExitVehicle : public CIVTask
{
public:
	CIVTaskComplexNewExitVehicle(CIVVehicle * pVehicle, int a3, int a4, int a5);
};

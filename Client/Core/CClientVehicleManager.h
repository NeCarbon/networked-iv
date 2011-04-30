//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientVehicleManager.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CClientVehicleManager
{
private:
	CClientVehicle * m_pNetworkVehicles[VEHICLE_MAX];

public:
	CClientVehicleManager();
	~CClientVehicleManager();

	CClientVehicle * Add(EntityId vehicleId, int iModelIndex);
	bool             Delete(EntityId vehicleId);
	void             Process();
	bool             IsActive(EntityId vehicleId);
	CClientVehicle * Get(EntityId vehicleId);
	EntityId         GetCount();
};

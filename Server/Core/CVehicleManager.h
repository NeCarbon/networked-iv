//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CVehicleManager.h
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CVehicleManager
{
private:
	CVehicle * m_pVehicles[VEHICLE_MAX];

public:
	CVehicleManager();
	~CVehicleManager();

	CVehicle * Add(int iModelIndex);
	bool       Delete(EntityId vehicleId);
	bool       IsActive(EntityId vehicleId);
	CVehicle * Get(EntityId vehicleId);
	EntityId   GetCount();
	void       HandlePlayerJoin(EntityId playerId);
};

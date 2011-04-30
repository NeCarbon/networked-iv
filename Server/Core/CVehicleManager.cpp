//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CVehicleManager.cpp
// Project: Server
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CVehicleManager::CVehicleManager()
{
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
		m_pVehicles[i] = NULL;
}

CVehicleManager::~CVehicleManager()
{
	// Loop through all vehicles
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Is the current vehicle active?
		if(IsActive(i))
		{
			// Delete the vehicle player
			Delete(i);
		}
	}
}

CVehicle * CVehicleManager::Add(int iModelIndex)
{
	// Loop through all vehicles
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Is the current vehicle inactive?
		if(!IsActive(i))
		{
			// Create the vehicle instance
			m_pVehicles[i] = new CVehicle(i, iModelIndex);

			// Was the vehicle instance created?
			if(m_pVehicles[i])
			{
				// Spawn the vehicle for everyone
				m_pVehicles[i]->SpawnForWorld();

				// Trigger the creation event
				m_pVehicles[i]->CallEvent("vehicleCreate", NULL);
				return m_pVehicles[i];
			}
		}
	}

	// No vehicle slots found
	return NULL;
}

bool CVehicleManager::Delete(EntityId vehicleId)
{
	// Is this vehicle not active?
	if(!IsActive(vehicleId))
		return false;

	// Trigger the destruction event
	m_pVehicles[vehicleId]->CallEvent("vehicleDestroy", NULL);

	// Delete the vehicle for everyone
	m_pVehicles[vehicleId]->DestroyForWorld();

	// Delete the vehicle instance
	delete m_pVehicles[vehicleId];

	// Set the vehicle pointer to NULL
	m_pVehicles[vehicleId] = NULL;
	return true;
}

bool CVehicleManager::IsActive(EntityId vehicleId)
{
	// Is the vehicle id invalid?
	if(vehicleId >= VEHICLE_MAX)
		return false;

	return (m_pVehicles[vehicleId] != NULL);
}

CVehicle * CVehicleManager::Get(EntityId vehicleId)
{
	// Is this vehicle not active?
	if(!IsActive(vehicleId))
		return NULL;

	return m_pVehicles[vehicleId];
}

EntityId CVehicleManager::GetCount()
{
	EntityId vehicleCount = 0;

	// Loop through all vehicles
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Is the current vehicle active?
		if(IsActive(i))
		{
			// Increment the vehicle count
			vehicleCount++;
		}
	}

	return vehicleCount;
}

void CVehicleManager::HandlePlayerJoin(EntityId playerId)
{
	// Loop through all vehicles
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Is the current vehicle active?
		if(IsActive(i))
		{
			// Spawn the current vehicle for this player
			m_pVehicles[i]->SpawnForPlayer(playerId);
		}
	}
}

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientVehicleManager.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CClientVehicleManager::CClientVehicleManager()
{
	// Loop through all network vehicle pointers
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Set this network vehicle pointer to NULL
		m_pNetworkVehicles[i] = NULL;
	}
}

CClientVehicleManager::~CClientVehicleManager()
{
	// Loop through all network vehicle pointers
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Is this network vehicle pointer valid?
		if(m_pNetworkVehicles[i])
		{
			// Delete this network vehicle
			SAFE_DELETE(m_pNetworkVehicles[i]);
		}
	}
}

CClientVehicle * CClientVehicleManager::Add(EntityId vehicleId, int iModelIndex)
{
	// Is this vehicle already added?
	if(IsActive(vehicleId))
		return false;

	// Create the network vehicle instance
	m_pNetworkVehicles[vehicleId] = new CClientVehicle(iModelIndex);

	// Was the network vehicle instance created?
	if(m_pNetworkVehicles[vehicleId])
	{
		// Set the vehicles id
		m_pNetworkVehicles[vehicleId]->SetVehicleId(vehicleId);
		return m_pNetworkVehicles[vehicleId];
	}

	return NULL;
}

bool CClientVehicleManager::Delete(EntityId vehicleId)
{
	// Is this vehicle not active?
	if(!IsActive(vehicleId))
		return false;

	// Delete the network vehicle instance
	SAFE_DELETE(m_pNetworkVehicles[vehicleId]);
	return true;
}

void CClientVehicleManager::Process()
{
	// Loop through all vehicles
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Is this network vehicle pointer valid?
		if(m_pNetworkVehicles[i])
		{
			// Process the current vehicle
			m_pNetworkVehicles[i]->Process();
		}
	}
}

bool CClientVehicleManager::IsActive(EntityId vehicleId)
{
	// Is the vehicle id invalid?
	if(vehicleId >= VEHICLE_MAX)
		return false;

	return (m_pNetworkVehicles[vehicleId] != NULL);
}

CClientVehicle * CClientVehicleManager::Get(EntityId vehicleId)
{
	// Is this vehicle not active?
	if(!IsActive(vehicleId))
		return NULL;

	return m_pNetworkVehicles[vehicleId];
}

EntityId CClientVehicleManager::GetCount()
{
	EntityId vehicleCount = 0;

	// Loop through all vehicles
	for(EntityId i = 0; i < VEHICLE_MAX; i++)
	{
		// Is the current network vehicle pointer valid?
		if(m_pNetworkVehicles[i])
		{
			// Increment the vehicle count
			vehicleCount++;
		}
	}

	return vehicleCount;
}

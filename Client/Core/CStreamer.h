//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CStreamer.h
// Project: Client
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

class CStreamer : public std::list<CStreamableEntity *>
{
private:
	unsigned long                  m_ulLastStreamTime;
	DimensionId                    m_dimensionId;
	std::list<CStreamableEntity *> m_streamedElements[ENTITY_TYPE_MAX];
	std::list<CStreamableEntity *> m_newlyStreamedElements[ENTITY_TYPE_MAX];
	unsigned int                   m_uiStreamingLimits[ENTITY_TYPE_MAX]; // Max number of each entity type the game can handle	

public:
	CStreamer();
	~CStreamer();

	void                             Reset();
	void                             Process();
	void                             UpdateInterior(unsigned int uiInterior);
	void                             ForceStreamIn(CStreamableEntity * pEntity, bool bInstantly = false);
	void                             remove(CStreamableEntity * pEntity);
	std::list<CStreamableEntity *> * GetStreamedInEntitiesOfType(eEntityType eType);
	CClientPlayer                  * GetPlayerFromGamePlayerPed(IVPlayerPed * pGamePlayerPed);
	CClientVehicle                 * GetVehicleFromGameVehicle(IVVehicle * pGameVehicle);
};

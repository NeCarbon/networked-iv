//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CStreamer.cpp
// Project: Client
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

// TODO: Adjust (Or remove totally)?
#define STREAMING_TICK 1000

extern CClient * g_pClient;

CStreamer::CStreamer()
{
	m_uiStreamingLimits[ENTITY_TYPE_PLAYER] = 31; // Player infos array size = 32 (Also needs to account for local player)
	m_uiStreamingLimits[ENTITY_TYPE_VEHICLE] = 64; // Vehicle pool = 140
	Reset();
}

CStreamer::~CStreamer()
{

}

void CStreamer::Reset()
{
	m_ulLastStreamTime = 0;
	m_dimensionId = INVALID_DIMENSION_ID;

	// Loop through all types
	for(int i = 0; i < ENTITY_TYPE_MAX; ++i)
	{
		// Check if we have streamed in elements for this
		if(m_streamedElements[i].size() > 0)
		{
			// Output a message
			CLogFile::Printf("CStreamer::Reset with %d objects of type %d", m_streamedElements[i].size(), i);

			// Loop through all entities
			for(std::list<CStreamableEntity *>::iterator iter = m_streamedElements[i].begin(); iter != m_streamedElements[i].end(); ++ iter)
			{
				// Stream it out
				(*iter)->StreamOutInternal();
			}

			// Clear the list of entities specific to that type
			m_streamedElements[i].clear();
		}
	}

	// Clear the list of all entities
	clear();
}

inline bool SortStreamableEntites(CStreamableEntity * pEntity, CStreamableEntity * pOther)
{
	CVector3 vecPlayerPos;
	CVector3 vecEntityPos;
	CVector3 vecOtherEntityPos;
	g_pClient->GetPlayerManager()->GetLocalPlayer()->GetPosition(vecPlayerPos);
	pEntity->GetStreamPosition(vecEntityPos);
	pOther->GetStreamPosition(vecOtherEntityPos);
	return (GetDistanceBetweenPoints3D(vecPlayerPos.fX, vecPlayerPos.fY, vecPlayerPos.fZ, vecEntityPos.fX, vecEntityPos.fY, vecEntityPos.fZ) < GetDistanceBetweenPoints3D(vecPlayerPos.fX, vecPlayerPos.fY, vecPlayerPos.fZ, vecOtherEntityPos.fX, vecOtherEntityPos.fY, vecOtherEntityPos.fZ));
}

// TODO: Notify server of stream in/out

void CStreamer::Process()
{
	unsigned long ulTime = SharedUtility::GetTime();

	if((ulTime - m_ulLastStreamTime) > STREAMING_TICK)
	{
		m_ulLastStreamTime = ulTime;

		std::list<CStreamableEntity *> newEntities[ENTITY_TYPE_MAX];

		CVector3 vecPlayerPos;
		g_pClient->GetPlayerManager()->GetLocalPlayer()->GetPosition(vecPlayerPos);

		// Loop through all streamable elements
		for(iterator iter = begin(); iter != end(); ++ iter)
		{
			if((*iter)->CanBeStreamedIn())
			{
				// check distance
				CVector3 vecPos;
				(*iter)->GetStreamPosition(vecPos);
				float fDistance = GetDistanceBetweenPoints3D(vecPlayerPos.fX, vecPlayerPos.fY, vecPlayerPos.fZ, vecPos.fX, vecPos.fY, vecPos.fZ);
				bool bInRange = (fDistance <= (*iter)->GetStreamingDistance());

				if(!bInRange || (m_dimensionId != INVALID_DIMENSION_ID && (*iter)->GetDimension() != INVALID_DIMENSION_ID && (m_dimensionId != (*iter)->GetDimension())))
				{
					// out of range or in another dimension, but streamed in?
					if((*iter)->IsStreamedIn())
					{
						CLogFile::Printf("Streamout with Distance %f and Dimension %d/%d", fDistance, m_dimensionId, (*iter)->GetDimension());

						// remove it from the list of streamed in elements
						m_streamedElements[(*iter)->GetType()].remove(*iter);

						// stream it out
						(*iter)->StreamOutInternal();
					}
				}
				else if(bInRange && (m_dimensionId == INVALID_DIMENSION_ID || (*iter)->GetDimension() == INVALID_DIMENSION_ID || (m_dimensionId == (*iter)->GetDimension())))
				{
					// in range and in same/all dimension, but not streamed in?
					if(!(*iter)->IsStreamedIn())
					{
						// flag it for being streamed in (important to have gta's hardcoded limits enforced)
						newEntities[(*iter)->GetType()].push_back(*iter);
					}
				}
			}
			else
			{
				if((*iter)->IsStreamedIn())
				{
					CLogFile::Printf("Streamout due to not being allowed to be streamed in");

					// remove it from the list of streamed in elements
					m_streamedElements[(*iter)->GetType()].remove(*iter);

					// stream it out
					(*iter)->StreamOutInternal();
				}
			}
		}

		// Add all new entites
		for(int i = 0; i < ENTITY_TYPE_MAX; ++i)
		{
			// check if we have entites of that type to stream in
			if(newEntities[i].size() > 0)
			{
				// we have enough space within the GTA engine
				if((m_streamedElements[i].size() + newEntities[i].size()) <= m_uiStreamingLimits[i])
				{
					for(std::list<CStreamableEntity *>::iterator iter = newEntities[i].begin(); iter != newEntities[i].end(); ++ iter)
					{
						// Stream the entity in
						(*iter)->StreamInInternal();

						// add it to our list of streamed in entities
						m_streamedElements[i].push_back(*iter);
					}
				}
				else
				{
					// alone reaching this isn't good - we should stream more entities than GTA can handle so we need to pick the closest ones

					// add our own entites, but do not stream them in
					for(std::list<CStreamableEntity *>::iterator iter = newEntities[i].begin(); iter != newEntities[i].end(); ++ iter)
						m_streamedElements[i].push_back(*iter);

					// sort the list by distance
					m_streamedElements[i].sort(SortStreamableEntites);

					// remove the entities that are too far away
					while(m_streamedElements[i].size() > m_uiStreamingLimits[i])
					{
						// get the furthest away entity
						CStreamableEntity * pEntity = m_streamedElements[i].back();

						// streamed in? (if not, it's new and we don't need to stream it out either)
						if(pEntity->IsStreamedIn())
						{
							// stream it out
							pEntity->StreamOutInternal();
						}

						// remove it from the streamed in entities
						m_streamedElements[i].pop_back();
					}

					// make sure all remaining entities are streamed in
					for(std::list<CStreamableEntity *>::iterator iter = m_streamedElements[i].begin(); iter != m_streamedElements[i].end(); ++iter)
					{
						// not streamed in?
						if(!(*iter)->IsStreamedIn())
						{
							// stream it in
							(*iter)->StreamInInternal();
						}
					}
				}
			}
		}

		//LogMessage(_FILE_, _LINE_, "CStreamer::Pulse (total = %d, time = %dms, veh = %d, pick = %d, obj = %d)", size(), Utility::GetTime() - ulTime, m_streamedElements[STREAM_ENTITY_VEHICLE].size(), m_streamedElements[STREAM_ENTITY_PICKUP].size(), m_streamedElements[STREAM_ENTITY_OBJECT].size());
	}
}

void CStreamer::UpdateInterior(unsigned int uiInterior)
{
	// loop through all entity types
	for(int i = 0; i < ENTITY_TYPE_MAX; ++ i)
	{
		// look through all spawned entites of that type
		for(std::list<CStreamableEntity *>::iterator iter = m_streamedElements[i].begin(); iter != m_streamedElements[i].end(); ++ iter)
		{
			// Update the interior
			(*iter)->UpdateInterior(uiInterior);
		}
	}
}

void CStreamer::ForceStreamIn(CStreamableEntity * pEntity, bool bInstantly)
{
	// TODO: Implement bInstantly (If false done next tick, if true done now)
	// not enough space to stream it in?
	if(m_streamedElements[pEntity->GetType()].size() > m_uiStreamingLimits[pEntity->GetType()])
	{
		// Get the last entity
		CStreamableEntity * pOtherEntity = m_streamedElements[pEntity->GetType()].back();

		// Check if it is streamed in
		if(pOtherEntity->IsStreamedIn())
		{
			// Stream it out
			pOtherEntity->StreamOutInternal();
		}

		// remove it from the list so we'll have space
		m_streamedElements[pEntity->GetType()].pop_back();
	}

	// stream the entity in
	pEntity->StreamInInternal();

	// add it to the list of streamed entities
	m_streamedElements[pEntity->GetType()].push_back(pEntity);
}

void CStreamer::remove(CStreamableEntity * pEntity)
{
	// Remove it from the streamed in elements
	m_streamedElements[pEntity->GetType()].remove(pEntity);

	// remove it from the global list
	std::list<CStreamableEntity *>::remove(pEntity);

	// Force streaming out
	pEntity->StreamOutInternal();
}

void CStreamer::NotifyDimensionChange(CStreamableEntity * pEntity)
{
	// Is the entity streamed in?
	if(pEntity->IsStreamedIn())
	{
		// Is the entity dimension different from our dimension?
		if(pEntity->GetDimension() != m_dimensionId)
		{
			// Stream the entity out
			pEntity->StreamOutInternal();
		}
	}
}

std::list<CStreamableEntity *> * CStreamer::GetStreamedInEntitiesOfType(eEntityType eType)
{
	return &m_streamedElements[eType];
}

CClientPlayer * CStreamer::GetPlayerFromGamePlayerPed(IVPlayerPed * pGamePlayerPed)
{
	// Get the streamed in players list
	std::list <CStreamableEntity *> * m_streamedVehicles = &m_streamedElements[ENTITY_TYPE_PLAYER];

	// Loop through the streamed in players list
	for(std::list<CStreamableEntity *>::iterator iter = m_streamedVehicles->begin(); iter != m_streamedVehicles->end(); iter++)
	{
		// Get the player pointer
		CClientPlayer * pTestPlayer = reinterpret_cast<CClientPlayer *>(*iter);

		// Is this the player we are looking for?
		if(pTestPlayer->GetGamePlayerPed()->GetPlayerPed() == pGamePlayerPed)
			return pTestPlayer;
	}

	// No player found
	return NULL;
}

CClientVehicle * CStreamer::GetVehicleFromGameVehicle(IVVehicle * pGameVehicle)
{
	// Get the streamed in vehicles list
	std::list <CStreamableEntity *> * m_streamedVehicles = &m_streamedElements[ENTITY_TYPE_VEHICLE];

	// Loop through the streamed in vehicles list
	for(std::list<CStreamableEntity *>::iterator iter = m_streamedVehicles->begin(); iter != m_streamedVehicles->end(); iter++)
	{
		// Get the vehicle pointer
		CClientVehicle * pTestVehicle = reinterpret_cast<CClientVehicle *>(*iter);

		// Is this the vehicle we are looking for?
		if(pTestVehicle->GetGameVehicle()->GetVehicle() == pGameVehicle)
			return pTestVehicle;
	}

	// No vehicle found
	return NULL;
}

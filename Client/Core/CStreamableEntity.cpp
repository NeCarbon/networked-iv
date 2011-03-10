//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CStreamableEntity.cpp
// Project: Client
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CStreamableEntity::CStreamableEntity(CStreamer * pStreamer, eEntityType eType, float fDistance)
{
	// set the streamer
	m_pStreamer = pStreamer;

	// set the type
	m_eType = eType;

	// set the streaming distance
	m_fStreamingDistance = fDistance;

	// streamed out by default
	m_bIsStreamedIn = false;

	// no dimension by default
	m_dimensionId = INVALID_DIMENSION_ID;

	// can't be streamed in by default
	m_bCanBeStreamedIn = false;

	// add it to the streamer
	pStreamer->push_back(this);
}

CStreamableEntity::~CStreamableEntity()
{
	if(m_bIsStreamedIn)
	{
		// Nothing we can do at this point, as it'd go only for pure virtual function call with StreamOut()
		CLogFile::Printf("CStreamableEntity::Deleting %p (%d) while it is streamed in\n", this, m_eType);
	}
}

void CStreamableEntity::SetDimension(DimensionId dimensionId)
{
	// NOTE: This should trigger an instant restream of ourselves
	m_dimensionId = dimensionId;
}

void CStreamableEntity::UpdateInterior(unsigned int uiInterior)
{

}

void CStreamableEntity::StreamInInternal()
{
	// check if the entity is streamed in
	if(!m_bIsStreamedIn)
	{
		CLogFile::Printf("CStreamableEntity::Stream In %p (%d)\n", this, m_eType);

		// flag us as streamed in
		m_bIsStreamedIn = true;

		// stream it in
		StreamIn();
	}
}

void CStreamableEntity::StreamOutInternal()
{
	// check if the entity is streamed in
	if(m_bIsStreamedIn)
	{
		CLogFile::Printf("CStreamableEntity::Stream Out %p (%d)\n", this, m_eType);

		// stream it out
		StreamOut();

		// flag us as streamed out
		m_bIsStreamedIn = false;
	}
}

void CStreamableEntity::OnDelete()
{
	// remove it from the streamer
	m_pStreamer->remove(this);
}

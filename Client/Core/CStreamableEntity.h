//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CStreamableEntity.h
// Project: Client
// Author(s): jenksta
//            mabako
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// Type used for dimension ids
typedef unsigned char DimensionId;

// Define used for invalid dimension ids
#define INVALID_DIMENSION_ID 0xFF

// TODO: Adjust?
#define STREAMING_TICK 1000

class CStreamer;

class CStreamableEntity
{
	friend class CStreamer;

private:
	CStreamer *       m_pStreamer;
	float             m_fStreamingDistance;
	bool              m_bIsStreamedIn;
	eEntityType       m_eType;
	DimensionId       m_dimensionId;
	bool              m_bCanBeStreamedIn;

	void              StreamInInternal();
	void              StreamOutInternal();

public:
	CStreamableEntity(CStreamer * m_pStreamer, eEntityType eType, float fDistance = 200.0f);
	virtual ~CStreamableEntity();

	float             GetStreamingDistance() { return m_fStreamingDistance; }
	bool              IsStreamedIn() { return m_bIsStreamedIn; }
	eEntityType       GetType() { return m_eType; }
	void              SetDimension(DimensionId dimensionId);
	DimensionId       GetDimension() { return m_dimensionId; }
	void              SetCanBeStreamedIn(bool bCanBeStreamedIn) { m_bCanBeStreamedIn = bCanBeStreamedIn; }
	bool              CanBeStreamedIn() { return m_bCanBeStreamedIn; }

	virtual void      GetStreamPosition(Vector3& vecCoordinates) = 0;
	virtual void      UpdateInterior(unsigned int uiInterior);

	virtual void      StreamIn() = 0;
	virtual void      StreamOut() = 0;

protected:
	void              OnDelete();
};

//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVModelInfo.h
// Project: Client
// Author(s): jenksta
//            Multi Theft Auto Team
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

enum eVehicleType
{
	VEHICLE_TYPE_AUTOMOBILE,
	VEHICLE_TYPE_BIKE,
	VEHICLE_TYPE_BOAT,
	VEHICLE_TYPE_TRAIN,
	VEHICLE_TYPE_HELI,
	VEHICLE_TYPE_PLANE
};

class IVModelInfo
{
public:
	PAD(IVModelInfo, pad0, 0x3C);
	DWORD dwHash;
	PAD(IVModelInfo, pad1, 0x16);
	WORD  wAnimIndex;
	PAD(IVModelInfo, pad2, 0x14);
	DWORD dwVehicleType;
	// TODO: Find size
};

class CIVModelInfo
{
private:
	IVModelInfo * m_pModelInfo;
	int           m_iModelIndex;

public:
	CIVModelInfo();
	CIVModelInfo(int iModelIndex);
	~CIVModelInfo();

	void          SetIndex(int iModelIndex);
	int           GetIndex() { return m_iModelIndex; }
	bool          IsValid();
	bool          IsLoaded();
	void          Load(bool bWaitForLoad = true);
	void          Unload();
	DWORD         GetHash();
	WORD          GetAnimIndex();
	bool          IsAutomobile();
	bool          IsBike();
	bool          IsBoat();
	bool          IsTrain();
	bool          IsHeli();
	bool          IsPlane();
};

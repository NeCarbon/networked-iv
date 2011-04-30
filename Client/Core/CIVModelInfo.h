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

#define ARRAY_ModelInfos 0x15F73B0
// void CModelInfo::SetAnimation(this, char * szAnimationName)
#define FUNC_CModelInfo__SetAnimation 0x98F3F0

enum eModelType
{
	MODEL_TYPE_BASE = 1, // Base and Instance
	MODEL_TYPE_MLO = 2,
	MODEL_TYPE_TIME = 3,
	MODEL_TYPE_WEAPON = 4,
	MODEL_TYPE_VEHICLE = 5,
	MODEL_TYPE_PED = 6
};

enum eVehicleType
{
	VEHICLE_TYPE_AUTOMOBILE,
	VEHICLE_TYPE_BIKE,
	VEHICLE_TYPE_BOAT,
	VEHICLE_TYPE_TRAIN,
	VEHICLE_TYPE_HELI,
	VEHICLE_TYPE_PLANE
};

class IVBaseModelInfoVFTable
{
public:
	DWORD ScalarDeletingDestructor;
	DWORD m4;
	DWORD m8;
	DWORD GetType;
	DWORD m10;
	DWORD m14;
	DWORD m18;
	DWORD m1C;
	DWORD m20;
	DWORD m24;
	DWORD m28;
	DWORD m2C;
	DWORD m30;
	DWORD m34;
	DWORD m38;
};

class IVInstanceModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVMloModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVTimeModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVWeaponModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVVehicleModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVPedModelInfoVFTable : public IVBaseModelInfoVFTable
{
	// Empty
};

class IVBaseModelInfo
{
public:
	IVBaseModelInfoVFTable * m_VFTable; // 00-04
	PAD(IVBaseModelInfo, pad0, 0x38);   // 04-3C
	DWORD dwHash;                       // 3C-40
	PAD(IVBaseModelInfo, pad1, 0x4);    // 40-44
	DWORD dwReferenceCount;             // 44-48
	PAD(IVBaseModelInfo, pad2, 0xE);    // 48-56
	WORD  wAnimIndex;                   // 56-58
	// 0x60 in IVWeaponModelInfo is IVWeaponInfo * m_pWeaponInfo
	PAD(IVBaseModelInfo, pad3, 0x14);   // 58-6C
	DWORD dwVehicleType;                // 6C-70
	// 0xCC - IVVehicleStruct * m_pVehicleStruct (Vehicle Struct Size = 0x20C)
	// TODO: Find size(s)
};

class CIVModelInfo
{
private:
	int m_iModelIndex;

public:
	CIVModelInfo();
	CIVModelInfo(int iModelIndex);
	~CIVModelInfo();

	IVBaseModelInfo * GetModelInfo();
	void              SetIndex(int iModelIndex);
	int               GetIndex() { return m_iModelIndex; }
	bool              IsValid() { return (GetModelInfo() != NULL); }
	BYTE              GetType();
	bool              IsBase() { return (GetType() == MODEL_TYPE_BASE); }
	bool              IsInstance() { return (GetType() == MODEL_TYPE_BASE); }
	bool              IsMlo() { return (GetType() == MODEL_TYPE_MLO); }
	bool              IsTime() { return (GetType() == MODEL_TYPE_TIME); }
	bool              IsWeapon() { return (GetType() == MODEL_TYPE_WEAPON); }
	bool              IsVehicle() { return (GetType() == MODEL_TYPE_VEHICLE); }
	bool              IsPed() { return (GetType() == MODEL_TYPE_PED); }
	bool              IsLoaded();
	void              Load(bool bWaitForLoad = true);
	void              Unload();
	DWORD             GetHash();
	// TODO: Implement our own reference counting system to override this
	void              AddReference();
	void              RemoveReference();
	DWORD             GetReferenceCount();
	WORD              GetAnimIndex();
	bool              IsAutomobile();
	bool              IsBike();
	bool              IsBoat();
	bool              IsTrain();
	bool              IsHeli();
	bool              IsPlane();
};

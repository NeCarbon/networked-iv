//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CClientPlayer.h
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

#define TASK_ENTER_CAR_AS_DRIVER    0x5BF03315
#define TASK_ENTER_CAR_AS_PASSENGER 0x0A2C70AF
#define WARP_CHAR_FROM_CAR_TO_COORD 0x6A77506A

class CClientVehicle;

class CClientPlayer : public CStreamableEntity, public CEntity
{
private:
	bool              m_bIsLocalPlayer;
	EntityId          m_playerId;
	String            m_strPlayerName;
	BYTE              m_byteInternalPlayerNumber;
	CIVPlayerPed *    m_pPlayerPed;
	CIVPlayerInfo *   m_pPlayerInfo;
	CContextData *    m_pContextData;
	CIVModelInfo    * m_pModelInfo;
	CNetworkPadState  m_previousNetPadState;
	CNetworkPadState  m_currentNetPadState;
	struct
	{
		struct
		{
			CVector3       vecStart;
			CVector3       vecTarget;
			CVector3       vecError;
			float         fLastAlpha;
			unsigned long ulStartTime;
			unsigned long ulFinishTime;
		} pos;
	}                 m_interp;
	CVector3           m_vecPosition;
	float             m_fCurrentHeading;
	float             m_fDesiredHeading;
	CVector3           m_vecMoveSpeed;
	unsigned int      m_uiHealth;
	DWORD             m_dwWeaponType;
	DWORD             m_dwAmmo;
	CClientVehicle  * m_pVehicle;
	BYTE              m_byteVehicleSeatId;
	struct
	{
		bool             bEntering;
		CClientVehicle * pVehicle;
		BYTE             byteSeatId;
		bool             bExiting;
		unsigned long    ulKeyHoldStartTime;
		bool             bRequesting;
	}                 m_vehicleEnterExit;
	bool              m_bDucking;

	bool             Create();
	void             Destroy();

public:
	CClientPlayer(bool bIsLocalPlayer);
	~CClientPlayer();

	bool             IsLocalPlayer() { return m_bIsLocalPlayer; }
	bool             IsNetworkPlayer() { return (m_playerId != INVALID_ENTITY_ID); }
	void             SetPlayerId(EntityId playerId) { m_playerId = playerId; }
	EntityId         GetPlayerId() { return m_playerId; }
	void             SetPlayerName(String strPlayerName) { m_strPlayerName = strPlayerName; }
	String           GetPlayerName() { return m_strPlayerName; }
	BYTE             GetInternalPlayerNumber() { return m_byteInternalPlayerNumber; }
	CIVPlayerPed *   GetGamePlayerPed() { return m_pPlayerPed; }
	bool             IsSpawned() { return (m_pPlayerPed != NULL); }
	CIVModelInfo   * GetModelInfo() { return m_pModelInfo; }
	void             StreamIn();
	void             StreamOut();
	void             Process();
	unsigned int     GetScriptingHandle();

	bool             InternalIsInVehicle();
	CClientVehicle * InternalGetVehicle();
	bool             IsOnFoot() { return (m_pVehicle == NULL); }
	bool             IsInVehicle() { return (m_pVehicle != NULL); }
	CClientVehicle * GetVehicle() { return m_pVehicle; }
	BYTE             GetVehicleSeatId() { return m_byteVehicleSeatId; }
	CClientVehicle * GetEnterExitVehicle() { return m_vehicleEnterExit.pVehicle; }

	bool             SetModel(int iModelIndex);

	void             SetNetPadState(const CNetworkPadState& netPadState);
	void             GetPreviousNetPadState(CNetworkPadState& netPadState);
	void             GetNetPadState(CNetworkPadState& netPadState);

	void             SetAimData(const Matrix& matAim);
	void             GetAimData(Matrix& matAim);

	// Health/armour
	void             SetHealth(unsigned int uiHealth);
	unsigned int     GetHealth();

	void             Teleport(const CVector3& vecPosition);
	void             SetPosition(const CVector3& vecPosition);
	void             GetPosition(CVector3& vecPosition);
	void             GetStreamPosition(CVector3& vecPosition) { GetPosition(vecPosition); }
	void             SetCurrentHeading(float fCurrentHeading);
	float            GetCurrentHeading();
	void             SetDesiredHeading(float fDesiredHeading);
	float            GetDesiredHeading();
	void             SetMoveSpeed(const CVector3& vecMoveSpeed);
	void             GetMoveSpeed(CVector3& vecMoveSpeed);

	// Weapons
	void             GiveWeapon(DWORD dwWeaponType, DWORD dwAmmo);
	void             RemoveWeapon(DWORD dwWeaponType);
	void             RemoveAllWeapons();
	void             SetCurrentWeapon(DWORD dwWeaponType);
	eWeaponSlot      GetCurrentWeaponSlot();
	DWORD            GetCurrentWeaponType();
	void             SetWeaponAmmo(DWORD dwWeaponType, DWORD dwAmmo);
	DWORD            GetWeaponAmmo(DWORD dwWeaponType);

	// Ducking
	void             SetDucking(bool bDucking);
	bool             IsDucking();

	// Task control
	void             ClearPrimaryTask(bool bImmidiately);

	// Vehicle entry/exit tasks
	bool             IsGettingInToAVehicle();
	bool             IsGettingOutOfAVehicle();
	bool             IsJackingAVehicle();
	bool             IsGettingJackedFromVehicle();

	// Vehicle entry/exit
	bool             GetClosestVehicle(bool bPassenger, CClientVehicle ** pVehicle, BYTE& byteSeatId);
	void             EnterVehicle(CClientVehicle * pVehicle, BYTE byteSeatId);
	void             ExitVehicle(bool bWarp);
	void             PutInVehicle(CClientVehicle * pVehicle, BYTE byteSeatId);
	void             CheckVehicleEntryExitKey();
	void             ProcessVehicleEntryExit();
	void             ResetVehicleEnterExit();
	bool             HasVehicleEnterExit() { return (m_vehicleEnterExit.bEntering || m_vehicleEnterExit.bExiting); }
	void             SetRequestingVehicleEnterExit(bool bRequesting) { m_vehicleEnterExit.bRequesting = bRequesting; }
	bool             GetRequestingVehicleEnterExit() { return m_vehicleEnterExit.bRequesting; }

	// Time based linear interpolation
	void             UpdateTargetPosition();
	void             Interpolate();
	void             SetTargetPosition(const CVector3& vecPosition, unsigned long ulDelay);
	void             RemoveTargetPosition();
	void             ResetInterpolation();
	bool             HasTargetPosition() { return (m_interp.pos.ulFinishTime != 0); }

	// Information Serialization/Deserialization
	void             Serialize(CBitStreamInterface * pBitStream);
	bool             Deserialize(CBitStreamInterface * pBitStream);
};

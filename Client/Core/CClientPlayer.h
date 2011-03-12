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
#define TASK_LEAVE_CAR              0x6B85214E

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
			Vector3       vecStart;
			Vector3       vecTarget;
			Vector3       vecError;
			float         fLastAlpha;
			unsigned long ulStartTime;
			unsigned long ulFinishTime;
		} pos;
	}                 m_interp;
	Vector3           m_vecPosition;
	Vector3           m_vecRotation;
	Vector3           m_vecMoveSpeed;
	Vector3           m_vecTurnSpeed;
	unsigned int      m_uiHealth;
	DWORD             m_dwWeaponType;
	DWORD             m_dwAmmo;
	CClientVehicle  * m_pVehicle;
	struct
	{
		bool              bEntering;
		CClientVehicle * pVehicle;
		BYTE              byteSeatId;
		bool              bExiting;
		unsigned long     ulKeyHoldStartTime;
	}                 m_vehicleEnterExit;

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
	bool             IsInVehicle() { return (m_pVehicle != NULL); }
	CClientVehicle * GetVehicle() { return m_pVehicle; }
	CClientVehicle * GetEnterExitVehicle() { return m_vehicleEnterExit.pVehicle; }

	bool             SetModel(int iModelIndex);
	bool             SetModelByHash(DWORD dwModelHash);

	void             SetNetPadState(const CNetworkPadState& netPadState);
	void             GetPreviousNetPadState(CNetworkPadState& netPadState);
	void             GetNetPadState(CNetworkPadState& netPadState);

	void             SetAimData(const Matrix& matAim);
	void             GetAimData(Matrix& matAim);

	// Health/armour
	void             SetHealth(unsigned int uiHealth);
	unsigned int     GetHealth();

	void             Teleport(const Vector3& vecPosition);
	void             SetPosition(const Vector3& vecPosition);
	void             GetPosition(Vector3& vecPosition);
	void             GetStreamPosition(Vector3& vecPosition) { GetPosition(vecPosition); }
	void             SetRotation(const Vector3& vecRotation);
	void             GetRotation(Vector3& vecRotation);
	void             SetMoveSpeed(const Vector3& vecMoveSpeed);
	void             GetMoveSpeed(Vector3& vecMoveSpeed);
	void             SetTurnSpeed(const Vector3& vecTurnSpeed);
	void             GetTurnSpeed(Vector3& vecTurnSpeed);

	// Weapons
	void             GiveWeapon(DWORD dwWeaponType, DWORD dwAmmo);
	void             RemoveWeapon(DWORD dwWeaponType);
	void             RemoveAllWeapons();
	void             SetCurrentWeapon(DWORD dwWeaponType);
	eWeaponSlot      GetCurrentWeaponSlot();
	DWORD            GetCurrentWeaponType();
	void             SetWeaponAmmo(DWORD dwWeaponType, DWORD dwAmmo);
	DWORD            GetWeaponAmmo(DWORD dwWeaponType);

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

	// Time based linear interpolation
	void             UpdateTargetPosition();
	void             Interpolate();
	void             SetTargetPosition(const Vector3& vecPosition, unsigned long ulDelay);
	void             RemoveTargetPosition();
	void             ResetInterpolation();
	bool             HasTargetPosition() { return (m_interp.pos.ulFinishTime != 0); }
};

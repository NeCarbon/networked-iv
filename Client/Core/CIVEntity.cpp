//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CIVEntity.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

extern CClient * g_pClient;

CIVEntity::CIVEntity()
{
	m_pEntity = NULL;
}

CIVEntity::CIVEntity(IVEntity * pEntity)
{
	m_pEntity = pEntity;
}

CIVEntity::~CIVEntity()
{

}

void CIVEntity::SetEntity(IVEntity * pEntity)
{
	m_pEntity = pEntity;
}

IVEntity * CIVEntity::GetEntity()
{
	return m_pEntity;
}

void CIVEntity::SetMatrix(Matrix * matMatrix)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
	{
		memcpy(&m_pEntity->m_pMatrix->vecRight, &matMatrix->vecRight, sizeof(CVector3));
		memcpy(&m_pEntity->m_pMatrix->vecFront, &matMatrix->vecFront, sizeof(CVector3));
		memcpy(&m_pEntity->m_pMatrix->vecUp, &matMatrix->vecUp, sizeof(CVector3));
		memcpy(&m_pEntity->m_pMatrix->vecPosition, &matMatrix->vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::GetMatrix(Matrix * matMatrix)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
	{
		memcpy(&matMatrix->vecRight, &m_pEntity->m_pMatrix->vecRight, sizeof(CVector3));
		memcpy(&matMatrix->vecFront, &m_pEntity->m_pMatrix->vecFront, sizeof(CVector3));
		memcpy(&matMatrix->vecUp, &m_pEntity->m_pMatrix->vecUp, sizeof(CVector3));
		memcpy(&matMatrix->vecPosition, &m_pEntity->m_pMatrix->vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::SetPosition(CVector3 * vecPosition)
{
	if(m_pEntity)
	{
		if(m_pEntity->m_pMatrix)
			memcpy(&m_pEntity->m_pMatrix->vecPosition, vecPosition, sizeof(CVector3));
		else
			memcpy(&m_pEntity->m_vecPosition, vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::GetPosition(CVector3 * vecPosition)
{
	if(m_pEntity)
	{
		if(m_pEntity->m_pMatrix)
			memcpy(vecPosition, &m_pEntity->m_pMatrix->vecPosition, sizeof(CVector3));
		else
			memcpy(vecPosition, &m_pEntity->m_vecPosition, sizeof(CVector3));
	}
}

void CIVEntity::SetRoll(CVector3 * vecRoll)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
			memcpy(&m_pEntity->m_pMatrix->vecRight, vecRoll, sizeof(CVector3));
}

void CIVEntity::GetRoll(CVector3 * vecRoll)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(vecRoll, &m_pEntity->m_pMatrix->vecRight, sizeof(CVector3));
}

void CIVEntity::SetDirection(CVector3 * vecDirection)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(&m_pEntity->m_pMatrix->vecFront, vecDirection, sizeof(CVector3));
}

void CIVEntity::GetDirection(CVector3 * vecDirection)
{
	if(m_pEntity && m_pEntity->m_pMatrix)
		memcpy(vecDirection, &m_pEntity->m_pMatrix->vecFront, sizeof(CVector3));
}

void CIVEntity::SetModelIndex(WORD wModelIndex)
{
	if(m_pEntity)
	{
		DWORD dwFunc = m_pEntity->m_VFTable->SetModelIndex;
		int iModelIndex = wModelIndex;
		_asm
		{
			push /*w*/iModelIndex
			call dwFunc
			add esp, 4
		}
	}
}

WORD CIVEntity::GetModelIndex()
{
	if(m_pEntity)
		return m_pEntity->m_wModelIndex;

	return 0;
}

void CIVEntity::SetAlpha(BYTE byteAlpha)
{
	if(m_pEntity)
		m_pEntity->m_byteAlpha = byteAlpha;
}

BYTE CIVEntity::GetAlpha()
{
	if(m_pEntity)
		return m_pEntity->m_byteAlpha;

	return 0;
}

bool CIVEntity::IsTouchingEntity(IVEntity * pTouchingEntity)
{
	if(m_pEntity)
	{
		DWORD dwFunc = (g_pClient->GetBaseAddress() + FUNC_CEntity__IsTouchingEntity_7);
		IVEntity * pEntity = m_pEntity;
		bool bResult = false;
		_asm
		{
			push pTouchingEntity
			push pEntity
			call dwFunc
			mov bResult, al
		}
		return bResult;
	}

	return false;
}

void  CIVEntity::AddToWorld()
{
	if(m_pEntity)
		CIVWorld::Add(m_pEntity);
}

void CIVEntity::RemoveFromWorld()
{
	if(m_pEntity)
		CIVWorld::Remove(m_pEntity);
}

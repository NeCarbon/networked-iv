//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CMath.h
// Project: Shared
// Author(s): jenksta
//            Multi Theft Auto Team
// License: See LICENSE in root directory
//
//==========================================================================
// TODO: Class'ify functions

#pragma once

#include <StdInc.h>
#include <math.h>

// From Bullet
#define DOUBLE_PI    6.283185307179586232f
#define PI           (DOUBLE_PI * 0.5f)
#define HALF_PI      (DOUBLE_PI * 0.25f)
#define RADS_PER_DEG (DOUBLE_PI / 360.0f)
#define DEGS_PER_RAD (360.0f / DOUBLE_PI)

// From Multi Theft Auto
static float WrapAround(float fValue, float fHigh)
{
	return fValue - (fHigh * floor((float)(fValue / fHigh)));
}

// From Multi Theft Auto
static float ConvertRadiansToDegrees(float fRotation)
{
	return WrapAround((float)(fRotation * 180.0f / PI + 360.0f), 360.0f);
}

static CVector3 ConvertRadiansToDegrees(CVector3& vecRotation)
{
	vecRotation.fX = ConvertRadiansToDegrees(vecRotation.fX);
	vecRotation.fY = ConvertRadiansToDegrees(vecRotation.fY);
	vecRotation.fZ = ConvertRadiansToDegrees(vecRotation.fZ);
	return vecRotation;
}

// From Multi Theft Auto
static float ConvertDegreesToRadians(float fRotation)
{
	return WrapAround((float)(fRotation * PI / 180.0f + 2 * PI), (float)(2 * PI));
}

static CVector3 ConvertDegreesToRadians(CVector3& vecRotation)
{
	vecRotation.fX = ConvertDegreesToRadians(vecRotation.fX);
	vecRotation.fY = ConvertDegreesToRadians(vecRotation.fY);
	vecRotation.fZ = ConvertDegreesToRadians(vecRotation.fZ);
	return vecRotation;
}

// From Multi Theft Auto
template<class T>
static T Lerp(const T& start, float fAlpha, const T& end)
{
	return (end - start) * fAlpha + start;
}

// From Multi Theft Auto
// Clamps a value between two other values
template<class T>
static T Clamp(const T& min, const T& a, const T& max)
{
	if(a < min)
		return min;

	if(a > max)
		return max;

	return a;
}

// From Multi Theft Auto
// Find the relative position of fPos between fFrom and fTo
static const float Unlerp(const double fStart, const double fPos, const double fEnd)
{
	// Avoid dividing by 0 (results in INF values)
	if(fStart == fEnd) return 1.0f;

	return (float)((fPos - fStart) / (fEnd - fStart));
}

// Find the distance between two 2D points
static float GetDistanceBetweenPoints2D(float x, float y, float xx, float yy) 
{
	float newx = (xx - x);
	float newy = (yy - y);
	return sqrt(newx * newx + newy * newy);
}

// Find the distance between two 3D points
static float GetDistanceBetweenPoints3D(float x, float y, float z, float xx, float yy, float zz)
{
	float newx = (xx - x);
	float newy = (yy - y);
	float newz = (zz - z);
	return sqrt(newx * newx + newy * newy + newz * newz);
}

// TODO: Class'ify in its own file
class Vector4
{
public:
	float fX; // 00-04
	float fY; // 04-08
	float fZ; // 08-0C
	float fW; // 0C-10

	Vector4()
	{
		fX = fY = fZ = 0.0f;
	}

	Vector4(float _fX, float _fY, float _fZ, float _fW)
	{
		fX = _fX; fY = _fY; fZ = _fZ; fW = _fW;
	}
};

// TODO: Class'ify in its own file
class Matrix
{
public:
	CVector3 vecRight;
	CVector3 vecFront;
	CVector3 vecUp;
	CVector3 vecPosition;

	Matrix()
	{
		Identity();
	}

	Matrix(CVector3 _vecRight, CVector3 _vecFront, CVector3 _vecUp, CVector3 _vecPosition)
	{
		vecRight = _vecRight;
		vecFront = _vecFront;
		vecUp = _vecUp;
		vecPosition = _vecPosition;
	}

	void Identity()
	{
		vecRight = CVector3(1.0f, 0.0f, 0.0f);
		vecFront = CVector3(0.0f, 1.0f, 0.0f);
		vecUp = CVector3(0.0f, 0.0f, 1.0f);
		vecPosition = CVector3(0.0f, 0.0f, 0.0f);
	}
};

// TODO: Class'ify in its own file
class Matrix34
{
public:
	CVector3 vecRight;      // 00-0C
	DWORD    dwPadRight;    // 0C-10
	CVector3 vecFront;      // 10-1C
	DWORD    dwPadFront;    // 1C-20
	CVector3 vecUp;         // 20-2C
	DWORD    dwPadUp;       // 2C-30
	CVector3 vecPosition;   // 30-3C
	DWORD    dwPadPosition; // 3C-40

	Matrix34()
	{
		memset(this, 0, sizeof(Matrix34));
	}

	void ToMatrix(Matrix * matMatrix) const
	{
		memcpy(&matMatrix->vecRight, &vecRight, sizeof(CVector3));
		memcpy(&matMatrix->vecFront, &vecFront, sizeof(CVector3));
		memcpy(&matMatrix->vecUp, &vecUp, sizeof(CVector3));
		memcpy(&matMatrix->vecPosition, &vecPosition, sizeof(CVector3));
	}

	void FromMatrix(Matrix * matMatrix)
	{
		memcpy(&vecRight, &matMatrix->vecRight, sizeof(CVector3));
		memcpy(&vecFront, &matMatrix->vecFront, sizeof(CVector3));
		memcpy(&vecUp, &matMatrix->vecUp, sizeof(CVector3));
		memcpy(&vecPosition, &matMatrix->vecPosition, sizeof(CVector3));
	}
};

//-----------------------------------------------------------------------------
//
// File Name:	Vector2D.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "Vector2D.h"
#define _USE_MATH_DEFINES
#include <math.h>


void Vector2DZero(Vector2D* pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

void Vector2DSet(Vector2D* pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

void Vector2DNeg(Vector2D* pResult, const Vector2D* pVec0)
{
	pResult->x = pVec0->x * -1;
	pResult->y = pVec0->y * -1;
}

void Vector2DAdd(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1)
{
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
}

void Vector2DSub(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1)
{
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
}

void Vector2DNormalize(Vector2D* pResult, const Vector2D* pVec0)
{
	float leng = Vector2DLength(pVec0);
	pResult->x = pVec0->x / leng;
	pResult->y = pVec0->y / leng;
}

void Vector2DScale(Vector2D* pResult, const Vector2D* pVec0, float scale)
{
	pResult->x = pVec0->x * scale;
	pResult->y = pVec0->y * scale;
}

void Vector2DScaleAdd(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1, float scale)
{
	pResult->x = (pVec0->x * scale) + pVec1->x;
	pResult->y = (pVec0->y * scale) + pVec1->y;
}

void Vector2DScaleSub(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1, float scale)
{
	pResult->x = (pVec0->x * scale) - pVec1->x;
	pResult->y = (pVec0->y * scale) - pVec1->y;
}

float Vector2DLength(const Vector2D* pVec0)
{
	return (float)sqrt(((double)pVec0->x * (double)pVec0->x) + ((double)pVec0->y * (double)pVec0->y));
}

float Vector2DSquareLength(const Vector2D* pVec0)
{
	float squareLeng = 0;
	squareLeng += (pVec0->x * pVec0->x);
	squareLeng += (pVec0->y * pVec0->y);
	return squareLeng;
}

float Vector2DDistance(const Vector2D* pVec0, const Vector2D* pVec1)
{
	return (float)sqrt((((double)pVec0->x - (double)pVec1->x) * ((double)pVec0->x - (double)pVec1->x)) + (((double)pVec0->y - (double)pVec1->y) * ((double)pVec0->y - (double)pVec1->y)));
}

float Vector2DSquareDistance(const Vector2D* pVec0, const Vector2D* pVec1)
{
	return ((pVec0->x - pVec1->x) * (pVec0->x - pVec1->x)) + ((pVec0->y - pVec1->y) * (pVec0->y - pVec1->y));
}

float Vector2DDotProduct(const Vector2D* pVec0, const Vector2D* pVec1)
{
	return (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
}

void Vector2DFromAngleDeg(Vector2D* pResult, float angle)
{
	angle = (angle * (float) M_PI) / 180.0f;
	pResult->x = cosf(angle);
	pResult->y = sinf(angle);
}

void Vector2DFromAngleRad(Vector2D* pResult, float angle)
{
	pResult->x = cosf(angle);
	pResult->y = sinf(angle);
}

float Vector2DToAngleRad(const Vector2D* pVec)
{
	if (pVec != NULL)
	{
		return atan2f(pVec->y, pVec->x);
	}
	else
	{
		return 0;
	}
}
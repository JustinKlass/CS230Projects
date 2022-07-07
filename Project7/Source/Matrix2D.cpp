//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2D.h"
#include "Vector2D.h"
#define _USE_MATH_DEFINES
#include <math.h>

void Matrix2DIdentity(Matrix2D* pResult)
{
	int row, col;

	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			if (col == row)
			{
				pResult->m[row][col] = 1;
			}
			else
			{
				pResult->m[row][col] = 0;
			}
		}
	}
}

void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx)
{
	int row, col;
	Matrix2D pMtxCopy = *pMtx;

	for(row = 0; row < 3; row++)
	{
		for(col = 0; col < 3; col++)
		{
			pResult->m[col][row] = pMtxCopy.m[row][col];
		}
	}
}

void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1)
{

	int newMatrix, row, col;
	Matrix2D pMtxCopy0 = *pMtx0;
	Matrix2D pMtxCopy1 = *pMtx1;

	for(newMatrix = 0; newMatrix < 3; newMatrix++)
	{
		for(row = 0; row < 3; row++)
		{
			pResult->m[newMatrix][row] = 0;
			for(col = 0; col < 3; col++)
			{
				pResult->m[newMatrix][row] += pMtxCopy0.m[newMatrix][col] * pMtxCopy1.m[col][row];
			}
		}
	}
}

void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
	if(pResult) 
	{
		Matrix2DIdentity(pResult);
		pResult->m[0][2] = x;
		pResult->m[1][2] = y;
	}
}

void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
	if (pResult)
	{
		Matrix2DIdentity(pResult);
		pResult->m[0][0] = x;
		pResult->m[1][1] = y;
	}
}

void Matrix2DRotDeg(Matrix2D* pResult, float angle)
{
	float rad = (float)(angle * M_PI) / 180.0f;
	Matrix2DRotRad(pResult, rad);
	
}

void Matrix2DRotRad(Matrix2D* pResult, float angle)
{
	if (pResult)
	{
		Matrix2DIdentity(pResult);
		pResult->m[0][0] = cosf(angle);
		pResult->m[0][1] = -sinf(angle);
		pResult->m[1][0] = sinf(angle);
		pResult->m[1][1] = cosf(angle);
	}
}

void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec)
{
	float xSum = 0.0f, ySum = 0.0f, zSum = 0.0f;

	xSum = (pMtx->m[0][0] * pVec->x) + (pMtx->m[0][1] * pVec->y) + pMtx->m[0][2];
	ySum = (pMtx->m[1][0] * pVec->x) + (pMtx->m[1][1] * pVec->y) + pMtx->m[1][2];

	pResult->x = xSum / zSum;
	pResult->y = ySum / zSum;
}

/*
[1, 0, 0]   [x]   [x1]
[0, 1, 0] * [y] = [y1]
[0, 0, 1]   [1]   [1]


*/
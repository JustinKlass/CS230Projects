//-----------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "SpriteSource.h"

typedef struct SpriteSource
{
	// The dimensions of the sprite sheet.
	int	numRows;
	int	numCols;

	// Pointer to a texture created using the Alpha Engine.
	AEGfxTexturePtr	pTexture;

} SpriteSource;

SpriteSourcePtr SpriteSourceCreate(int numCols, int numRows, AEGfxTexturePtr pTexture)
{
	SpriteSourcePtr newSpriteSource = calloc(1, sizeof(SpriteSource));

	if (newSpriteSource != NULL)
	{
		newSpriteSource->numRows = numRows;
		newSpriteSource->numCols = numCols;
		newSpriteSource->pTexture = pTexture;
		return newSpriteSource;
	}
	else
	{
		return NULL;
	}
}

void SpriteSourceFree(SpriteSourcePtr* spriteSource)
{
	if(*spriteSource != NULL)
	{
		free(*spriteSource);
		*spriteSource = NULL;
	}
}

AEGfxTexturePtr SpriteSourceGetTexture(SpriteSourcePtr spriteSource)
{
	if(spriteSource != NULL)
	{
		return spriteSource->pTexture;
	}
	else
	{
		return NULL;
	}
}

unsigned int SpriteSourceGetFrameCount(SpriteSourcePtr spriteSource)
{
	if (spriteSource != NULL)
	{
		return spriteSource->numCols * spriteSource->numRows;
	}
	else
	{
		return 0;
	}
}

void SpriteSourceGetUV(SpriteSourcePtr spriteSource, unsigned int frameIndex, float* u, float* v)
{	
	float uSize = 1.0f / spriteSource->numCols;
	float vSize = 1.0f / spriteSource->numRows;

	*u = uSize * (frameIndex % spriteSource->numCols);
	*v = vSize * (frameIndex / spriteSource->numCols);
}
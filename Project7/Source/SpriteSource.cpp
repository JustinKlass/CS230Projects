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
#include "AEEngine.h"
#include "SpriteSource.h"
#include "Stream.h"

typedef struct SpriteSource
{
	// The name of the sprite source.
	// A buffer is used to allow each sprite source to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	char name[32];

	// The dimensions of the sprite sheet.
	int	numRows;
	int	numCols;

	// Pointer to a texture created using the Alpha Engine.
	AEGfxTexturePtr	pTexture;

} SpriteSource;

SpriteSourcePtr SpriteSourceCreate(int numCols, int numRows, AEGfxTexturePtr pTexture)
{
	SpriteSourcePtr newSpriteSource = (SpriteSourcePtr)calloc(1, sizeof(SpriteSource));

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
	if(spriteSource != NULL && *spriteSource != NULL)
	{
		if(SpriteSourceGetTexture(*spriteSource) != NULL)
		{
			AEGfxTextureUnload(SpriteSourceGetTexture(*spriteSource));
		}

		free(*spriteSource);
		*spriteSource = NULL;
	}
}

SpriteSourcePtr SpriteSourceRead(Stream stream)
{
	// Get name
	if(stream != NULL)
	{	
		// Read token for name
		const char* spriteSourceName = StreamReadToken(stream);


		if(strncmp(spriteSourceName, "SpriteSource", _countof("SpriteSource")) == 0)
		{
			const char* name = StreamReadToken(stream);
			SpriteSourcePtr spriteSource = SpriteSourceCreate(0, 0, NULL);
			strncpy_s(spriteSource->name, _countof(spriteSource->name), name, _countof(spriteSource->name));

			int numCols = StreamReadInt(stream);
			spriteSource->numCols = numCols;

			int numRows = StreamReadInt(stream);
			spriteSource->numRows = numRows;

			const char* texture = StreamReadToken(stream);
			spriteSource->pTexture = AEGfxTextureLoad(texture);
			// Read token and use AEGFXtextureload


			return spriteSource;
		}
	}

	return NULL;
}

bool SpriteSourceIsNamed(SpriteSourcePtr spriteSource, const char* name)
{
	if(spriteSource != NULL && name != NULL)
	{
		if(strncmp(name, spriteSource->name, _countof(spriteSource->name)) == 0)
		{
			return true;
		}
	}

	return false;
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
//-----------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Stream.h"
#include "AEEngine.h"
#include "AEMath.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Trace.h"

typedef struct Sprite
{
	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	SpriteSourcePtr spriteSource;

	// The mesh used to draw the sprite.
	AEGfxVertexList* mesh;

} Sprite;

SpritePtr SpriteCreate(void)
{
	SpritePtr newSprite = calloc(1, sizeof(Sprite));

	if (newSprite != NULL)
	{
		newSprite->alpha = 1.0f;
		return newSprite;
	}
	else
	{
		return NULL;
	}
}

void SpriteFree(SpritePtr* sprite)
{
	if(*sprite != NULL)
	{
		free(*sprite);
		*sprite = NULL;
	}
}

void SpriteRead(SpritePtr sprite, Stream stream)
{
	if(sprite != NULL && stream != NULL)
	{
		sprite->frameIndex = StreamReadInt(stream);
		sprite->alpha = StreamReadFloat(stream);
	}
}

void SpriteDraw(const Sprite* sprite, TransformPtr transform)
{
	if(sprite->spriteSource != NULL)
	{
		// Prepare to render a textured sprite
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set texture and texture offsets

		float x, y;
		SpriteSourceGetUV(sprite->spriteSource, sprite->frameIndex, &x, &y);
		AEGfxTextureSet(SpriteSourceGetTexture(sprite->spriteSource), x, y);
	}
	else
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	
	// Set position for the sprite
	Vector2D translationVals = *TransformGetTranslation(transform);
	Vector2D scaleVals = *TransformGetScale(transform);
	AEGfxSetFullTransform(translationVals.x, translationVals.y, AERadToDeg(TransformGetRotation(transform)), scaleVals.x, scaleVals.y);

	// Set transparency (range 0.0f – 1.0f)
	AEGfxSetTransparency(sprite->alpha);
	// Set blend color (RGBA, A = “strength” of blend)
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(sprite->mesh, AE_GFX_MDM_TRIANGLES);

}

float SpriteGetAlpha(SpritePtr sprite)
{
	if(sprite != NULL)
	{
		return sprite->alpha;
	}
	else
	{
		return 0.0f;
	}
}

void SpriteSetAlpha(SpritePtr sprite, float newAlpha)
{
	if(sprite != NULL)
	{
		sprite->alpha = AEClamp(newAlpha, 0.0f, 1.0f);
	}
}

void SpriteSetFrame(SpritePtr sprite, unsigned int frameIndex)
{
	if(sprite != NULL && frameIndex <= SpriteSourceGetFrameCount(sprite->spriteSource))
	{
		sprite->frameIndex = frameIndex;
		TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
	}
}

void SpriteSetMesh(SpritePtr sprite, AEGfxVertexList* mesh)
{
	if(sprite != NULL && mesh != NULL)
	{
		sprite->mesh = mesh;
	}
}

void SpriteSetSpriteSource(SpritePtr sprite, SpriteSourcePtr spriteSource)
{
	if(sprite != NULL && spriteSource != NULL)
	{
		sprite->spriteSource = spriteSource;
	}

}
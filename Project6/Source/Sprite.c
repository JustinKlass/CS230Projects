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
#include "Mesh.h"
#include "Matrix2D.h"
#include "MeshManager.h"
#include "SpriteSource.h"
#include "SpriteSourceManager.h"

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

	// Zero-terminated string used to display sprite text.
	const char* text;

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

SpritePtr SpriteClone(const SpritePtr other)
{
	if(other != NULL)
	{
		SpritePtr spriteClone = SpriteCreate();

		if(spriteClone != NULL)
		{
			*spriteClone = *other;
			return spriteClone;
		}
		else
		{
			return NULL;
		}
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

		const char* name = StreamReadToken(stream);
		SpriteSetMesh(sprite, MeshManagerBuild(name));

		const char* spriteSourceName = StreamReadToken(stream);
		SpriteSourcePtr spriteSource = SpriteSourceManagerBuild(spriteSourceName);
		SpriteSetSpriteSource(sprite, spriteSource);

	}
}

void SpriteDraw(const Sprite* sprite, TransformPtr transform)
{
	if(sprite != NULL && sprite->mesh != NULL)
	{
		if (sprite->spriteSource != NULL)
		{
			// Prepare to render a textured sprite
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			// Set texture and texture offsets
		}
		else
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}

		// Set transparency (range 0.0f – 1.0f)
		AEGfxSetTransparency(sprite->alpha);

		// Set blend color (RGBA, A = “strength” of blend)
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	
		if(sprite->text != NULL)
		{
			Matrix2D offset = { 0 };
			Matrix2D matrix = *TransformGetMatrix(transform);
			Vector2D scaleValues = *TransformGetScale(transform);

			Matrix2DTranslate(&offset, scaleValues.x, 0.0f);

			const char* spriteText = sprite->text;

			while (*spriteText != '\0')
			{
				unsigned int newFrameIndex = *spriteText - 32;
				MeshRender(sprite->mesh, sprite->spriteSource, &matrix, newFrameIndex);
				Matrix2DConcat(&matrix, &offset, &matrix);

				spriteText++;
			}
		}
		else
		{
			MeshRender(sprite->mesh, sprite->spriteSource, TransformGetMatrix(transform), sprite->frameIndex);
		}

	}
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

void SpriteSetText(SpritePtr sprite, const char* text)
{
	if(sprite != NULL)
	{
		sprite->text = text;
	}
}
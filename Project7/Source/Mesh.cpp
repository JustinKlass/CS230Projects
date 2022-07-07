//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230SU22A
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "AEEngine.h"
#include "Mesh.h"
#include "Matrix2D.h"
#include "Vector2D.h"
#include "Stream.h"
#include "SpriteSource.h"

AEGfxVertexList* MeshCreateQuad(float xHalfSize, float yHalfSize, float uSize, float vSize, const char* name)
{
	AEGfxVertexList* pMesh;				// Pointer to Mesh (Model)
// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles
	AEGfxTriAdd(
		-xHalfSize, -yHalfSize, 0xFFFFFFFF, 0.0f, vSize,
		xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
		-xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
		xHalfSize, yHalfSize, 0xFFFFFFFF, uSize, 0.0f,
		-xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);

	pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh, "Failed to create mesh %s!", &name);

	if(pMesh != NULL)
	{
		return pMesh;
	}
	else
	{
		return NULL;
	}
}

void MeshRender(AEGfxVertexList* mesh, SpriteSource* spriteSource, Matrix2D* transform, unsigned frameIndex)
{
	if(spriteSource != NULL)
	{
		float u, v;
		SpriteSourceGetUV(spriteSource, frameIndex, &u, &v);
		AEGfxTextureSet(SpriteSourceGetTexture(spriteSource), u, v);
	}
		AEGfxSetTransform(transform->m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

AEGfxVertexList* MeshRead(Stream stream)
{
	if(stream != NULL)
	{
		const char* token;
		token = StreamReadToken(stream);

		if(strncmp(token, "Mesh", 4) == 0)
		{
			AEGfxMeshStart();
			int numMatrices = StreamReadInt(stream);
			int i;
			for(i = 0; i < numMatrices; i++)
			{
				float x = StreamReadFloat(stream);
				float y = StreamReadFloat(stream);

				unsigned int color = StreamReadHex(stream);

				float u = StreamReadFloat(stream);
				float v = StreamReadFloat(stream);

				AEGfxVertexAdd(x, y, color, u, v);
			}
			return AEGfxMeshEnd();
		}
		else if(strncmp(token, "Quad", 4) == 0)
		{
			Vector2D halfSizeMesh;
			StreamReadVector2D(stream, &halfSizeMesh);

			int cols = StreamReadInt(stream);
			int rows = StreamReadInt(stream);

			const char* meshName;
			meshName = StreamReadToken(stream);

			AEGfxVertexList* meshVertex = MeshCreateQuad(halfSizeMesh.x, halfSizeMesh.y, (float)1 / cols, (float)1 / rows, meshName);
			return meshVertex;
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
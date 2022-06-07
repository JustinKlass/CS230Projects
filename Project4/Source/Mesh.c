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
//------------------------------------------------------------------------------
//
// File Name:	MeshManager.h
// Author(s):	Justin Klass (Justin.Klass)
// Project:		Project 4
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "Stream.h"
#include "AEEngine.h"
#include "MeshManager.h"

#define meshListSize 10

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.


typedef struct MeshManager
{
	// The number of game objects currently in the list.
	unsigned int meshCount;

	// The maximum number of game objects ever in the list, at any given moment.
	//unsigned int meshMax;

	// A list of all currently loaded meshes.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	AEGfxVertexList* meshList[meshListSize];

} MeshManager;


static MeshManager newMeshManager;

static void MeshManagerAdd(AEGfxVertexList* mesh);

void MeshManagerInit()
{
	newMeshManager.meshCount = 0;
	memset(newMeshManager.meshList, 0, meshListSize);
}

AEGfxVertexList* MeshManagerBuild(const char* meshName)
{
	char pathName[FILENAME_MAX] = "";
	sprintf_s(pathName, _countof(pathName), "Data/%s.txt", meshName);

	Stream stream = StreamOpen(pathName);
	if(stream != NULL)
	{
		AEGfxVertexList* newMesh = MeshRead(stream);
		MeshManagerAdd(newMesh);
		StreamClose(&stream);
		return newMesh;
	}
	else
	{
		return NULL;
	}
}

void MeshManagerFreeAll()
{
	int i;
	for (i = 0; i < meshListSize; i++)
	{
		AEGfxMeshFree(newMeshManager.meshList[i]);
		newMeshManager.meshList[i] = NULL;
	}
}

static void MeshManagerAdd(AEGfxVertexList* mesh)
{
	if(mesh != NULL)
	{
		if (newMeshManager.meshCount < meshListSize)
		{
			newMeshManager.meshList[newMeshManager.meshCount] = mesh;
			newMeshManager.meshCount += 1;
		}
	}
}
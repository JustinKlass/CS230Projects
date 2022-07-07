//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceManager.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSourceManager.h"
#include "SpriteSource.h"
#include "Stream.h"

#define spriteSourceListSize 10

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct SpriteSourceManager
{
	// The number of sprite sources currently in the list.
	unsigned int spriteSourceCount;

	// A list of all currently loaded sprite sources.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	SpriteSourcePtr	spriteSourceList[spriteSourceListSize];

} SpriteSourceManager;

static SpriteSourceManager ssManager;

static void SpriteSourceManagerAdd(SpriteSourcePtr spriteSource);
static SpriteSourcePtr SpriteSourceManagerFind(const char* name);

void SpriteSourceManagerInit()
{
	ssManager.spriteSourceCount = 0;
	memset(ssManager.spriteSourceList, 0, (sizeof(SpriteSourcePtr) * spriteSourceListSize));
}

SpriteSourcePtr SpriteSourceManagerBuild(const char* spriteSourceName)
{
	SpriteSourcePtr spriteSource = SpriteSourceManagerFind(spriteSourceName);
	if(spriteSource == NULL)
	{
		// Data/&s.txt
		char pathName[FILENAME_MAX] = "";
		sprintf_s(pathName, _countof(pathName), "Data/%s.txt", spriteSourceName);

		Stream stream = StreamOpen(pathName);

		if(stream != NULL)
		{
			spriteSource = SpriteSourceRead(stream);
			SpriteSourceManagerAdd(spriteSource);
		}
	}
	return spriteSource;
}

void SpriteSourceManagerFreeAll()
{
	int i;
	for (i = 0; i < spriteSourceListSize; i++)
	{
		SpriteSourceFree(&ssManager.spriteSourceList[i]);
		ssManager.spriteSourceList[i] = NULL;
	}
}

static void SpriteSourceManagerAdd(SpriteSourcePtr spriteSource)
{
	if (ssManager.spriteSourceCount < spriteSourceListSize && spriteSource != NULL)
	{
		unsigned int i;
		for (i = 0; i < spriteSourceListSize; i++)
		{
			if (ssManager.spriteSourceList[i] == NULL)
			{
				ssManager.spriteSourceList[i] = spriteSource;
				ssManager.spriteSourceCount += 1;

				return;
			}
		}
	}
}

static SpriteSourcePtr SpriteSourceManagerFind(const char* name)
{
	unsigned int i;
	for (i = 0; i < spriteSourceListSize; i++)
	{
		if (SpriteSourceIsNamed(ssManager.spriteSourceList[i], name))
		{
			return ssManager.spriteSourceList[i];
		}
	}

	return NULL;
}
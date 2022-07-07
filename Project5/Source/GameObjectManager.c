//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.h
// Author(s):	Justin Klass (Justin.Klass)
// Project:		Project 4
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Collider.h"

#define objectListSize 100

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct GameObjectManager
{
	// The current number of game objects currently in the list.
	unsigned int objectCount;

	// The maximum number of game objects ever in the list, at any given moment.
	unsigned int objectMax;

	// This list can be a fixed-length array (minimum size of 100 entries)
	// or a dynamically sized array, such as a linked list.
	GameObjectPtr	objectList[objectListSize];

} GameObjectManager;

static GameObjectManager gameObjectActiveList;
static GameObjectManager gameObjectArchetypes;

void GameObjectManagerInit(void)
{

	gameObjectActiveList.objectCount = 0;
	gameObjectActiveList.objectMax = objectListSize;
	memset(gameObjectActiveList.objectList, 0, objectListSize);

	gameObjectArchetypes.objectCount = 0;
	gameObjectArchetypes.objectMax = objectListSize;
	memset(gameObjectArchetypes.objectList, 0, objectListSize);

}

void GameObjectManagerUpdate(float dt)
{
	unsigned int i;
	for(i = 0; i < gameObjectActiveList.objectMax; i++)
	{
		if(gameObjectActiveList.objectList[i] != NULL)
		{
			GameObjectUpdate(gameObjectActiveList.objectList[i], dt);

			if (GameObjectIsDestroyed(gameObjectActiveList.objectList[i]))
			{
      			GameObjectFree(&gameObjectActiveList.objectList[i]);
				gameObjectActiveList.objectList[i] = NULL;
				gameObjectActiveList.objectCount -= 1;
			}
		}

	}
}

void GameObjectManagerCheckCollisions(void)
{
	unsigned int i;


	for(i = 0; i < gameObjectActiveList.objectMax; i++)
	{
		if(GameObjectGetCollider(gameObjectActiveList.objectList[i]) != NULL)
		{	
			unsigned int j;
			for(j = i; j < gameObjectActiveList.objectMax; j++)
			{
				if(GameObjectGetCollider(gameObjectActiveList.objectList[j]) != NULL)
				{
					ColliderCheck(GameObjectGetCollider(gameObjectActiveList.objectList[i]), GameObjectGetCollider(gameObjectActiveList.objectList[j]));
				}
			}
		}
	}
}

void GameObjectManagerDraw(void)
{
	unsigned int i;
	for(i = 0; i < gameObjectActiveList.objectMax; i++)
	{
		if(gameObjectActiveList.objectList[i] != NULL)
		{
			GameObjectDraw(gameObjectActiveList.objectList[i]);
		}
	}
}

void GameObjectManagerShutdown(void)
{
	unsigned int i;
	for (i = 0; i < gameObjectActiveList.objectMax; i++)
	{
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			GameObjectFree(&gameObjectActiveList.objectList[i]);
			gameObjectActiveList.objectCount -= 1;
		}
	}

	for (i = 0; i < gameObjectArchetypes.objectMax; i++)
	{
		if (gameObjectArchetypes.objectList[i] != NULL)
		{
			GameObjectFree(&gameObjectArchetypes.objectList[i]);
			gameObjectArchetypes.objectCount -= 1;
		}
	}

}

void GameObjectManagerAdd(GameObjectPtr gameObject)
{
	if(gameObjectActiveList.objectCount < gameObjectActiveList.objectMax && gameObject != NULL)
	{
		unsigned int i;
		for(i = 0; i < gameObjectActiveList.objectMax; i++)
		{
			if(gameObjectActiveList.objectList[i] == NULL)
			{
				gameObjectActiveList.objectList[i] = gameObject;
				gameObjectActiveList.objectCount += 1;

				return;
			}
		}
	}
}

void GameObjectManagerAddArchetype(GameObjectPtr gameObject)
{
	if(gameObjectArchetypes.objectCount < gameObjectArchetypes.objectMax && gameObject != NULL)
	{
		unsigned int i;
		for (i = 0; i < gameObjectArchetypes.objectMax; i++)
		{
			if(gameObjectArchetypes.objectList[i] == 0)
			{
				gameObjectArchetypes.objectList[i] = gameObject;
				gameObjectArchetypes.objectCount += 1;

				return;			
			}
		}
	}
}

GameObjectPtr GameObjectManagerGetObjectByName(const char* name)
{
	unsigned int i;
	for(i = 0; i < gameObjectActiveList.objectMax; i++)
	{
		if(GameObjectIsNamed(gameObjectActiveList.objectList[i], name))
		{
			return gameObjectActiveList.objectList[i];
		}
	}

	return NULL;
}

GameObjectPtr GameObjectManagerGetArchetype(const char* name)
{
	unsigned int i;
	for(i = 0; i < gameObjectArchetypes.objectMax; i++)
	{
		if(GameObjectIsNamed(gameObjectArchetypes.objectList[i], name))
		{
			return gameObjectArchetypes.objectList[i];
		}
	}

	return NULL;
}
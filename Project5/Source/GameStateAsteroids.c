//------------------------------------------------------------------------------
//
// File Name:	GameStateAsteroids.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 4
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateStub.h"
#include "Trace.h"
#include "MeshManager.h"
#include "AEEngine.h"
#include "SpriteSourceManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include "Behavior.h"
#include "BehaviorHudText.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const int cAsteroidSpawnInitial = 8;
static const int cAsteroidSpawnMaximum = 20;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
static int asteroidScore = 0;
static int asteroidHighScore = 0;
static int asteroidSpawnCount;
static int asteroidWaveCount;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
GameObjectPtr newSpaceshipObj;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void GameStateAsteroidsCreateHudElement(const char* objectName, const char* formatString, const int* watchValue);
static void GameStateAsteroidsSpawnAsteroidWave(void);
static void GameStateAsteroidsSpawnAsteroid(void);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateAsteroidsLoad()
{
	MeshManagerInit();
	SpriteSourceManagerInit();
}

// Initialize the memory associated with the Stub game state.
void GameStateAsteroidsInit()
{
	newSpaceshipObj = GameObjectFactoryBuild("Spaceship");
	if(newSpaceshipObj != NULL)
	{
		GameObjectManagerAdd(newSpaceshipObj);
	}

	GameStateAsteroidsCreateHudElement("AsteroidsHighScore", "High Score: %d", &asteroidHighScore);
	GameStateAsteroidsCreateHudElement("AsteroidsScore", "Score: %d", &asteroidScore);
	GameStateAsteroidsCreateHudElement("AsteroidsWave", "Wave: %d", &asteroidWaveCount);

	if(asteroidHighScore < asteroidScore)
	{
		asteroidHighScore = asteroidScore;
	}
	
	asteroidScore = 0;
	asteroidWaveCount = 0;
	asteroidSpawnCount = cAsteroidSpawnInitial;

	GameStateAsteroidsSpawnAsteroidWave();

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateAsteroidsUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);
	

	if (AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}

	if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}

	if (AEInputCheckTriggered('3'))
	{
		GameStateManagerSetNextState(GsRestart);
	}

	if (AEInputCheckTriggered('9'))
	{
		GameStateManagerSetNextState(GsSandbox);
	}

	if (AEInputCheckTriggered('0'))
	{
		GameStateManagerSetNextState(GsDemo);
	}

	GameObjectPtr gameObject = GameObjectManagerGetObjectByName("Asteroid");
	if(gameObject == NULL)
	{
		GameStateAsteroidsSpawnAsteroidWave();
	}
}

// Free any memory associated with the Stub game state.
void GameStateAsteroidsShutdown()
{
	// Free all objects.
	GameObjectManagerShutdown();
	MeshManagerFreeAll();
}

// Unload the resources associated with the Stub game state.
void GameStateAsteroidsUnload()
{
	// Free all sprite sources.
	SpriteSourceManagerFreeAll();	
	// Unload all textures.

	// Free all meshes.
}

void GameStateAsteroidsIncreaseScore(unsigned int scoreValue)
{
	asteroidScore += scoreValue;
}

static void GameStateAsteroidsCreateHudElement(const char* objectName, const char* formatString, const int* watchValue)
{
	GameObjectPtr gameObject = GameObjectFactoryBuild(objectName);
	BehaviorPtr behavior = GameObjectGetBehavior(gameObject);
	
	BehaviorHudTextSetDisplay(behavior, formatString, watchValue);
	GameObjectManagerAdd(gameObject);
	
}

static void GameStateAsteroidsSpawnAsteroidWave(void)
{
	asteroidWaveCount += 1;
	int i;
	for(i = 0; i < asteroidSpawnCount; i++)
	{
		GameStateAsteroidsSpawnAsteroid();
	}
	if(asteroidSpawnCount < cAsteroidSpawnMaximum)
	{
		asteroidSpawnCount += 1;
	}
}

static void GameStateAsteroidsSpawnAsteroid(void)
{
	GameObjectPtr gameObject = GameObjectFactoryBuild("Asteroid");
	GameObjectManagerAdd(gameObject);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


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
#include "GameStateAsteroids.h"
#include "Trace.h"
#include "MeshManager.h"
#include "AEEngine.h"
#include "SpriteSourceManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include "Behavior.h"
#include "BehaviorHudText.h"
#include "ScoreSystem.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
static const int cAsteroidSpawnInitial = 8;
static const int cAsteroidSpawnMaximum = 20;

static int asteroidSpawnCount;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
GameObjectPtr newSpaceshipObj;
GameObjectPtr newAsteroidScoreObj;
GameObjectPtr newAsteroidHighScoreObj;
GameObjectPtr newAsteroidWaveObj;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
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
	ScoreSystemInit();

	asteroidSpawnCount = cAsteroidSpawnInitial;
}

// Initialize the memory associated with the Stub game state.
void GameStateAsteroidsInit()
{
	newSpaceshipObj = GameObjectFactoryBuild("Spaceship");
	if(newSpaceshipObj != NULL)
	{
		GameObjectManagerAdd(newSpaceshipObj);
	}

	newAsteroidScoreObj = GameObjectFactoryBuild("AsteroidsScore");
	if(newAsteroidScoreObj != NULL)
	{
		GameObjectManagerAdd(newAsteroidScoreObj);
	}

	newAsteroidHighScoreObj = GameObjectFactoryBuild("AsteroidsHighScore");
	if(newAsteroidHighScoreObj != NULL)
	{
		GameObjectManagerAdd(newAsteroidHighScoreObj);
	}

	newAsteroidWaveObj = GameObjectFactoryBuild("AsteroidsWave");
	if(newAsteroidWaveObj != NULL)
	{
		GameObjectManagerAdd(newAsteroidWaveObj);
	}

	ScoreSystemRestart();
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

	if (AEInputCheckTriggered('4'))
	{
		GameStateManagerSetNextState(GsOmega);
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

static void GameStateAsteroidsSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWaveCount();
	for(int i = 0; i < asteroidSpawnCount; i++)
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


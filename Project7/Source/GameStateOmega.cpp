//------------------------------------------------------------------------------
//
// File Name:	GameStateOmega.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 6
// Course:		CS230S22
//
// Copyright ? 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateOmega.h"
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
static const int cAsteroidSpawnInitial = 8; //8
static const int cAsteroidSpawnMaximum = 20; //20

static int asteroidSpawnCount;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
GameObjectPtr newOmegaSpaceshipObj;
GameObjectPtr newArenaObj;
GameObjectPtr newOmegaScoreObj;
GameObjectPtr newOmegaHighScoreObj;
GameObjectPtr newOmegaWaveObj;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void GameStateOmegaSpawnAsteroidWave(void);
static void GameStateOmegaSpawnAsteroid(void);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateOmegaLoad()
{
	MeshManagerInit();
	SpriteSourceManagerInit();
	ScoreSystemInit();

	asteroidSpawnCount = cAsteroidSpawnInitial;
}

// Initialize the memory associated with the Stub game state.
void GameStateOmegaInit()
{
	newOmegaSpaceshipObj = GameObjectFactoryBuild("SpaceshipOmega");
	if (newOmegaSpaceshipObj != NULL)
	{
		GameObjectManagerAdd(newOmegaSpaceshipObj);
	}

	newArenaObj = GameObjectFactoryBuild("Arena");
	if (newArenaObj != NULL)
	{
		GameObjectManagerAdd(newArenaObj);
	}

	newOmegaScoreObj = GameObjectFactoryBuild("OmegaScore");
	if (newOmegaScoreObj != NULL)
	{
		GameObjectManagerAdd(newOmegaScoreObj);
	}

	newOmegaHighScoreObj = GameObjectFactoryBuild("OmegaHighScore");
	if (newOmegaHighScoreObj != NULL)
	{
		GameObjectManagerAdd(newOmegaHighScoreObj);
	}

	newOmegaWaveObj = GameObjectFactoryBuild("OmegaWave");
	if (newOmegaWaveObj != NULL)
	{
		GameObjectManagerAdd(newOmegaWaveObj);
	}




	ScoreSystemRestart();
	GameStateOmegaSpawnAsteroidWave();

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateOmegaUpdate(float dt)
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
		GameStateManagerSetNextState(GsAsteroids);
	}

	if (AEInputCheckTriggered('4'))
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
	if (gameObject == NULL)
	{
		GameStateOmegaSpawnAsteroidWave();
	}
}

// Free any memory associated with the Stub game state.
void GameStateOmegaShutdown()
{
	// Free all objects.
	GameObjectManagerShutdown();
	MeshManagerFreeAll();
}

// Unload the resources associated with the Stub game state.
void GameStateOmegaUnload()
{
	// Free all sprite sources.
	SpriteSourceManagerFreeAll();
	// Unload all textures.

	// Free all meshes.
}

static void GameStateOmegaSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWaveCount();
	for (int i = 0; i < asteroidSpawnCount; i++)
	{
		GameStateOmegaSpawnAsteroid();
	}
	if (asteroidSpawnCount < cAsteroidSpawnMaximum)
	{
		asteroidSpawnCount += 1;
	}
}

static void GameStateOmegaSpawnAsteroid(void)
{
	GameObjectPtr gameObject = GameObjectFactoryBuild("Asteroid");
	GameObjectManagerAdd(gameObject);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


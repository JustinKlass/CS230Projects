//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel2.h"
#include "Trace.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static int numLives = 0;
static int numHealth = 0;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateLevel2Load()
{
	Stream fp = StreamOpen("Data/Level2_Lives.txt");
	if (fp != NULL)
	{
		numLives = StreamReadInt(fp);
		StreamClose(&fp);
	}
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel2Init()
{
	Stream fp = StreamOpen("Data/Level2_Health.txt");
	if (fp != NULL)
	{
		numHealth = StreamReadInt(fp);
		StreamClose(&fp);
	}
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	numHealth -= 1;
	if(numHealth <= 0)
	{
		numLives -= 1;
		if(numLives <= 0)
		{
			GameStateManagerSetNextState(GsSandbox);
		}
	}
	else
	{
		GameStateManagerSetNextState(GsRestart);
	}
}

// Free any memory associated with the Stub game state.
void GameStateLevel2Shutdown()
{
	// Free all objects.
}

// Unload the resources associated with the Stub game state.
void GameStateLevel2Unload()
{
	// Free all sprite sources.

	// Unload all textures.

	// Free all meshes.
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 1
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel1.h"
#include "Trace.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
static int numLives1 = 0;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateLevel1Load()
{
	Stream fp = StreamOpen("Data/Level1_Lives.txt");
	if(fp != NULL)
	{
		numLives1 = StreamReadInt(fp);
		StreamClose(&fp);
	}
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel1Init()
{
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);
	numLives1 -= 1;
	if(numLives1 <= 0)
	{
		GameStateManagerSetNextState(GsLevel2);
	}

}

// Free any memory associated with the Stub game state.
void GameStateLevel1Shutdown()
{
	// Free all objects.
}

// Unload the resources associated with the Stub game state.
void GameStateLevel1Unload()
{
	// Free all sprite sources.

	// Unload all textures.

	// Free all meshes.
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
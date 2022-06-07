//------------------------------------------------------------------------------
//
// File Name:	GameStateSandbox.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateSandbox.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Trace.h"

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

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateSandboxLoad()
{
}

// Initialize the memory associated with the Stub game state.
void GameStateSandboxInit()
{
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateSandboxUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (AEInputCheckTriggered('0'))
	{
		GameStateManagerSetNextState(GsDemo);
	}

	if (AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}

	if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}

	if (AEInputCheckTriggered('9'))
	{
		GameStateManagerSetNextState(GsRestart);
		//GameStateDemoInit();
		//GameStateDemoLoad();
	}
	//GameStateManagerSetNextState(GsQuit);
}

// Free any memory associated with the Stub game state.
void GameStateSandboxShutdown()
{
	// Free all objects.
}

// Unload the resources associated with the Stub game state.
void GameStateSandboxUnload()
{
	// Free all sprite sources.

	// Unload all textures.

	// Free all meshes.
}
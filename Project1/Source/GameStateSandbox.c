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
#include "GameStateSandbox.h"
#include "Trace.h"
#include "Vector2D.h"
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
static void GameStateSandboxTraceFloat(const char* text, float f);
static void GameStateSandboxTraceVector(const char* text, const Vector2D* v);
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
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Stream fp = StreamOpen("Data/VectorTests.txt");
	if (fp != NULL)
	{
		Vector2D* vect = 0;

		Vector2DZero(vect);


	}


	GameStateManagerSetNextState(GsQuit);
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

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateSandboxTraceFloat(const char* text, float f)
{
	TraceMessage("Vector Test: %s = %f", text, f);
}

static void GameStateSandboxTraceVector(const char* text, const Vector2D* v)
{
	TraceMessage("Vector Test: %s = [%f, %f]", text, v->x, v->y);
}
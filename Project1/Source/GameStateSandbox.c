//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 1
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateSandbox.h"
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
		Vector2D vect;

		Vector2DZero(&vect);
		GameStateSandboxTraceVector("Vector2DZero", &vect);

		Vector2DSet(&vect, 1.0f, 1.5f);
		GameStateSandboxTraceVector("Vector2DSet", &vect);

		Vector2DNeg(&vect, &vect);
		GameStateSandboxTraceVector("Vector2DNeg", &vect);

		Vector2DAdd(&vect, &vect, &vect);
		GameStateSandboxTraceVector("Vector2DAdd", &vect);

		Vector2DSub(&vect, &vect, &vect);
		GameStateSandboxTraceVector("Vector2DSub", &vect);

		StreamReadVector2D(fp, &vect);
		GameStateSandboxTraceVector("StreamReadVector2D", &vect);

		Vector2DNormalize(&vect, &vect);
		GameStateSandboxTraceVector("Vector2DNormalize", &vect);

		float scale = StreamReadFloat(fp);
		GameStateSandboxTraceFloat("StreamReadFloat", scale);

		Vector2DScale(&vect, &vect, scale);
		GameStateSandboxTraceVector("Vector2DScale", &vect);

		Vector2DScaleAdd(&vect, &vect, &vect, scale);
		GameStateSandboxTraceVector("Vector2DScaleAdd", &vect);

		Vector2DScaleSub(&vect, &vect, &vect, scale);
		GameStateSandboxTraceVector("Vector2DScaleSub", &vect);

		GameStateSandboxTraceFloat("Vector2DLength", Vector2DLength(&vect));

		GameStateSandboxTraceFloat("Vector2DSquareLength", Vector2DSquareLength(&vect));

		Vector2D vect1;
		Vector2D vect2;

		StreamReadVector2D(fp, &vect1);
		GameStateSandboxTraceVector("StreamReadVector2D", &vect1);

		StreamReadVector2D(fp, &vect2);
		GameStateSandboxTraceVector("StreamReadVector2D", &vect2);

		GameStateSandboxTraceFloat("Vector2DDistance", Vector2DDistance(&vect1, &vect2));

		GameStateSandboxTraceFloat("Vector2DSquareDistance", Vector2DSquareDistance(&vect1, &vect2));

		GameStateSandboxTraceFloat("Vector2DDotProduct", Vector2DDotProduct(&vect1, &vect2));

		float ang = StreamReadFloat(fp);
		GameStateSandboxTraceFloat("StreamReadFloat", ang);

		Vector2DFromAngleDeg(&vect, ang);
		GameStateSandboxTraceVector("Vector2DFromAngleDeg", &vect);

		ang = StreamReadFloat(fp);
		GameStateSandboxTraceFloat("StreamReadFloat", ang);

		Vector2DFromAngleRad(&vect, ang);
		GameStateSandboxTraceVector("Vector2DFromAngleRad", &vect);

		GameStateSandboxTraceFloat("Vector2DToAngleRad", Vector2DToAngleRad(&vect));

		StreamClose(&fp);
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
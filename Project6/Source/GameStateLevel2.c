//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel2.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "AEEngine.h"
#include "Trace.h"
#include "Stream.h"
#include "Sprite.h"
#include "Vector2D.h"
#include "SpriteSource.h"
#include "Transform.h"
#include "Physics.h"

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
static const float spaceshipSpeed = 500.0f;
static AEGfxVertexList* pMeshSpaceship;
static GameObjectPtr newSpaceship;
static SpritePtr newSprite;
float cursorX, cursorY;
s32 pX, pY;
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

		StreamClose(&fp);
	}
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshSpaceship = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshSpaceship, "Failed to create spaceship mesh!");

}

// Initialize the memory associated with the Stub game state.
void GameStateLevel2Init()
{
	newSpaceship = GameObjectFactoryBuild("SpaceshipHoming");
	if(newSpaceship != NULL)
	{
		newSprite = GameObjectGetSprite(newSpaceship);
		SpriteSetMesh(newSprite, pMeshSpaceship);
	}
	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

void GameStateLevel2MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr newPhysics = GameObjectGetPhysics(gameObject);
	TransformPtr newTransform = GameObjectGetTransform(gameObject);

	if(newPhysics != NULL && newTransform != NULL)
	{	
		AEInputGetCursorPosition(&pX, &pY);
		AEGfxConvertScreenCoordinatesToWorld((float)pX, (float)pY, &cursorX, &cursorY);

		Vector2D cursorPos;
		cursorPos.x = cursorX;
		cursorPos.y = cursorY;

		Vector2D ship = *TransformGetTranslation(newTransform);
		Vector2DSub(&ship, &cursorPos, &ship);
		Vector2DNormalize(&ship, &ship);


		TransformSetRotation(newTransform, Vector2DToAngleRad(&ship));

		Vector2DScale(&ship, &ship, spaceshipSpeed);
		PhysicsSetVelocity(newPhysics, &ship);
	}
}
// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	if(AEInputCheckCurr('Z'))
	{
		SpriteSetAlpha(newSprite, 0.5f);
	}

	if(AEInputCheckCurr('X'))
	{
		SpriteSetAlpha(newSprite, 1.0f);
	}

	GameStateLevel2MovementController(newSpaceship);
	GameObjectUpdate(newSpaceship, dt);
	GameObjectDraw(newSpaceship);

	if(AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}

	if(AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsRestart);
	}

	if(AEInputCheckTriggered('3'))
	{
		GameStateManagerSetNextState(GsAsteroids);
	}

	if(AEInputCheckTriggered('4'))
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
}

// Free any memory associated with the Stub game state.
void GameStateLevel2Shutdown()
{
	// Free all objects.
	GameObjectFree(&newSpaceship);
}

// Unload the resources associated with the Stub game state.
void GameStateLevel2Unload()
{
	// Free all meshes.
	AEGfxMeshFree(pMeshSpaceship);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
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
#include "Sprite.h"
#include "SpriteSource.h"
#include "Vector2D.h"
#include "mesh.h"
#include "Transform.h"
#include "Physics.h"
#include "AEEngine.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Animation.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static AEGfxVertexList* planetVertexList;
static AEGfxVertexList* monkeyVertexList;
static AEGfxVertexList* monkeyFontVertexList;

static AEGfxTexture* newPlanetTexture;
static AEGfxTexture* monkeyIdleTexture;
static AEGfxTexture* monkeyWalkTexture;
static AEGfxTexture* monkeyJumpTexture;
static AEGfxTexture* monkeyFontTexture;

static SpriteSourcePtr newPlanetSpriteSource;
static SpriteSourcePtr monkeyIdleSpriteSource;
static SpriteSourcePtr monkeyWalkSpriteSource;
static SpriteSourcePtr monkeyJumpSpriteSource;
static SpriteSourcePtr monkeyFontSpriteSource;

static GameObjectPtr newPlanetObj;
static GameObjectPtr newMonkeyObj;
static GameObjectPtr newLivesObj;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
static int numLives = 0;
static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const float wallDistance = 350.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

typedef const enum
{
	MonkeyInvalid = -1,
	MonkeyIdle,
	MonkeyWalk,
	MonkeyJump

} MonkeyStates;

static enum MonkeyStates monkeyState = MonkeyInvalid;
static char livesBuffer[16] = "";


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
		numLives = StreamReadInt(fp);
		StreamClose(&fp);
	}

	planetVertexList = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
	newPlanetTexture = AEGfxTextureLoad("Assets/PlanetTexture.png");
	newPlanetSpriteSource = SpriteSourceCreate(1, 1, newPlanetTexture);

	monkeyVertexList = MeshCreateQuad(0.5, 0.5, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	monkeyFontVertexList = MeshCreateQuad(0.5, 0.5, 1.0f / 16, 1.0f / 8, "Mesh16x8");

	monkeyIdleTexture = AEGfxTextureLoad("Assets/MonkeyIdle.png");
	monkeyWalkTexture = AEGfxTextureLoad("Assets/MonkeyWalk.png");
	monkeyJumpTexture = AEGfxTextureLoad("Assets/MonkeyJump.png");
	monkeyFontTexture = AEGfxTextureLoad("Assets/Roboto_Mono_black.png");

	monkeyIdleSpriteSource = SpriteSourceCreate(1, 1, monkeyIdleTexture);
	monkeyWalkSpriteSource = SpriteSourceCreate(3, 3, monkeyWalkTexture);
	monkeyJumpSpriteSource = SpriteSourceCreate(1, 1, monkeyJumpTexture);
	monkeyFontSpriteSource = SpriteSourceCreate(16, 8, monkeyFontTexture);

}

static void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState)
{
	if (monkeyState != newState)
	{
		monkeyState = newState;
		AnimationPtr monkeyAnimation = GameObjectGetAnimation(gameObject);
		SpritePtr monkeySprite = GameObjectGetSprite(gameObject);

		switch (newState)
		{
		case MonkeyIdle:
			SpriteSetMesh(monkeySprite, planetVertexList);
			SpriteSetSpriteSource(monkeySprite, monkeyIdleSpriteSource);
			AnimationPlay(monkeyAnimation, 1, 0.0f, false);
			break;

		case MonkeyWalk:
			SpriteSetMesh(monkeySprite, monkeyVertexList);
			SpriteSetSpriteSource(monkeySprite, monkeyWalkSpriteSource);
			AnimationPlay(monkeyAnimation, 8, 0.05f, true);
			break;

		case MonkeyJump:
			SpriteSetMesh(monkeySprite, planetVertexList);
			SpriteSetSpriteSource(monkeySprite, monkeyJumpSpriteSource);
			AnimationPlay(monkeyAnimation, 1, 0.0f, false);
			break;

		}

	}
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel1Init()
{
	newPlanetObj = GameObjectFactoryBuild("PlanetBounce");
	newMonkeyObj = GameObjectFactoryBuild("Monkey");
	if(newPlanetObj != NULL)
	{
		SpritePtr newPlanetSprite = GameObjectGetSprite(newPlanetObj);

		SpriteSetMesh(newPlanetSprite, planetVertexList);
		SpriteSetSpriteSource(newPlanetSprite, newPlanetSpriteSource);
		SpriteSetFrame(newPlanetSprite, 0);

	}

	if(newMonkeyObj != NULL)
	{
		monkeyState = MonkeyInvalid;
		GameStateLevel1SetMonkeyState(newMonkeyObj, MonkeyIdle);
		newLivesObj = GameObjectFactoryBuild("MonkeyLivesText");
	}

	if (newLivesObj != NULL)
	{
		SpritePtr monkeyLivesSprite = GameObjectGetSprite(newLivesObj);
		SpriteSetMesh(monkeyLivesSprite, monkeyFontVertexList);
		SpriteSetSpriteSource(monkeyLivesSprite, monkeyFontSpriteSource);

		sprintf_s(livesBuffer, 16, "Lives: %d", numLives);
		SpriteSetText(monkeyLivesSprite, livesBuffer);

		AEGfxSetBackgroundColor(1, 1, 1);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	}
}

static void GameStateLevel1MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr newPhysics = GameObjectGetPhysics(gameObject);
	TransformPtr newTransform = GameObjectGetTransform(gameObject);

	if(newPhysics != NULL && newTransform != NULL)
	{
		Vector2D currVelocity = *PhysicsGetVelocity(newPhysics);
		//Vector2D currTransform = *PhysicsGetOldTranslation(newPhysics);

		if(&currVelocity != NULL)
		{
			if(AEInputCheckCurr(VK_LEFT))
			{
				currVelocity.x = -moveVelocity;
				// TO FLIP THE MONKEY
				if(monkeyState != MonkeyJump)
				{
					GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
				}
			}
			else if(AEInputCheckCurr(VK_RIGHT))
			{
				currVelocity.x = moveVelocity;
				// TO FLIP THE MONKEY
				if (monkeyState != MonkeyJump)
				{
					GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
				}
			}

			else
			{
				currVelocity.x = 0;
				if (monkeyState != MonkeyJump)
				{
					GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
				}
			}

			if(AEInputCheckTriggered(VK_UP))
			{
				currVelocity.y = jumpVelocity;
				PhysicsSetAcceleration(newPhysics, &gravityNormal);
				GameStateLevel1SetMonkeyState(gameObject, MonkeyJump);

			}

			Vector2D currTranslation = *TransformGetTranslation(newTransform);
			if(currTranslation.y < groundHeight)
			{
				currTranslation.y = groundHeight;
				TransformSetTranslation(newTransform, &currTranslation);
				currVelocity.y = 0;

				PhysicsSetAcceleration(newPhysics, &gravityNone);
				GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
			}
			PhysicsSetVelocity(newPhysics, &currVelocity);
		}
	}
}

static void GameStateLevel1BounceController(GameObjectPtr gameOject)
{
	const PhysicsPtr bouncePhysics = GameObjectGetPhysics(gameOject);
	const TransformPtr bounceTransform = GameObjectGetTransform(gameOject);

	if(bouncePhysics != NULL && bounceTransform != NULL)
	{
		Vector2D currVelocity = *PhysicsGetVelocity(bouncePhysics);
		Vector2D currPosition = *TransformGetTranslation(bounceTransform);

		if(currPosition.x <= -wallDistance)
		{
			currPosition.x = -wallDistance;
			currVelocity.x = -currVelocity.x;
		}
		if(currPosition.x >= wallDistance)
		{
			currPosition.x = wallDistance;
			currVelocity.x = -currVelocity.x;
		}
		if(currPosition.y <= groundHeight)
		{
			currPosition.y = groundHeight + (groundHeight - currPosition.y);
			currVelocity.y = -currVelocity.y;
		}

		TransformSetTranslation(bounceTransform, &currPosition);
		PhysicsSetVelocity(bouncePhysics, &currVelocity);
	}
}

static bool GameStateLevel1IsColliding(GameObjectPtr objectA, GameObjectPtr objectB)
{
	TransformPtr objATransform = GameObjectGetTransform(objectA);
	TransformPtr objBTransform = GameObjectGetTransform(objectB);

	const Vector2D* objACurrPosition = TransformGetTranslation(objATransform);
	const Vector2D* objBCurrPosition = TransformGetTranslation(objBTransform);

	float distance = Vector2DSquareDistance(objACurrPosition, objBCurrPosition);

	if(distance < CheckSquareDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{

	GameStateLevel1MovementController(newMonkeyObj);
	GameStateLevel1BounceController(newPlanetObj);

	GameObjectUpdate(newMonkeyObj, dt);
	GameObjectUpdate(newPlanetObj, dt);
	GameObjectUpdate(newLivesObj, dt);

	GameObjectDraw(newMonkeyObj);
	GameObjectDraw(newPlanetObj);
	GameObjectDraw(newLivesObj);

	if(GameStateLevel1IsColliding(newMonkeyObj, newPlanetObj))
	{
		numLives -= 1;
		if(numLives <= 0)
		{
			GameStateManagerSetNextState(GsLevel2);
		}
		else
		{
			GameStateManagerSetNextState(GsRestart);
		}
	}

	if(AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsRestart);
	}

	if(AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}

	if(AEInputCheckTriggered('3'))
	{
		GameStateManagerSetNextState(GsAsteroids);
	}

	if(AEInputCheckTriggered('9'))
	{
		GameStateManagerSetNextState(GsSandbox);
	}

	if(AEInputCheckTriggered('0'))
	{
		GameStateManagerSetNextState(GsDemo);
	}
}

// Free any memory associated with the Stub game state.
void GameStateLevel1Shutdown()
{
	// Free all objects.
	GameObjectFree(&newLivesObj);
	GameObjectFree(&newPlanetObj);
	GameObjectFree(&newMonkeyObj);
}

// Unload the resources associated with the Stub game state.
void GameStateLevel1Unload()
{
	// Free all sprite sources.
	SpriteSourceFree(&newPlanetSpriteSource);
	SpriteSourceFree(&monkeyIdleSpriteSource);
	SpriteSourceFree(&monkeyWalkSpriteSource);
	SpriteSourceFree(&monkeyJumpSpriteSource);
	SpriteSourceFree(&monkeyFontSpriteSource);

	// Unload all textures.
	//AEGfxTextureUnload(newPlanetTexture);
	//AEGfxTextureUnload(monkeyIdleTexture);
	//AEGfxTextureUnload(monkeyWalkTexture);
	//AEGfxTextureUnload(monkeyJumpTexture);
	//AEGfxTextureUnload(monkeyFontTexture);

	// Free all meshes.
	AEGfxMeshFree(planetVertexList);
	AEGfxMeshFree(monkeyVertexList);
	AEGfxMeshFree(monkeyFontVertexList);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
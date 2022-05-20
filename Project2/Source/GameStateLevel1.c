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

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static AEGfxVertexList* vertexList;
static AEGfxTexture* newTexture;
static SpriteSourcePtr newSpriteSource;
static GameObjectPtr newGameObj;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
static int numLives = 0;
static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

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

	vertexList = MeshCreateQuad(0.5f, 0.5f,1.0f, 1.0f, "Mesh1x1");
	newTexture = AEGfxTextureLoad("Assets/PlanetTexture.png");
	newSpriteSource = SpriteSourceCreate(1, 1, newTexture);
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel1Init()
{
	newGameObj = GameObjectFactoryBuild("./data/PlanetJump.txt");
	if(newGameObj != NULL)
	{
		SpritePtr newGameSprite = GameObjectGetSprite(newGameObj);

		SpriteSetMesh(newGameSprite, vertexList);
		SpriteSetSpriteSource(newGameSprite, newSpriteSource);
		SpriteSetFrame(newGameSprite, 0);

		AEGfxSetBackgroundColor(1, 1, 1);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	}
}


void GameStateLevel1MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr newPhysics = GameObjectGetPhysics(gameObject);
	TransformPtr newTransform = GameObjectGetTransform(gameObject);

	if(newPhysics != NULL && newTransform != NULL)
	{
		Vector2D currVelocity = *PhysicsGetVelocity(newPhysics);
		Vector2D currTransform = *PhysicsGetOldTranslation(newPhysics);

		if(&currVelocity != NULL && &currTransform != NULL)
		{
			if(AEInputCheckCurr(VK_LEFT))
			{
				currVelocity.x = -moveVelocity;
			}
			else if(AEInputCheckCurr(VK_RIGHT))
			{
				currVelocity.x = moveVelocity;
			}
			else if(!AEInputCheckCurr(VK_LEFT) && !AEInputCheckCurr(VK_RIGHT))
			{
				currVelocity.x = 0;
			}

			if(AEInputCheckTriggered(VK_UP))
			{
				currVelocity.y = jumpVelocity;
				PhysicsSetAcceleration(newPhysics, &gravityNormal);

			}

			Vector2D currTranslation = *TransformGetTranslation(newTransform);
			if(currTranslation.y < groundHeight)
			{
				currTranslation.y = groundHeight;
				TransformSetTranslation(newTransform, &currTranslation);
				currVelocity.y = 0;
				PhysicsSetAcceleration(newPhysics, &gravityNone);
				numLives -= 1;
				if(numLives <= 0)
				{
					GameStateManagerSetNextState(GsLevel2);
				}
			}
			PhysicsSetVelocity(newPhysics, &currVelocity);
		}
	}
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{
	GameStateLevel1MovementController(newGameObj);
	GameObjectUpdate(newGameObj, dt);
	GameObjectDraw(newGameObj);

	if(AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsRestart);
	}

	if(AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
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
	GameObjectFree(&newGameObj);
}

// Unload the resources associated with the Stub game state.
void GameStateLevel1Unload()
{
	// Free all sprite sources.
	SpriteSourceFree(&newSpriteSource);
	// Unload all textures.
	AEGfxTextureUnload(newTexture);
	// Free all meshes.
	AEGfxMeshFree(vertexList);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
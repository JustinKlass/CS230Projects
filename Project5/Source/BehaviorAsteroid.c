//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorAsteroid.h"
#include "Behavior.h"
#include "Random.h"
#include "GameObject.h"
#include "Collider.h"
#include "Teleporter.h"
#include "Physics.h"
#include "Transform.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "GameStateAsteroids.h"

typedef enum
{
	cAsteroidLarge,
	cAsteroidMedium,
	cAsteroidSmall,

} AsteroidSize;

typedef enum
{
	cAsteroidOriginTlc,
	cAsteroidOriginTrc,
	cAsteroidOriginBlc,
	cAsteroidOriginBrc,
	cAsteroidOriginCount,

} AsteroidOrigin;

typedef enum
{
	cAsteroidInvalid = -1,
	cAsteroidIdle = 0
} AsteroidStates;


typedef struct BehaviorAsteroid
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add asteroid-specific behavior variables.
	AsteroidSize	size;
	AsteroidOrigin	origin;

} BehaviorAsteroid, * BehaviorAsteroidPtr;

static const float asteroidSpeedMin = 50.0f;
static const float asteroidSpeedMax = 100.0f;

static void BehaviorAsteroidSetPosition(BehaviorAsteroidPtr asteroidPtr);
static void BehaviorAsteroidSetVelocity(BehaviorAsteroidPtr asteroidPtr);
static void BehaviorAsteroidCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other);

BehaviorPtr BehaviorAsteroidCreate(void)
{
	BehaviorAsteroidPtr newAsteroid = calloc(1, sizeof(BehaviorAsteroid));

	if (newAsteroid != NULL)
	{
		newAsteroid->base.onInit = &BehaviorAsteroidInit;
		newAsteroid->base.onUpdate = &BehaviorAsteroidUpdate;
		newAsteroid->base.onExit = &BehaviorAsteroidExit;
		newAsteroid->base.memorySize = sizeof(BehaviorAsteroid);
		newAsteroid->size = cAsteroidLarge;

		return (BehaviorPtr)newAsteroid;
	}
	else
	{
		return NULL;
	}
}

void BehaviorAsteroidInit(BehaviorPtr behavior)
{
	BehaviorAsteroidPtr asteroidPtr = (BehaviorAsteroidPtr)behavior;
	if(asteroidPtr->base.stateCurr == cAsteroidIdle)
	{
		asteroidPtr->origin = RandomRange(0, 3);
		BehaviorAsteroidSetPosition(asteroidPtr);
		BehaviorAsteroidSetVelocity(asteroidPtr);

		ColliderPtr collider = GameObjectGetCollider(asteroidPtr->base.parent);
		if(collider != NULL)
		{
			ColliderSetCollisionHandler(collider, &BehaviorAsteroidCollisionHandler);
		}
	}
}

void BehaviorAsteroidUpdate(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	TeleporterUpdateObject(behavior->parent);
}

void BehaviorAsteroidExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorAsteroidCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	if(gameObject != NULL && other != NULL)
	{
		const char* name = GameObjectGetName(other);
		if(strncmp(name, "Bullet", _countof("Bullet")) == 0 ||
		   strncmp(name, "Spaceship", _countof("Spaceship")) == 0)
		{
			GameStateAsteroidsIncreaseScore(20);
			GameObjectDestroy(gameObject);
		}
	}
}

static void BehaviorAsteroidSetPosition(BehaviorAsteroidPtr asteroidPtr)
{
	if(asteroidPtr->size == cAsteroidLarge)
	{
		TransformPtr transform = GameObjectGetTransform(asteroidPtr->base.parent);
		Vector2D newPosition;

		float maxX = AEGfxGetWinMaxX();
		float minX = AEGfxGetWinMinX();

		float maxY = AEGfxGetWinMaxY();
		float minY = AEGfxGetWinMinY();

		if(asteroidPtr->origin == cAsteroidOriginTlc)
		{
			Vector2DSet(&newPosition, minX, maxY);
			TransformSetTranslation(transform, &newPosition);
			
		}
		else if(asteroidPtr->origin == cAsteroidOriginBlc)
		{
			Vector2DSet(&newPosition, minX, minY);
			TransformSetTranslation(transform, &newPosition);
		}
		else if(asteroidPtr->origin == cAsteroidOriginTrc)
		{
			Vector2DSet(&newPosition, maxX, maxY);
			TransformSetTranslation(transform, &newPosition);
		}
		else if(asteroidPtr->origin == cAsteroidOriginBrc)
		{
			Vector2DSet(&newPosition, maxX, minY);
			TransformSetTranslation(transform, &newPosition);
		}
	}
}

static void BehaviorAsteroidSetVelocity(BehaviorAsteroidPtr asteroidPtr)
{
	float randAngle = 0.0f;
	float velocity = RandomRangeFloat(asteroidSpeedMin, asteroidSpeedMax);

	if(asteroidPtr->size == cAsteroidLarge)
	{
		if (asteroidPtr->origin == cAsteroidOriginTlc)
		{
			randAngle = RandomRangeFloat(-80, -10);
		}
		else if (asteroidPtr->origin == cAsteroidOriginBlc)
		{
			randAngle = RandomRangeFloat(10, 80);
		}
		else if (asteroidPtr->origin == cAsteroidOriginTrc)
		{
			randAngle = RandomRangeFloat(-170, -100);
		}
		else if (asteroidPtr->origin == cAsteroidOriginBrc)
		{
			randAngle = RandomRangeFloat(100, 170);
		}
	}

	if(asteroidPtr->size == cAsteroidMedium || asteroidPtr->size == cAsteroidSmall)
	{
		randAngle = RandomRangeFloat(0, 359);
	}

	PhysicsPtr physics = GameObjectGetPhysics(asteroidPtr->base.parent);
	Vector2D newVelocity;

	Vector2DFromAngleDeg(&newVelocity, randAngle);
	Vector2DScale(&newVelocity, &newVelocity, velocity);

	PhysicsSetVelocity(physics, &newVelocity);
}
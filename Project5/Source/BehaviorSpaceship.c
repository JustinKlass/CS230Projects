//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.h
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 4
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Vector2D.h"
#include "Behavior.h"
#include "AEEngine.h"
#include "Physics.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include "BehaviorSpaceship.h"
#include "Teleporter.h"
#include "Collider.h"
#include "Sprite.h"
#include "GameStateManager.h"
#define _USE_MATH_DEFINES
#include <Math.h>

typedef enum 
{
	cSpaceshipInvalid = -1,

	cSpaceshipIdle,

	cSpaceshipThrust,

	cSpaceshipDead

} behaviorStates;


static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.25f;
static const float spaceshipWeaponBulletSpeed = 750.0f;
static const float spaceshipDeathDuration = 3.0f;

static void BehaviorSpaceshipCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other);

static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt);

static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt);

static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt);

static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior);

BehaviorPtr BehaviorSpaceshipCreate(void)
{
	BehaviorPtr newBehaviorObject = calloc(1, sizeof(Behavior));

	if (newBehaviorObject != NULL)
	{
		newBehaviorObject->stateCurr = cSpaceshipInvalid;
		newBehaviorObject->stateNext = cSpaceshipInvalid;
		newBehaviorObject->memorySize = sizeof(Behavior);
		
		newBehaviorObject->onInit = &BehaviorSpaceshipInit;
		newBehaviorObject->onUpdate = &BehaviorSpaceshipUpdate;
		newBehaviorObject->onExit = &BehaviorSpaceshipExit;

		return newBehaviorObject;
	}
	else
	{
		return NULL;
	}
}

void BehaviorSpaceshipInit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
	if(behavior->stateCurr == cSpaceshipIdle)
	{
		ColliderPtr GOCollider = GameObjectGetCollider(behavior->parent);
		if(GOCollider != NULL)
		{
			ColliderSetCollisionHandler(GOCollider, &BehaviorSpaceshipCollisionHandler);
		}
	}
	if(behavior->stateCurr == cSpaceshipDead)
	{
		behavior->timer = spaceshipDeathDuration;

	}
}

void BehaviorSpaceshipUpdate(BehaviorPtr behavior, float dt)
{
	if(behavior != NULL)
	{
		switch (behavior->stateCurr)
		{
		case cSpaceshipIdle:
			BehaviorSpaceshipUpdateRotation(behavior, dt);
			BehaviorSpaceshipUpdateWeapon(behavior, dt);
			if (AEInputCheckCurr(VK_UP))
			{
				behavior->stateNext = cSpaceshipThrust;
			}
			break;

		case cSpaceshipThrust:
			BehaviorSpaceshipUpdateRotation(behavior, dt);
			BehaviorSpaceshipUpdateVelocity(behavior, dt);
			BehaviorSpaceshipUpdateWeapon(behavior, dt);
			if (!AEInputCheckTriggered(VK_UP))
			{
				behavior->stateNext = cSpaceshipIdle;
			}
			break;
		}

		TeleporterUpdateObject(behavior->parent);
		
		if(behavior->stateCurr == cSpaceshipDead)
		{
			TransformPtr transform = GameObjectGetTransform(behavior->parent);
			SpritePtr sprite = GameObjectGetSprite(behavior->parent);

			float rotation = TransformGetRotation(transform);
			Vector2D scale = *TransformGetScale(transform);

			Vector2DSet(&scale, (scale.x - (10.0f * dt)), (scale.y - (10.0f * dt)));

			TransformSetScale(transform, &scale);
			float alpha = SpriteGetAlpha(sprite);

			SpriteSetAlpha(sprite, (alpha - (dt * 1.0f)));
			TransformSetRotation(transform, (rotation + (dt * 15.0f)));


			behavior->timer -= dt;
			if(behavior->timer < 0)
			{
				GameStateManagerSetNextState(GsRestart);
			}
		}
	}

}

void BehaviorSpaceshipExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	GameObjectPtr behaviorGO = behavior->parent;
	PhysicsPtr behaviorPhysics = GameObjectGetPhysics(behaviorGO);

	if(AEInputCheckCurr(VK_LEFT))
	{
		PhysicsSetRotationalVelocity(behaviorPhysics, spaceshipTurnRateMax);
	}

	else if(AEInputCheckCurr(VK_RIGHT))
	{
		PhysicsSetRotationalVelocity(behaviorPhysics, -spaceshipTurnRateMax);
	}

	else
	{
		PhysicsSetRotationalVelocity(behaviorPhysics, 0.0f);
	}
}

static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt)
{
	GameObjectPtr behaviorGO = behavior->parent;

	PhysicsPtr behaviorPhysics = GameObjectGetPhysics(behaviorGO);
	TransformPtr behaviorTransform = GameObjectGetTransform(behaviorGO);

	if(behaviorPhysics != NULL && behaviorTransform != NULL)
	{
		float tranformRotation = TransformGetRotation(behaviorTransform);
		Vector2D vec;
		Vector2DFromAngleRad(&vec, tranformRotation);
		Vector2DNormalize(&vec, &vec);

		Vector2D currVelocity = *PhysicsGetVelocity(behaviorPhysics);
		Vector2DScaleAdd(&currVelocity, &vec, &currVelocity, (spaceshipAcceleration * dt));

		float newSpeed = Vector2DLength(&currVelocity);

		if(newSpeed > spaceshipSpeedMax)
		{
			Vector2DScale(&currVelocity, &currVelocity, (spaceshipSpeedMax / newSpeed));
		}
		PhysicsSetVelocity(behaviorPhysics, &currVelocity);
	}

}

static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt)
{
	if(behavior->timer > 0)
	{
		behavior->timer -= dt;
		if(behavior->timer < 0)
		{
			behavior->timer = 0;
		}
	}

	if(AEInputCheckCurr(' '))
	{
		if(behavior->timer <= 0)
		{
			BehaviorSpaceshipSpawnBullet(behavior);
			behavior->timer = spaceshipWeaponCooldownTime;
		}
	}
}

static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior)
{
	GameObjectPtr bulletGO = GameObjectFactoryBuild("Bullet");

	if(bulletGO != NULL)
	{
		GameObjectPtr behaviorGO = behavior->parent;
		TransformPtr behaviorTransform = GameObjectGetTransform(behaviorGO);
		PhysicsPtr behaviorPhysics = GameObjectGetPhysics(bulletGO);

		TransformPtr bulletTransform = GameObjectGetTransform(bulletGO);
		TransformSetRotation(bulletTransform, TransformGetRotation(behaviorTransform));
		TransformSetTranslation(bulletTransform, TransformGetTranslation(behaviorTransform));

		float tranformRotation = TransformGetRotation(behaviorTransform);

		Vector2D vec;
		Vector2DFromAngleRad(&vec, tranformRotation);
		Vector2DNormalize(&vec, &vec);

		Vector2DScale(&vec, &vec, spaceshipWeaponBulletSpeed);
		PhysicsSetVelocity(behaviorPhysics, &vec);
		GameObjectManagerAdd(bulletGO);
		
	}
}

static void BehaviorSpaceshipCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	if(gameObject != NULL && other != NULL)
	{
		const char* name = GameObjectGetName(other);
		if(strncmp(name, "Asteroid", _countof("Asteroid")) == 0)
		{
			BehaviorPtr behavior = GameObjectGetBehavior(gameObject);
			behavior->stateNext = cSpaceshipDead;
		}
	}
}
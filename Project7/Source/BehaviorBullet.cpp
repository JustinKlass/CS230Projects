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
#include "Behavior.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "BehaviorBullet.h"
#include "Collider.h"
#include "Teleporter.h"

enum class behaviorBulletStates
{
	cBulletInvalid = -1,

	cBulletIdle

};

static void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt);
static void BehaviorBulletCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other);

BehaviorPtr BehaviorBulletCreate(void)
{
	BehaviorPtr newBullet = (BehaviorPtr)calloc(1, sizeof(Behavior));

	if (newBullet != NULL)
	{
		newBullet->stateCurr = (int)behaviorBulletStates::cBulletInvalid;
		newBullet->stateNext = (int)behaviorBulletStates::cBulletInvalid;
		newBullet->memorySize = sizeof(Behavior);

		newBullet->onInit = &BehaviorBulletInit;
		newBullet->onUpdate = &BehaviorBulletUpdate;
		newBullet->onExit = &BehaviorBulletExit;

		return newBullet;
	}
	else
	{
		return NULL;
	}
}

void BehaviorBulletInit(BehaviorPtr behavior)
{
	if(behavior->stateCurr == (int)behaviorBulletStates::cBulletIdle)
	{
		ColliderPtr GOCollider = GameObjectGetCollider(behavior->parent);
		if(GOCollider != NULL)
		{
			ColliderSetCollisionHandler(GOCollider, &BehaviorBulletCollisionHandler);
		}
	}
}

void BehaviorBulletUpdate(BehaviorPtr behavior, float dt)
{
	switch(behavior->stateCurr)
	{
		case (int)behaviorBulletStates::cBulletIdle:
			BehaviorBulletUpdateLifeTimer(behavior, dt);
			break;
	}
	TeleporterUpdateObject(behavior->parent);
}

void BehaviorBulletExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt)
{
	if (behavior->timer > 0)
	{
		behavior->timer -= dt;
		if (behavior->timer <= 0)
		{
			GameObjectDestroy(behavior->parent);
		}
	}
}

static void BehaviorBulletCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	if(gameObject != NULL && other != NULL)
	{
		const char* name = GameObjectGetName(other);
		if(strncmp(name, "Asteroid", _countof("Asteroid")) == 0)
		{
			GameObjectDestroy(gameObject);
		}
	}
}
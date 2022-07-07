//------------------------------------------------------------------------------
//
// File Name:	GameObject.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "GameObject.h"
#include "Stream.h"
#include "animation.h"
#include "Behavior.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "BehaviorBullet.h"
#include "BehaviorAsteroid.h"
#include "BehaviorSpaceship.h"
#include "BehaviorHudText.h"
#include "Collider.h"
#include "Behavior.h"

typedef struct GameObject
{
	// The name of the game object.
	// A buffer is used to allow each game object to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
	//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
	//	 calling this function!  Instead, use the _countof() macro to get the
	//	 size of the "name" array.]
	char name[32];

	// Flag to indicate that the game object is dead and should be destroyed.
	bool isDestroyed;

	// Pointer to an attached animation component.
	AnimationPtr animation;

	// Pointer to an attached behavior component.
	BehaviorPtr behavior;

	// Pointer to an attached collider component.
	ColliderPtr collider;

	// Pointer to an attached physics component.
	PhysicsPtr physics;

	// Pointer to an attached sprite component.
	SpritePtr sprite;

	// Pointer to an attached transform component.
	TransformPtr transform;

} GameObject;

GameObjectPtr GameObjectCreate(void)
{
	GameObjectPtr newGameObject = calloc(1, sizeof(GameObject));

	if (newGameObject != NULL)
	{
		return newGameObject;
	}
	else
	{
		return NULL;
	}
}

GameObjectPtr GameObjectClone(const GameObjectPtr other)
{
	if (other != NULL)
	{
		GameObjectPtr gameObjectClone = GameObjectCreate();

		if (gameObjectClone != NULL)
		{
			//*gameObjectClone = *other;
			// DO NAME AND GAMEOBJECT ADD BLANK

			GameObjectSetName(gameObjectClone, GameObjectGetName(other));
			GameObjectAddBehavior(gameObjectClone, BehaviorClone(GameObjectGetBehavior(other)));
			GameObjectAddAnimation(gameObjectClone, AnimationClone(GameObjectGetAnimation(other)));
			GameObjectAddPhysics(gameObjectClone, PhysicsClone(GameObjectGetPhysics(other)));
			GameObjectAddSprite(gameObjectClone, SpriteClone(GameObjectGetSprite(other)));
			GameObjectAddTransform(gameObjectClone, TransformClone(GameObjectGetTransform(other)));
			GameObjectAddCollider(gameObjectClone, ColliderClone(GameObjectGetCollider(other)));

			gameObjectClone->isDestroyed = other->isDestroyed;

			return gameObjectClone;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

void GameObjectFree(GameObjectPtr* gameObject)
{
	
	if (*gameObject != NULL)
	{	

		SpriteFree(&(*gameObject)->sprite);
		AnimationFree(&(*gameObject)->animation);
		PhysicsFree(&(*gameObject)->physics);
		TransformFree(&(*gameObject)->transform);
		BehaviorFree(&(*gameObject)->behavior);
		ColliderFree(&(*gameObject)->collider);
		
		free(*gameObject);
		*gameObject = NULL;
	}
}

void GameObjectRead(GameObjectPtr gameObject, Stream stream)
{
	if (gameObject != NULL && stream != NULL)
	{
		GameObjectSetName(gameObject, StreamReadToken(stream));
	}
	while(true)
	{
		const char* streamVar = StreamReadToken(stream);
		if(strncmp(streamVar, "Transform", _countof("Transform")) == 0)
		{
			TransformPtr transformObj = TransformCreate();
			TransformRead(transformObj, stream);
			GameObjectAddTransform(gameObject, transformObj);
		}
		else if(strncmp(streamVar, "Physics", _countof("Physics")) == 0)
		{
			PhysicsPtr physicsObj = PhysicsCreate();
			PhysicsRead(physicsObj, stream);
			GameObjectAddPhysics(gameObject, physicsObj);
		}
		else if(strncmp(streamVar, "Sprite", _countof("Sprite")) == 0)
		{
			SpritePtr spriteObj = SpriteCreate();
			SpriteRead(spriteObj, stream);
			GameObjectAddSprite(gameObject, spriteObj);
		}
		else if(strncmp(streamVar, "Animation", _countof("Animation")) == 0)
		{
			AnimationPtr animationObj = AnimationCreate();
			AnimationRead(animationObj, stream);
			GameObjectAddAnimation(gameObject, animationObj);
		}
		else if(strncmp(streamVar, "BehaviorSpaceship", _countof("BehaviorSpaceship")) == 0)
		{
			BehaviorPtr spaceshipObj = BehaviorSpaceshipCreate();
			BehaviorRead(spaceshipObj, stream);
			GameObjectAddBehavior(gameObject, spaceshipObj);
		}
		else if(strncmp(streamVar, "BehaviorBullet", _countof("BehaviorBullet")) == 0)
		{
			BehaviorPtr bulletObj = BehaviorBulletCreate();
			BehaviorRead(bulletObj, stream);
			GameObjectAddBehavior(gameObject, bulletObj);
		}		
		else if(strncmp(streamVar, "BehaviorAsteroid", _countof("BehaviorAsteroid")) == 0)
		{
			BehaviorPtr bulletObj = BehaviorAsteroidCreate();
			BehaviorRead(bulletObj, stream);
			GameObjectAddBehavior(gameObject, bulletObj);
		}
		else if(strncmp(streamVar, "BehaviorHudText", _countof("BehaviorHudText")) == 0)
		{
			BehaviorPtr behaviorHudText = BehaviorHudTextCreate();
			BehaviorRead(behaviorHudText, stream);
			GameObjectAddBehavior(gameObject, behaviorHudText);
		}
		else if(strncmp(streamVar, "Collider", _countof("Collider")) == 0)
		{
			ColliderPtr colliderObj = ColliderCreate();
			ColliderRead(colliderObj, stream);
			GameObjectAddCollider(gameObject, colliderObj);
		}
		else if(strncmp(streamVar, "", _countof("")) == 0)
		{
			break;
		}
	}
}

void GameObjectDestroy(GameObjectPtr gameObject)
{
	if(gameObject != NULL)
	{
		gameObject->isDestroyed = true;
	}

}

bool GameObjectIsDestroyed(GameObjectPtr gameObject)
{
	if(gameObject != NULL)
	{
		return gameObject->isDestroyed;
	}
	else
	{
		return false;
	}
}

void GameObjectAddAnimation(GameObjectPtr gameObject, AnimationPtr animation)
{
	if(gameObject != NULL && animation != NULL)
	{
		gameObject->animation = animation;
		AnimationSetParent(animation, gameObject);
	}
}

void GameObjectAddBehavior(GameObjectPtr gameObject, BehaviorPtr behavior)
{
	if(gameObject != NULL && behavior != NULL)
	{
		gameObject->behavior = behavior;
		BehaviorSetParent(behavior, gameObject);
	}
}

void GameObjectAddCollider(GameObjectPtr gameObject, ColliderPtr collider)
{
	if(gameObject != NULL && collider != NULL)
	{
		gameObject->collider = collider;
		ColliderSetParent(collider, gameObject);
	}
}

void GameObjectAddPhysics(GameObjectPtr gameObject, PhysicsPtr physics)
{
	if(gameObject != NULL && physics != NULL)
	{
		gameObject->physics = physics;
	}
}

void GameObjectAddSprite(GameObjectPtr gameObject, SpritePtr sprite)
{
	if(gameObject != NULL && sprite != NULL)
	{
		gameObject->sprite = sprite;
	}
}

void GameObjectAddTransform(GameObjectPtr gameObject, TransformPtr transform)
{
	if(gameObject != NULL && transform != NULL)
	{
		gameObject->transform = transform;
	}
}

void GameObjectSetName(GameObjectPtr gameObject, const char* name)
{
	if(gameObject != NULL && name != NULL)
	{
		strcpy_s(gameObject->name, _countof(gameObject->name), name);
	}
}

const char* GameObjectGetName(const GameObject* gameObject)
{
	if(gameObject != NULL && gameObject->name != NULL)
	{
		return gameObject->name;
	}
	else
	{
		return NULL;
	}
}

bool GameObjectIsNamed(const GameObject* gameObject, const char* name)
{
	if(gameObject != NULL)
	{
		if(strncmp(name, gameObject->name, _countof(gameObject->name)) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

AnimationPtr GameObjectGetAnimation(const GameObjectPtr gameObject)
{
	if(gameObject != NULL && gameObject->animation != NULL)
	{
		return gameObject->animation;
	}
	else
	{
		return NULL;
	}
}

BehaviorPtr GameObjectGetBehavior(const GameObjectPtr gameObject)
{
	if(gameObject != NULL)
	{
		return gameObject->behavior;
	}
	else
	{
		return NULL;
	}
}

ColliderPtr GameObjectGetCollider(const GameObjectPtr gameObject)
{
	if(gameObject != NULL)
	{
		return gameObject->collider;
	}
	else
	{
		return NULL;
	}
}

PhysicsPtr GameObjectGetPhysics(const GameObjectPtr gameObject)
{
	if (gameObject != NULL && gameObject->physics != NULL)
	{
		return gameObject->physics;
	}
	else
	{
		return NULL;
	}
}

SpritePtr GameObjectGetSprite(const GameObjectPtr gameObject)
{
	if (gameObject != NULL && gameObject->sprite != NULL)
	{
		return gameObject->sprite;
	}
	else
	{
		return NULL;
	}
}

TransformPtr GameObjectGetTransform(const GameObjectPtr gameObject)
{
	if (gameObject != NULL && gameObject->transform != NULL)
	{
		return gameObject->transform;
	}
	else
	{
		return NULL;
	}
}

void GameObjectUpdate(GameObjectPtr gameObject, float dt)
{

	if(gameObject->physics != NULL && gameObject->transform != NULL)
	{
		PhysicsUpdate(gameObject->physics, gameObject->transform, dt);
	}
	if(gameObject->animation != NULL)
	{
		AnimationUpdate(gameObject->animation, dt);
	}
	if (gameObject->behavior != NULL)
	{
		BehaviorUpdate(gameObject->behavior, dt);
	}
}

void GameObjectDraw(GameObjectPtr gameObject)
{	
	if(gameObject != NULL)
	{
		SpriteDraw(gameObject->sprite, gameObject->transform);
	}
}
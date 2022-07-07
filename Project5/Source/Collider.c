//------------------------------------------------------------------------------
//
// File Name:	Collider.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"
#include "GameObject.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"
#define _USE_MATH_DEFINES
#include <math.h>


typedef struct Collider
{
	// Pointer to the collider's parent game object.
	GameObjectPtr parent;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler handler;

} Collider;


ColliderPtr ColliderCreate(void)
{
	ColliderPtr newCollider = calloc(1, sizeof(Collider));

	if (newCollider != NULL)
	{
		return newCollider;
	}
	else
	{
		return NULL;
	}
}

ColliderPtr ColliderClone(const ColliderPtr other)
{
	ColliderPtr colliderClone = ColliderCreate();

	if (other != NULL && colliderClone != NULL)
	{
		*colliderClone = *other;
		return colliderClone;
	}
	else
	{
		return NULL;
	}
}

void ColliderFree(ColliderPtr* collider)
{
	if (*collider != NULL)
	{
		free(*collider);
		*collider = NULL;
	}
}

void ColliderRead(ColliderPtr collider, Stream stream)
{
	if(collider != NULL && stream != NULL)
	{
		
	}
}

void ColliderSetParent(ColliderPtr collider, GameObjectPtr parent)
{
	if(collider != NULL && parent != NULL)
	{
		collider->parent = parent;
	}
}

void ColliderCheck(ColliderPtr collider, ColliderPtr other)
{
	if(collider != NULL && other != NULL)
	{
		TransformPtr col1 = GameObjectGetTransform(collider->parent);
		TransformPtr col2 = GameObjectGetTransform(other->parent);

		Vector2D col1Translation = *TransformGetTranslation(col1);
		Vector2D col2Translation = *TransformGetTranslation(col2);

		Vector2D col1Scale = *TransformGetScale(col1);
		Vector2D col2Scale = *TransformGetScale(col2);

		float col1Radius = col1Scale.x / 2.0f;
		float col2Radius = col2Scale.x / 2.0f;

		float distance = Vector2DSquareDistance(&col1Translation, &col2Translation);

		if (distance <= ((col1Radius + col2Radius) * (col1Radius + col2Radius)))
		{
			if(collider->handler != NULL)
			{
				collider->handler(collider->parent, other->parent);
			}

			if(other->handler != NULL)
			{
				other->handler(other->parent, collider->parent);
			}
		}
	
	}
}

void ColliderSetCollisionHandler(ColliderPtr collider, CollisionEventHandler handler)
{
	if(collider != NULL)
	{
		collider->handler = handler;
	}
}
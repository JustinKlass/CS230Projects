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
#include "ColliderCircle.h"
#include "ColliderLine.h"

bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2);


ColliderPtr ColliderClone(const ColliderPtr other)
{
	if (other != NULL)
	{
		ColliderPtr colliderClone = (ColliderPtr)calloc(1, other->memorySize);
	
		if (colliderClone != NULL)
		{
			memcpy_s(colliderClone, other->memorySize, other, other->memorySize);
			ColliderSetParent(colliderClone, NULL);

			return colliderClone;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
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
	if(collider != NULL)
	{
		collider->parent = parent;
	}
}

void ColliderCheck(ColliderPtr collider, ColliderPtr other)
{
	if(collider != NULL && other != NULL)
	{
		if(ColliderIsColliding(collider, other))
		{
			if (collider->handler != NULL)
			{
				collider->handler(collider->parent, other->parent);
			}

			if (other->handler != NULL)
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

bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2)
{
	if(collider1->type == ColliderType::ColliderTypeCircle && collider2->type == ColliderType::ColliderTypeCircle)
	{
		return ColliderCircleIsCollidingWithCircle(collider1, collider2);
	}
	else if(collider1->type == ColliderType::ColliderTypeLine && collider2->type == ColliderType::ColliderTypeLine)
	{
		return false;
	}
	else if(collider1->type == ColliderType::ColliderTypeLine && collider2->type == ColliderType::ColliderTypeCircle)
	{
		return ColliderLineIsCollidingWithCircle(collider1, collider2);
	}
	else if(collider2->type == ColliderType::ColliderTypeLine && collider1->type == ColliderType::ColliderTypeCircle)
	{
		return ColliderLineIsCollidingWithCircle(collider2, collider1);
	}

	return false;
}
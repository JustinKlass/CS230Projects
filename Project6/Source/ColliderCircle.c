//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 6
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderCircle.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Stream.h"

typedef struct ColliderCircle
{
	// Inherit the base collider structure.
	Collider	base;

	// Radius of the circle collider.
	float radius;

} ColliderCircle, * ColliderCirclePtr;

ColliderPtr ColliderCircleCreate(void)
{
	ColliderCirclePtr newCollider = calloc(1, sizeof(ColliderCircle));

	if (newCollider != NULL)
	{
		newCollider->base.memorySize = sizeof(ColliderCircle);
		newCollider->base.type = ColliderTypeCircle;
		newCollider->base.parent = NULL;
		newCollider->base.handler = NULL;
		newCollider->radius = 0.5f;
		
		return &(newCollider->base);
	}
	else
	{
		return NULL;
	}
}


void ColliderCircleRead(ColliderPtr collider, Stream stream)
{
	if (collider != NULL && stream != NULL)
	{
		float radius;
		radius = StreamReadFloat(stream);

		((ColliderCirclePtr)collider)->radius = radius;
	}
}

float ColliderCircleGetRadius(ColliderPtr collider)
{
	if(collider != NULL)
	{
		return ((ColliderCirclePtr)collider)->radius;
	}
	else
	{
		return 0.0f;
	}
}


void ColliderCircleSetRadius(ColliderPtr collider, float radius)
{
	if(collider != NULL)
	{
		((ColliderCirclePtr)collider)->radius = radius;
	}

}

bool ColliderCircleIsCollidingWithCircle(ColliderPtr collider, ColliderPtr other)
{

	ColliderCirclePtr colliderCircle = (ColliderCirclePtr)collider;
	ColliderCirclePtr otherCircle = (ColliderCirclePtr)other;
	
	GameObjectPtr colliderGO = collider->parent;
	GameObjectPtr otherGO = other->parent;

	TransformPtr colliderTransform = GameObjectGetTransform(colliderGO);
	TransformPtr otherTransform = GameObjectGetTransform(otherGO);

	Vector2D colliderTranslation = *TransformGetTranslation(colliderTransform);
	Vector2D otherTranslation = *TransformGetTranslation(otherTransform);

	float dist = Vector2DDistance(&colliderTranslation, &otherTranslation);
	float rad = (colliderCircle->radius + otherCircle->radius);

	if(dist < rad)
	{
		return true;
	}
		
	return false;

}
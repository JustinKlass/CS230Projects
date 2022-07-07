//-----------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "AEEngine.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Transform.h"

typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Rotational velocity (in radians).
	float rotationalVelocity;

	// Drag to be applied to moving objects in order to slow them down.
	float drag;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	//float		inverseMass;

} Physics;

PhysicsPtr PhysicsCreate(void)
{
	PhysicsPtr newPhysics = (PhysicsPtr)calloc(1, sizeof(Physics));

	if (newPhysics != NULL)
	{
		return newPhysics;
	}
	else
	{
		return NULL;
	}
}

PhysicsPtr PhysicsClone(const PhysicsPtr other)
{
	PhysicsPtr physicsClone = PhysicsCreate();

	if (other != NULL && physicsClone != NULL)
	{
		*physicsClone = *other;
		return physicsClone;
	}
	else
	{
		return NULL;
	}
}

void PhysicsFree(PhysicsPtr* physics)
{
	if(*physics != NULL)
	{
		free(*physics);
		*physics = NULL;
	}
}

void PhysicsRead(PhysicsPtr physics, Stream stream)
{
	if(physics != NULL && stream != NULL)
	{
		StreamReadVector2D(stream, &physics->acceleration);
		StreamReadVector2D(stream, &physics->velocity);
	}
}

const Vector2D* PhysicsGetAcceleration(const PhysicsPtr physics)
{
	if(physics != NULL)
	{
		return &physics->acceleration;
	}
	else
	{
		return NULL;
	}
}

const Vector2D* PhysicsGetVelocity(const PhysicsPtr physics)
{
	if (physics != NULL)
	{
		return &physics->velocity;
	}
	else
	{
		return NULL;
	}
}

float PhysicsGetRotationalVelocity(PhysicsPtr physics)
{
	if(physics != NULL)
	{
		return physics->rotationalVelocity;
	}
	else
	{
		return 0.0f;
	}
}

const Vector2D* PhysicsGetOldTranslation(PhysicsPtr physics)
{
	if (physics != NULL)
	{
		return &physics->oldTranslation;
	}
	else
	{
		return NULL;
	}
}

void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D* acceleration)
{
	if(physics != NULL && acceleration != NULL)
	{
		physics->acceleration = *acceleration;
	}
}

void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D* velocity)
{
	if (physics != NULL && velocity != NULL)
	{
		physics->velocity = *velocity;
	}
}

void PhysicsSetRotationalVelocity(PhysicsPtr physics, float rotationalVelocity)
{
	if(physics != NULL)
	{
		physics->rotationalVelocity = rotationalVelocity;
	}
}

void PhysicsSetDrag(PhysicsPtr physics, float drag)
{
	if(physics != NULL)
	{
		physics->drag = AEClamp(drag, 0.0f, 1.0f);
	}
}

void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt)
{
	if(physics != NULL && transform != NULL)
	{
		physics->oldTranslation = *TransformGetTranslation(transform);
		Vector2DScaleAdd(&physics->velocity, &physics->acceleration, &physics->velocity, dt);

		Vector2D newTranslation;
		Vector2DScaleAdd(&newTranslation, &physics->velocity, &physics->oldTranslation, dt);
		TransformSetTranslation(transform, &newTranslation);

		float rotation = TransformGetRotation(transform);
		rotation += physics->rotationalVelocity * dt;
		TransformSetRotation(transform, rotation);

	}
}
//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 6
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderLine.h"
#include "Collider.h"
#include "Stream.h"
#include "Vector2D.h"
#include "GameObject.h"
#include "Physics.h"
#include "Transform.h"

#define cLineSegmentMax 15

typedef struct ColliderLineSegment
{
	// A single line segment (P0 and P1).
	Vector2D	point[2];
} ColliderLineSegment;

typedef struct ColliderLine
{
	// Inherit the base collider structure.
	Collider	base;

	// The number of line segments in the list.
	unsigned int		lineCount;

	// The individual line segments.
	// (NOTE: Make sure to allocate enough memory for all line segments!)
	ColliderLineSegment	lineSegments[cLineSegmentMax];

} ColliderLine, * ColliderLinePtr;

ColliderPtr ColliderLineCreate(void)
{
	ColliderLinePtr newCollider = (ColliderLinePtr)calloc(1, sizeof(ColliderLine));

	if (newCollider != NULL)
	{
		newCollider->base.memorySize = sizeof(ColliderLine);
		newCollider->base.type = ColliderType::ColliderTypeLine;
		newCollider->base.parent = NULL;
		newCollider->base.handler = NULL;

		return (ColliderPtr)newCollider;
	}
	else
	{
		return NULL;
	}
}

void ColliderLineRead(ColliderPtr collider, Stream stream)
{
	if(collider != NULL && stream != NULL)
	{
		int numLines = StreamReadInt(stream);
		for(int i  = 0; i < numLines; i++)
		{
			Vector2D p0;
			Vector2D p1;
			StreamReadVector2D(stream, &p0);
			StreamReadVector2D(stream, &p1);
			ColliderLineAddLineSegment(collider, &p0, &p1);
		}
	}
}

void ColliderLineAddLineSegment(ColliderPtr collider, const Vector2D* p0, const Vector2D* p1)
{
	if(collider != NULL && p0 != NULL && p1 != NULL)
	{
		ColliderLinePtr temp = ((ColliderLinePtr)collider);
		temp->lineSegments[temp->lineCount].point[0] = *p0;
		temp->lineSegments[temp->lineCount].point[1] = *p1;
		temp->lineCount += 1;
	}
}

bool ColliderLineIsCollidingWithCircle(ColliderPtr collider, ColliderPtr other)
{
	if(collider != NULL && other != NULL)
	{
		PhysicsPtr otherPhysics = GameObjectGetPhysics(other->parent);
		TransformPtr otherTransform = GameObjectGetTransform(other->parent);
		//PhysicsPtr otherPhysics = GameObjectGetPhysics(other->parent);

		Vector2D Bs = *PhysicsGetOldTranslation(otherPhysics);
		Vector2D Be = *TransformGetTranslation(otherTransform);

		Vector2D v;
		Vector2DSub(&v, &Be, &Bs);

		ColliderLinePtr colliderLine = (ColliderLinePtr)collider;
		for(unsigned int iterator = 0; iterator < colliderLine->lineCount; iterator++)
		{
			Vector2D p0 = colliderLine->lineSegments[iterator].point[0];
			Vector2D p1 = colliderLine->lineSegments[iterator].point[1];
			
			Vector2D e;
			Vector2DSub(&e, &p1, &p0);

			Vector2D n;
			Vector2DSet(&n, e.y, -e.x);

			Vector2DNormalize(&n, &n);

			if ((Vector2DDotProduct(&n, &v)) == 0)
			{
				continue;
			}

			// INSIDE HALF PLANE
			if( Vector2DDotProduct(&n, &Bs) <= Vector2DDotProduct(&n, &p0) && 
				Vector2DDotProduct(&n, &Be) < Vector2DDotProduct(&n, &p0) )
			{
				continue;
			}

			// OUTSIDE HALF PLANE
			if( Vector2DDotProduct(&n, &Bs) >= Vector2DDotProduct(&n, &p0) && 
				Vector2DDotProduct(&n, &Be) > Vector2DDotProduct(&n, &p0) )
			{
				continue;
			}

			float ti = (Vector2DDotProduct(&n, &p0) - Vector2DDotProduct(&n, &Bs)) / Vector2DDotProduct(&n, &v);

			Vector2D Bi;
			Vector2DScale(&v, &v, ti);
			Vector2DAdd(&Bi, &Bs, &v);

			Vector2D temp;
			Vector2D otherTemp;
			Vector2DSub(&temp, &p1, &p0);
			Vector2DSub(&otherTemp, &Bi, &p0);

			if(Vector2DDotProduct(&temp, &otherTemp) < 0)
			{
				continue;
			}

			Vector2DSub(&temp, &p0, &p1);
			Vector2DSub(&otherTemp, &Bi, &p1);
			if(Vector2DDotProduct(&temp, &otherTemp) < 0)
			{
				continue;
			}

			Vector2D i;
			Vector2DSub(&i, &Be, &Bi);

			Vector2D s;
			Vector2DScale(&s, &n, Vector2DDotProduct(&i, &n));

			Vector2D r;
			Vector2DScale(&temp, &n, (2 * Vector2DDotProduct(&i, &n)));
			Vector2DSub(&r, &i, &temp);

			Vector2D Br;
			Vector2DAdd(&Br, &Bi, &r);

			float angle = Vector2DToAngleRad(&r);
			TransformSetRotation(otherTransform, angle);

			Vector2D oldVel = *PhysicsGetVelocity(otherPhysics);
			float speed = Vector2DLength(&oldVel);

			Vector2DNormalize(&r, &r);
			Vector2D vel;
			Vector2DScale(&vel, &r, speed);
			PhysicsSetVelocity(otherPhysics, &vel);
			TransformSetTranslation(otherTransform, &Br);

			continue;
		}

	}

	return false;
}
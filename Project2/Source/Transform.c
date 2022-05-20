//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230SU22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

typedef struct Transform
{
	// The translation (or world position) of a game object.
	Vector2D	translation;

	// The rotation (or orientation) of a game object (in radians).
	float	rotation;

	// The scale (or size) of a game object.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	scale;

} Transform;

TransformPtr TransformCreate(void)
{
	TransformPtr newTransform = calloc(1, sizeof(Transform));

	if(newTransform != NULL)
	{
		return newTransform;
	}
	else
	{
		return NULL;
	}
}

void TransformFree(TransformPtr* transform)
{
	if(*transform != NULL)
	{
		free(*transform);
		*transform = NULL;
	}
}

void TransformRead(TransformPtr transform, Stream stream)
{
	if(transform != NULL && stream != NULL)
	{
		float p1;

		StreamReadVector2D(stream, &transform->translation);
		p1 = StreamReadFloat(stream);
		StreamReadVector2D(stream, &transform->scale);

		transform->rotation = p1;
	}
}

const Vector2D* TransformGetTranslation(const TransformPtr transform)
{
	const Vector2D* translationPtr = &transform->translation;
	if(transform != NULL)
	{
		return translationPtr;
	}
	else
	{
		return NULL;
	}
}

float TransformGetRotation(const TransformPtr transform)
{
	if(transform != NULL)
	{
		return transform->rotation;
	}
	else
	{
		return 0.0f;
	}
}

const Vector2D* TransformGetScale(const TransformPtr transform)
{
	if(transform != NULL)
	{
		return &transform->scale;
	}
	else
	{
		return NULL;
	}
}

void TransformSetTranslation(TransformPtr transform, const Vector2D* translation)
{
	Vector2DSet(&transform->translation, translation->x, translation->y);
}

void TransformSetRotation(TransformPtr transform, float rotation)
{
	transform->rotation = rotation;
}

void TransformSetScale(TransformPtr transform, const Vector2D* scale)
{
	Vector2DSet(&transform->scale, scale->x, scale->y);
}
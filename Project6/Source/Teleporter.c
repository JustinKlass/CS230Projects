//------------------------------------------------------------------------------
//
// File Name:	Teleporter.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "Teleporter.h"
#include "GameObject.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Transform.h"

void TeleporterUpdateObject(GameObjectPtr gameObject)
{
	if(gameObject != NULL)
	{
		TransformPtr transform = GameObjectGetTransform(gameObject);

		Vector2D position = *TransformGetTranslation(transform);

		float maxX = AEGfxGetWinMaxX();
		float minX = AEGfxGetWinMinX();

		float maxY = AEGfxGetWinMaxY();
		float minY = AEGfxGetWinMinY();

		if(position.x > maxX)
		{
			position.x = minX;
		}

		else if(position.x < minX)
		{
			position.x = maxX;
		}

		if(position.y > maxY)
		{
			position.y = minY;
		}

		else if(position.y < minY)
		{
			position.y = maxY;
		}

		TransformSetTranslation(transform, &position);
	}
}
//------------------------------------------------------------------------------
//
// File Name:	Behavior.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Behavior.h"
#include "Stream.h"

BehaviorPtr BehaviorClone(BehaviorPtr other)
{
	if(other != NULL)
	{
		BehaviorPtr newBehavior = calloc(1, other->memorySize);
		if (newBehavior != NULL)
		{
			memcpy_s(newBehavior, other->memorySize, other, other->memorySize);
			return newBehavior;
		}
	}

	return NULL;
}

void BehaviorFree(BehaviorPtr* behavior)
{
	if(*behavior != NULL)
	{
  		free(*behavior);
		*behavior = NULL;
	}
}

void BehaviorRead(BehaviorPtr behavior, Stream stream)
{
	int stateCurr = StreamReadInt(stream);
	int stateNext = StreamReadInt(stream);
	float timer = StreamReadFloat(stream);

	behavior->stateCurr = stateCurr;
	behavior->stateNext = stateNext;
	behavior->timer = timer;
}

void BehaviorSetParent(BehaviorPtr behavior, GameObjectPtr parent)
{
	if(behavior != NULL && parent != NULL)
	{
		behavior->parent = parent;
	}
}

void BehaviorUpdate(BehaviorPtr behavior, float dt)
{
	if(behavior != NULL)
	{
		if(behavior->stateCurr != behavior->stateNext)
		{
			if(behavior->onExit != NULL)
			{
				behavior->onExit(behavior);
			}

			behavior->stateCurr = behavior->stateNext;

			if(behavior->onInit != NULL)
			{
    				behavior->onInit(behavior);
			}
		}

		if(behavior->onUpdate != NULL)
		{
         	behavior->onUpdate(behavior, dt);
		}
	}
}
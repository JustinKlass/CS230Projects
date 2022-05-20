//-----------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "Stream.h"

GameObjectPtr GameObjectFactoryBuild(const char* filename)
{	
	if(filename != NULL)
	{
		Stream newStream = StreamOpen(filename);

		if(newStream != NULL)
		{
			if(strncmp(StreamReadToken(newStream), "GameObject", _countof("GameObject")) == 0)
			{
				GameObjectPtr newGameObj = GameObjectCreate();
				GameObjectRead(newGameObj, newStream);
				StreamClose(&newStream);
				return newGameObj;
			}
			else
			{
				StreamClose(&newStream);
				return NULL;
			}
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
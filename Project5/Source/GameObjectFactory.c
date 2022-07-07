//-----------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 4
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Stream.h"

GameObjectPtr GameObjectFactoryBuild(const char* objectName)
{	
	if(objectName != NULL)
	{
		GameObjectPtr tempObj = GameObjectManagerGetArchetype(objectName);
		if(tempObj == NULL)
		{
			char pathName[FILENAME_MAX] = "";
			sprintf_s(pathName, _countof(pathName), "Data/%s.txt", objectName);

			Stream newStream = StreamOpen(pathName);

			if (newStream != NULL)
			{
				if (strncmp(StreamReadToken(newStream), "GameObject", _countof("GameObject")) == 0)
				{
					tempObj = GameObjectCreate();
					GameObjectRead(tempObj, newStream);
					GameObjectManagerAddArchetype(tempObj);

					StreamClose(&newStream);
					GameObjectPtr objClone = GameObjectClone(tempObj);
					return objClone;
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
		
		GameObjectPtr newGameObj = GameObjectClone(tempObj);
		return newGameObj;
	}
	else
	{
		return NULL;
	}
}
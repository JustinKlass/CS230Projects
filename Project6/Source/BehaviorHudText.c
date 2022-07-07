//------------------------------------------------------------------------------
//
// File Name:	BehaviorHudText.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorHudText.h"
#include "Behavior.h"
#include "GameObject.h"
#include "Sprite.h"
#include "ScoreSystem.h"
#include "Stream.h"

typedef enum HudTextStates
{
	cHudTextInvalid = -1,	// HUD Text has not yet been initialized.
	cHudTextIdle,			// HUD Text will normally remain static.

} HudTextStates;


typedef struct BehaviorHudText
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add HUD Text-specific behavior variables.

	// The index used to access values from the Score System.
	ScoreSystemId scoreSystemId;

	// The format string to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: %d")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char formatString[32];

	// The buffer to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: 9001")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char displayString[32];

	// The value currently being displayed by the HUD Text object.
	// (NOTE: This value can be compared with *watchValue to determine when the text must be updated.)
	// (NOTE: Make sure to update this value each time the text is updated.)
	int displayValue;

} BehaviorHudText, * BehaviorHudTextPtr;

static void BehaviorHudTextUpdateText(BehaviorHudTextPtr behavior);

BehaviorPtr BehaviorHudTextCreate(void)
{
	BehaviorHudTextPtr newHudText = calloc(1, sizeof(BehaviorHudText));

	if (newHudText != NULL)
	{
		newHudText->base.onInit = &BehaviorHudTextInit;
		newHudText->base.onUpdate = &BehaviorHudTextUpdate;
		newHudText->base.onExit = &BehaviorHudTextExit;
		newHudText->base.memorySize = sizeof(BehaviorHudText);
		newHudText->base.stateCurr = cHudTextInvalid;
		newHudText->base.stateNext = cHudTextInvalid;
		newHudText->scoreSystemId = SsiInvalid;

		return (BehaviorPtr)newHudText;
	}
	else
	{
		return NULL;
	}
}

void BehaviorHudTextRead(BehaviorPtr behavior, Stream stream)
{
	if(behavior != NULL && stream != NULL)
	{
		BehaviorRead(behavior, stream);
		BehaviorHudTextPtr behaviorHud = (BehaviorHudTextPtr)behavior;

		const char* token = StreamReadToken(stream);
		strncpy_s(behaviorHud->formatString, _countof(behaviorHud->formatString), token, _countof(behaviorHud->formatString));
		
		behaviorHud->scoreSystemId = StreamReadInt(stream);
	}
}

void BehaviorHudTextInit(BehaviorPtr behavior)
{
	if(behavior != NULL)
	{
		BehaviorHudTextPtr behaviorHud = (BehaviorHudTextPtr)behavior;
		BehaviorHudTextUpdateText(behaviorHud);

		SpritePtr sprite = GameObjectGetSprite(behaviorHud->base.parent);
		SpriteSetText(sprite, behaviorHud->displayString);
	}
}

void BehaviorHudTextUpdate(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if(behavior != NULL)
	{
		BehaviorHudTextPtr behaviorHud = (BehaviorHudTextPtr)behavior;
		int score = ScoreSystemGetValue(behaviorHud->scoreSystemId);

		if(behaviorHud->displayValue != score)
		{
			BehaviorHudTextUpdateText(behaviorHud);
		}
		
	}
}

void BehaviorHudTextExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorHudTextUpdateText(BehaviorHudTextPtr behavior)
{
	if(behavior != NULL)
	{
		if(behavior->scoreSystemId != SsiInvalid)
		{
			behavior->displayValue = ScoreSystemGetValue(behavior->scoreSystemId);
			sprintf_s(behavior->displayString, _countof(behavior->displayString), behavior->formatString, behavior->displayValue);
		}
	}
}
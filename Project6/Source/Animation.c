//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 4
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "Stream.h"
#include "Sprite.h"
#include "GameObject.h"

typedef struct Animation
{
	// Pointer to the parent game object associated with the animation.
	GameObjectPtr parent;

	// The current frame being displayed.
	unsigned int frameIndex;

	// The maximum number of frames in the sequence.
	unsigned int frameCount;

	// The time remaining for the current frame.
	float frameDelay;

	// The amount of time to display each successive frame.
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool isRunning;

	// True if the animation loops back to the beginning.
	bool isLooping;

	// True if the end of the animation has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)
	bool isDone;

	// The animation sequence currently being played, if any.
	//AnimationSequencePtr	sequence;

} Animation;


static void AnimationAdvanceFrame(AnimationPtr animation)
{
	if (animation != NULL)
	{
		animation->frameIndex++;
		if (animation->frameIndex >= animation->frameCount)
		{
			if (animation->isLooping)
			{
				animation->frameIndex = 0;
			}
			else
			{
				animation->frameIndex = animation->frameCount - 1;
				animation->isRunning = false;
			}
			animation->isDone = true;

			if (animation->isRunning)
			{
				SpriteSetFrame(GameObjectGetSprite(animation->parent), animation->frameIndex);
				animation->frameDelay += animation->frameDuration;
			}
			else
			{
				animation->frameDelay = 0;
			}
			return;
		}
		else if(animation->isRunning)
		{

			SpriteSetFrame(GameObjectGetSprite(animation->parent), animation->frameIndex);
			animation->frameDelay += animation->frameDuration;

		}
		else
		{
		animation->frameDelay = 0;
		}
		return;
	}
}

AnimationPtr AnimationCreate(void)
{
	AnimationPtr newAnimation = calloc(1, sizeof(Animation));

	if (newAnimation != NULL)
	{
		return newAnimation;
	}
	else
	{
		return NULL;
	}
}

AnimationPtr AnimationClone(const AnimationPtr other)
{
	if(other != NULL)
	{
		AnimationPtr animationClone = AnimationCreate();

		if (animationClone != NULL)
		{
			*animationClone = *other;
			return animationClone;
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

void AnimationFree(AnimationPtr* animation)
{
	if (*animation != NULL)
	{
		free(*animation);
		*animation = NULL;
	}
}

void AnimationRead(AnimationPtr animation, Stream stream)
{
	if (animation != NULL && stream != NULL)
	{
		int frameIndex;
		int frameCount;

		float frameDelay;
		float frameDuration;

		bool isRunning;
		bool isLooping;

		frameIndex = StreamReadInt(stream);
		frameCount = StreamReadInt(stream);

		frameDelay = StreamReadFloat(stream);
		frameDuration = StreamReadFloat(stream);

		isRunning = StreamReadBoolean(stream);
		isLooping = StreamReadBoolean(stream);

		animation->frameIndex = frameIndex;
		animation->frameCount = frameCount;

		animation->frameDelay = frameDelay;
		animation->frameDuration = frameDuration;

		animation->isRunning = isRunning;
		animation->isLooping = isLooping;
	}
}

void AnimationSetParent(AnimationPtr animation, GameObjectPtr parent)
{
	if(animation != NULL && parent != NULL)
	{
		animation->parent = parent;
	}
}

void AnimationPlay(AnimationPtr animation, int frameCount, float frameDuration, bool isLooping)
{
	if(animation != NULL)
	{
		SpritePtr newSprite = GameObjectGetSprite(animation->parent);
		animation->frameCount = frameCount;
		animation->frameDuration = frameDuration;
		animation->isLooping = isLooping;

		animation->frameDelay = 0;
		animation->frameIndex = 0;
		animation->isRunning = true;
		animation->isDone = false;

		SpriteSetFrame(newSprite, animation->frameIndex);
	}
}

void AnimationUpdate(AnimationPtr animation, float dt)
{
	if(animation != NULL)
	{
		animation->isDone = false;
		
		if(animation->isRunning == true)
		{
			animation->frameDelay -= dt;

			if(animation->frameDelay <= 0)
			{
				AnimationAdvanceFrame(animation);
			}
		}
		
	}
}

bool AnimationIsDone(AnimationPtr animation)
{
	if(animation != NULL)
	{
		return animation->isDone;
	}
	else
	{
		return false;
	}

}
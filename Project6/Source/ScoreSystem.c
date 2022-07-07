//------------------------------------------------------------------------------
//
// File Name:	ScoreSystem.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 6
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "ScoreSystem.h"

static int asteroidScore;
static int asteroidHighScore;
static int asteroidWaveCount;



void ScoreSystemInit()
{

	asteroidHighScore = 0;
	asteroidScore = 0;
	asteroidWaveCount = 0;
}

void ScoreSystemRestart()
{
	if(asteroidScore > asteroidHighScore)
	{
		asteroidHighScore = asteroidScore;
	}
	asteroidScore = 0;
	asteroidWaveCount = 0;
}

int ScoreSystemGetValue(ScoreSystemId ssid)
{
	if(ssid > SsiInvalid && ssid < SsiMax)
	{
		if(ssid == SsiScore)
		{
			return asteroidScore;
		}
		else if(ssid == SsiHighScore)
		{
			return asteroidHighScore;
		}
		else if(ssid == SsiWaveCount)
		{
			return asteroidWaveCount;
		}
	}
	return 0;
}

void ScoreSystemIncreaseScore(unsigned int scoreValue)
{
 	asteroidScore += scoreValue;
}

void ScoreSystemIncreaseWaveCount()
{
	asteroidWaveCount += 1;
}
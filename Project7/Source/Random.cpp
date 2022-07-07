//------------------------------------------------------------------------------
//
// File Name:	Random.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 5
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Random.h"
#include <stdlib.h> // rand(), srand()

void RandomInit()
{
	// Code from Mircosoft Documentation - Link below to original code
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand?view=msvc-170
	srand(1792);
	// cast time function to unsigned int and pass it
}

int RandomRange(int rangeMin, int rangeMax)
{
	int randomVal = (rand() % (rangeMax - rangeMin + 1) + rangeMin);

	return randomVal;
}

float RandomRangeFloat(float rangeMin, float rangeMax)
{
	float randomVal = rangeMin + (float)rand() / ((float)RAND_MAX / (rangeMax - rangeMin));


	return randomVal;
}
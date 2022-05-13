//------------------------------------------------------------------------------
//
// File Name:	Stream.c
// Author(s):	Justin Klass (justin.klass)
// Project:		Project 1
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stream.h"
#include "trace.h"
#include "Vector2D.h"

Stream StreamOpen(const char* filename)
{
	errno_t err;
	Stream file;

	// Open for read (will fail if file "crt_fopen_s.c" doesn't exist)
	err = fopen_s(&file, filename, "rt");
	if (err == 0)
	{
		return file;
	}
	else
	{
		char errorMsg[1024];
		strerror_s(errorMsg, 1024, err);
		TraceMessage("Error: StreamOpen could not open file %s; %s", filename, errorMsg);
	}
	return NULL;
}

int StreamReadInt(Stream stream)
{
	if(stream == NULL)
	{
		return 0;
	}
	int streamInt;
	fscanf_s(stream, "%i", &streamInt);
	return streamInt;
}

float StreamReadFloat(Stream stream)
{
	float streamFloat;
	if (stream == NULL)
	{
		return 0;
	}
	fscanf_s(stream, "%f", &streamFloat);
	return streamFloat;
}

void StreamReadVector2D(Stream stream, Vector2D* vector)
{

	if (stream != NULL && vector != NULL)
	{
		vector->x = StreamReadFloat(stream);
		vector->y = StreamReadFloat(stream);
	}
	else
	{
		TraceMessage("Error: Stream or Vector is invalid");
	}
}

void StreamClose(Stream * stream)
{
	if(stream != NULL)
	{
		fclose((FILE*)*stream);
	}
}
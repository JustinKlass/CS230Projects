//------------------------------------------------------------------------------
//
// File Name:	Trace.c
// Author(s):	Doug Schilling (dschilling)
//				Justin Klass (justin.klass)
// Project:		Project 1
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdarg.h>

#include "AEEngine.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const char* traceFileName = "Trace.log";

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

// TODO: Declare a private variable for storing a file handle.
static FILE* traceFile;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Tracing/Logging module.
// - Open "trace.log" for writing in text mode.
// - Perform error handling in the event that the file failed to open.
void TraceInit()
{
	// TODO: Open "trace.log" for writing (text mode).
	// fopen_s:
	// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(stdio%2Ffopen_s);k(fopen_s);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
	errno_t err;

	// Open for read (will fail if file "crt_fopen_s.c" doesn't exist)
	err = fopen_s(&traceFile, "Trace.log", "wt");
	if (err == 0)
	{
		AESysPrintf("The file 'Trace.log' was opened\n");
	}
	else
	{
		AESysPrintf("The file 'Trace.log' was not opened\n");
	}

	// Error handling (implementation details to be determined by the student):
	// https://msdn.microsoft.com/en-us/library/9t0e6085.aspx
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-s-strerror-s-wcserror-s-wcserror-s?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev16.query%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(STRING%2Fstrerror_s);k(strerror_s);k(DevLang-C%2B%2B);k(TargetOS-Windows)%26rd%3Dtrue&view=vs-2019

}

// Output a message to the Tracing/Logging file.
// - Print the given message to the file if it was opened successfully.
// - Every message must be printed on its own line.
// - There must be no blank lines between messages.
void TraceMessage(const char * formatString, ...)
{
	UNREFERENCED_PARAMETER(formatString);

	// TODO: Write the message to the Tracing/Logging file.

	if(traceFile != NULL)
	{
		va_list args;

		va_start(args, formatString);
		vfprintf(traceFile, formatString, args);
		fprintf_s(traceFile, "\n");
		va_end(args);

	}

}

// Shutdown the Tracing/Logging module.
// - Close the file if-and-only-if the file was opened successfully
void TraceShutdown()
{
	// TODO: Close "trace.log" if it has been opened successfully.
	if(traceFile != NULL)
	{
		fclose(traceFile);
	}

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


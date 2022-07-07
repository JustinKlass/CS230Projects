//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 1
// Course:		CS230S22
//
// Copyright � 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateSandboxLoad();

// Initialize the memory associated with the Stub game state.
void GameStateSandboxInit();

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateSandboxUpdate(float dt);

// Shutdown any memory associated with the Stub game state.
void GameStateSandboxShutdown();

// Unload the resources associated with the Stub game state.
void GameStateSandboxUnload();

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif


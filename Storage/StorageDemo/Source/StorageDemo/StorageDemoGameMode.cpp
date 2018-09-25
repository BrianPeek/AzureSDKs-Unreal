// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StorageDemoGameMode.h"
#include "StorageDemoPlayerController.h"
#include "StorageDemoPawn.h"

AStorageDemoGameMode::AStorageDemoGameMode()
{
	// no pawn by default
	DefaultPawnClass = AStorageDemoPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AStorageDemoPlayerController::StaticClass();
}

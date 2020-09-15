// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestCommands.h"

#define LOCTEXT_NAMESPACE "FTestModule"

void FTestCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Test", "Bring up Test window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TestStyle.h"

class FTestCommands : public TCommands<FTestCommands>
{
public:

	FTestCommands()
		: TCommands<FTestCommands>(TEXT("Test"), NSLOCTEXT("Contexts", "Test", "Test Plugin"), NAME_None, FTestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Widgets/SWidget.h"
#include "Modules/ModuleInterface.h"
#include "Widgets/SWindow.h"
#include "Features/IModularFeatures.h"
#include "HAL/IConsoleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class SMultiLineEditableTextBox;


namespace EDebugConsoleStyle
{
	enum Type
	{
		/** Shows the debug console input line with tab completion only */
		Compact,

		/** Shows a scrollable log window with the input line on the bottom */
		WithLog,
	};
};

struct FDebugConsoleDelegates
{
	FSimpleDelegate OnFocusLost;
	FSimpleDelegate OnConsoleCommandExecuted;
	FSimpleDelegate OnCloseConsole;
};

class FTestModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	/** Generates a console input box widget.  Remember, this widget will become invalid if the
	output log DLL is unloaded on the fly. */
	virtual TSharedRef< SWidget > MakeConsoleInputBox(TSharedPtr< SMultiLineEditableTextBox >& OutExposedEditableTextBox) const;

	/** Opens a debug console in the specified window, if not already open */
	virtual void ToggleDebugConsoleForWindow(const TSharedRef< SWindow >& Window, const EDebugConsoleStyle::Type InStyle, const FDebugConsoleDelegates& DebugConsoleDelegates);

	/** Closes the debug console for the specified window */
	virtual void CloseDebugConsole();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	/** Weak pointer to a debug console that's currently open, if any */
	TWeakPtr< SWidget > DebugConsole;

};
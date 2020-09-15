// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Test.h"
#include "TestStyle.h"
#include "TestCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "Features/IModularFeatures.h"
#include "Modules/ModuleManager.h"
#include "Framework/Application/SlateApplication.h"
#include "Textures/SlateIcon.h"
#include "Framework/Docking/TabManager.h"
#include "EditorStyleSet.h"
#include "SDebugConsole.h"
#include "SOutputLog.h"
#include "SDeviceOutputLog.h"
#include "Editor/WorkspaceMenuStructure/Public/WorkspaceMenuStructure.h"
#include "Editor/WorkspaceMenuStructure/Public/WorkspaceMenuStructureModule.h"


static const FName TestTabName("Test");

#define LOCTEXT_NAMESPACE "FTestModule"

namespace OutputLogModule
{
	static const FName OutputLogTabName = FName(TEXT("OutputLog"));
	static const FName DeviceOutputLogTabName = FName(TEXT("DeviceOutputLog"));
}

/** This class is to capture all log output even if the log window is closed */
class FOutputLogHistory : public FOutputDevice
{
public:

	FOutputLogHistory()
	{
		GLog->AddOutputDevice(this);
		GLog->SerializeBacklog(this);
	}

	~FOutputLogHistory()
	{
		// At shutdown, GLog may already be null
		if (GLog != NULL)
		{
			GLog->RemoveOutputDevice(this);
		}
	}

	/** Gets all captured messages */
	const TArray< TSharedPtr<FOutputLogMessage> >& GetMessages() const
	{
		return Messages;
	}

protected:

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category) override
	{
		// Capture all incoming messages and store them in history
		SOutputLog::CreateLogMessages(V, Verbosity, Category, Messages);
	}

private:

	/** All log messsges since this module has been started */
	TArray< TSharedPtr<FOutputLogMessage> > Messages;
};

/** Our global output log app spawner */
static TSharedPtr<FOutputLogHistory> OutputLogHistory;

void FTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTestStyle::Initialize();
	FTestStyle::ReloadTextures();

	FTestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTestCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FTestModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FTestModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FTestModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	UE_LOG(LogTemp, Warning, TEXT("StartupModule"));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestTabName, FOnSpawnTab::CreateRaw(this, &FTestModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FTestTabTitle", "Test"))
		.SetTooltipText(NSLOCTEXT("UnrealEditor", "OutputLogTooltipText", "Open the Output Log tab."))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetDeveloperToolsLogCategory())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "Log.TabIcon"));

	OutputLogHistory = MakeShareable(new FOutputLogHistory);
}

void FTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FTestStyle::Shutdown();

	FTestCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TestTabName);

	if (FSlateApplication::IsInitialized())
	{
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(OutputLogModule::OutputLogTabName);
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(OutputLogModule::DeviceOutputLogTabName);
	}

	OutputLogHistory.Reset();

	UE_LOG(LogTemp, Warning, TEXT("ShutdownModule"));
}

TSharedRef<SDockTab> SpawnOutputLog(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("Log.TabIcon"))
		.TabRole(ETabRole::NomadTab)
		.Label(NSLOCTEXT("OutputLog", "TabTitle", "Output Log"))
		[
			SNew(SOutputLog).Messages(OutputLogHistory->GetMessages())
		];
}

TSharedRef<SDockTab> FTestModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSpawnPluginTab"));
	
	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("Log.TabIcon"))
		.TabRole(ETabRole::NomadTab)
		.Label(NSLOCTEXT("OutputLog", "TabTitle", "Output Log"))
		[
			SNew(SOutputLog).Messages(OutputLogHistory->GetMessages())
		];

}

void FTestModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(TestTabName);
}

void FTestModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FTestCommands::Get().OpenPluginWindow);
}

void FTestModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FTestCommands::Get().OpenPluginWindow);
}


TSharedRef< SWidget > FTestModule::MakeConsoleInputBox(TSharedPtr< SMultiLineEditableTextBox >& OutExposedEditableTextBox) const
{
	TSharedRef< SConsoleInputBox > NewConsoleInputBox = SNew(SConsoleInputBox);
	OutExposedEditableTextBox = NewConsoleInputBox->GetEditableTextBox();
	return NewConsoleInputBox;
}

void FTestModule::ToggleDebugConsoleForWindow(const TSharedRef< SWindow >& Window, const EDebugConsoleStyle::Type InStyle, const FDebugConsoleDelegates& DebugConsoleDelegates)
{
	bool bShouldOpen = true;
	// Close an existing console box, if there is one
	TSharedPtr< SWidget > PinnedDebugConsole(DebugConsole.Pin());
	if (PinnedDebugConsole.IsValid())
	{
		// If the console is already open close it unless it is in a different window.  In that case reopen it on that window
		bShouldOpen = false;
		TSharedPtr< SWindow > WindowForExistingConsole = FSlateApplication::Get().FindWidgetWindow(PinnedDebugConsole.ToSharedRef());
		if (WindowForExistingConsole.IsValid())
		{
			WindowForExistingConsole->RemoveOverlaySlot(PinnedDebugConsole.ToSharedRef());
			DebugConsole.Reset();
		}

		if (WindowForExistingConsole != Window)
		{
			// Console is being opened on another window
			bShouldOpen = true;
		}
	}

	TSharedPtr<SDockTab> ActiveTab = FGlobalTabmanager::Get()->GetActiveTab();
	if (ActiveTab.IsValid() && ActiveTab->GetLayoutIdentifier() == FTabId(OutputLogModule::OutputLogTabName))
	{
		FGlobalTabmanager::Get()->DrawAttention(ActiveTab.ToSharedRef());
		bShouldOpen = false;
	}

	if (bShouldOpen)
	{
		const EDebugConsoleStyle::Type DebugConsoleStyle = InStyle;
		TSharedRef< SDebugConsole > DebugConsoleRef = SNew(SDebugConsole, DebugConsoleStyle, this, &DebugConsoleDelegates);
		DebugConsole = DebugConsoleRef;

		const int32 MaximumZOrder = MAX_int32;
		Window->AddOverlaySlot(MaximumZOrder)
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Center)
			.Padding(10.0f)
			[
				DebugConsoleRef
			];

		// Force keyboard focus
		DebugConsoleRef->SetFocusToEditableText();
	}
}

void FTestModule::CloseDebugConsole()
{
	TSharedPtr< SWidget > PinnedDebugConsole(DebugConsole.Pin());

	if (PinnedDebugConsole.IsValid())
	{
		TSharedPtr< SWindow > WindowForExistingConsole = FSlateApplication::Get().FindWidgetWindow(PinnedDebugConsole.ToSharedRef());
		if (WindowForExistingConsole.IsValid())
		{
			WindowForExistingConsole->RemoveOverlaySlot(PinnedDebugConsole.ToSharedRef());
			DebugConsole.Reset();
		}
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestModule, Test)
// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;


	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor - Found class %s"), *MenuBPClass.Class->GetName());
}

void UPuzzlePlatformGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
	}
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init!!"));
}

void UPuzzlePlatformGameInstance::Host()
{
	UE_LOG(LogTemp, Warning, TEXT("Host"));
	if (Menu != nullptr)
	{
		Menu->Teardown();
		UE_LOG(LogTemp, Warning, TEXT("menu tear down"));
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* GameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(GameMenu != nullptr)) return;

	GameMenu->Setup();

	GameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
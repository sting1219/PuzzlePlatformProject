// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"


#define STAGE_NAME TEXT("Stage_")
#define STAGE_PATH TEXT("/Game/ThirdPersonCPP/Maps/")
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
		void InGameLoadMenu();

	UFUNCTION(Exec)
		void Host() override;

	UFUNCTION(Exec)
		void Join(const FString& Address) override;

	virtual void LoadMainMenu() override;



	//////////////////////////// Replay ////////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Replays")
		FString RecordingName;

	UPROPERTY(EditDefaultsOnly, Category = "Replays")
		FString FriendlyRecordingName;

	UFUNCTION(BlueprintCallable, Category = "Replays")
		void StartRecording();

	UFUNCTION(BlueprintCallable, Category = "Replays")
		void StopRecording();

	UFUNCTION(BlueprintCallable, Category = "Replays")
		void StartReplay();

	//////////////////////////// Replay ////////////////////////////////

public:
	UFUNCTION(Exec)
		void GoStage(int stagenum);

	UFUNCTION(Exec)
		void NextLevel();

	UFUNCTION(BlueprintCallable)
	bool GetIsFinish() { return IsFinish; }

	UFUNCTION(BlueprintCallable)
	void SetIsFInish(bool value) { IsFinish = value; }

	UFUNCTION(BlueprintCallable)
		void TestReplayRecording();
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;
	class UInGameMenu* InGameMenu;

	bool IsFinish;

	int CurrentStage_Number;

	float RemainTime;
};
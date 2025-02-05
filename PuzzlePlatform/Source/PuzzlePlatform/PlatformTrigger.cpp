// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"

#include "Components/BoxComponent.h"
#include "MovingPlatform.h"
#include "PuzzlePlatformCharacter.h"
#include "PuzzlePlatformGameInstance.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));

	if (!ensure(TriggerVolume != nullptr)) return;

	RootComponent = TriggerVolume;


	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Activated"));
	//for (AMovingPlatform* Platform : PlatformsToTrigger)
	//{
	//	Platform->AddActiveTrigger();
	//}

	if (OtherActor != nullptr)
	{
		APuzzlePlatformCharacter* playercharacter = Cast<APuzzlePlatformCharacter>(OtherActor);

		if (playercharacter != nullptr)
		{
			playercharacter->AddStageScore(100);
		}
	}

	if (GetGameInstance())
	{
		UPuzzlePlatformGameInstance* instance = Cast< UPuzzlePlatformGameInstance>(GetGameInstance());
		if (instance != nullptr)
		{
			instance->NextLevel();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"),*OtherActor->GetName());

}

void APlatformTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("DeActivated"));
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}
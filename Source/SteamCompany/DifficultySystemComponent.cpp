// Fill out your copyright notice in the Description page of Project Settings.

#include "DifficultySystemComponent.h"
#include "DifficultyWidget.h"
#include "EnemyManagerActor.h"
#include "LevelAdvancedFriendsGameInstance.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../../../../../../Source/Runtime/Core/Public/Containers/UnrealString.h"

UDifficultySystemComponent::UDifficultySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = 1;
}


// Called when the game starts
void UDifficultySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	DifficultyWidget = CreateWidget<UDifficultyWidget>(GetWorld(), DifficultyWidgetClass);
	if (!DifficultyWidget) {
		UE_LOG(LogTemp, Warning, TEXT("DifficultyWidget null"));
		return;
	}

	if (IsActive()) DifficultyWidget->AddToViewport();

	ULevelAdvancedFriendsGameInstance* LevelAdvancedFriendsGameInstance = GetWorld()->GetGameInstance<ULevelAdvancedFriendsGameInstance>();
	if (!LevelAdvancedFriendsGameInstance) {
		UE_LOG(LogTemp, Warning, TEXT("LevelAdvancedFriendsGameInstance null"));
		return;
	}

	DifficultyWidget->World = LevelAdvancedFriendsGameInstance->CurrentLevel;

	DifficultyWidget->SetDifficulty(((int)LevelAdvancedFriendsGameInstance->CurrentGameTime) / DifficultyTextInterval);

	AEnemyManagerActor* EnemyManagerActor = Cast<AEnemyManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManagerActor::StaticClass()));
	if (EnemyManagerActor) {
		EnemyManagerActor->OnGameTimeUpdate.AddDynamic(this, &UDifficultySystemComponent::GameTimeUpdate);
	}
}

void UDifficultySystemComponent::Activate(bool bReset)
{
	Super::Activate();
}

void UDifficultySystemComponent::Deactivate()
{
	Super::Deactivate();
}

void UDifficultySystemComponent::GameTimeUpdate(float NewTime) {
	// Calculate minutes and seconds from NewTime
	int Minutes = (int)NewTime / 60;
	int Seconds = (int)NewTime % 60;
	FString GameTimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	DifficultyWidget->GameTime = FText::FromString(GameTimeString);

	// Update Difficulty
	if (((int)NewTime) / DifficultyTextInterval > CurrentDifficulty - 1) {
		CurrentDifficulty++;
		DifficultyWidget->SetDifficulty(((int)NewTime) / DifficultyTextInterval);
	}
}

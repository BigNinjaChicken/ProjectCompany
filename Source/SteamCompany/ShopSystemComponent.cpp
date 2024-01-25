// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopSystemComponent.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputTriggers.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LevelAdvancedFriendsGameInstance.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "JesterActor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "ShopActor.h"

UShopSystemComponent::UShopSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShopSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is not ACharacter"));
		return;
	}

	PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController null"));
		return;
	}
}

void UShopSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShopSystemComponent::EndShop() {
	OnEndDialog.Broadcast();

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = StartingMaxWalkSpeed;
	GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimerHandle);

	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);

	CameraComponent->bUsePawnControlRotation = true;

	PlayerController->bShowMouseCursor = false;

	return;
}

void UShopSystemComponent::Interact()
{
	if (bDoOnce) { return; }
	bDoOnce = true;

	if (ItemOptions.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("No ItemOptions"));
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	PlayerController->SetInputMode(InputModeUIOnly);

	PlayerController->bShowMouseCursor = true;

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	StartingMaxWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
	CharacterMovementComponent->MaxWalkSpeed = 0.0f;

	LerpCameraToJester();

	TArray<TSubclassOf<UItemEffectComponent>> ItemsToPickFrom;
	ItemsToPickFrom.AddUnique(ItemOptions[FMath::RandRange(0, ItemOptions.Num() - 1)]);
	ItemsToPickFrom.AddUnique(ItemOptions[FMath::RandRange(0, ItemOptions.Num() - 1)]);
	ItemsToPickFrom.AddUnique(ItemOptions[FMath::RandRange(0, ItemOptions.Num() - 1)]);

	OnShopChange.Broadcast(ItemsToPickFrom);
}

void UShopSystemComponent::LerpCameraToJester()
{
	ShopKeeperActor = UGameplayStatics::GetActorOfClass(GetWorld(), AShopActor::StaticClass());
	if (!ShopKeeperActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("JesterActor is not set"));
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(CameraLerpTimerHandle, this, &UShopSystemComponent::UpdateCameraLerp, 0.01f, true);
}

void UShopSystemComponent::UpdateCameraLerp()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	CameraComponent = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
	CameraComponent->bUsePawnControlRotation = false;
	if (!CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent is not set"));
		return;
	}

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraComponent->GetComponentLocation(), ShopKeeperActor->GetActorLocation());
	FRotator NewRotation = FMath::RInterpTo(CameraComponent->GetComponentRotation(), TargetRotation, DeltaTime, 8.0f);
	CameraComponent->SetWorldRotation(NewRotation);
}


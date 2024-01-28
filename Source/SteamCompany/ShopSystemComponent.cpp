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

	GetStartingSpeed();
}

void UShopSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShopSystemComponent::EndShop() {
	OnEndDialog.Broadcast();

	ServerResetMovementSpeed();

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	if (CharacterMovementComponent && StartingMaxWalkSpeed != 0) CharacterMovementComponent->MaxWalkSpeed = StartingMaxWalkSpeed;
	
	FInputModeGameOnly InputModeGameOnly;
	if (PlayerController) {
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->bShowMouseCursor = false;
	}

	GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimerHandle);

	if (CameraComponent) CameraComponent->bUsePawnControlRotation = true;
}


void UShopSystemComponent::ServerResetMovementSpeed_Implementation()
{
	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	if (CharacterMovementComponent && StartingMaxWalkSpeed != 0) {

		UE_LOG(LogTemp, Warning, TEXT("Here1"));
		CharacterMovementComponent->MaxWalkSpeed = StartingMaxWalkSpeed;
	}

	FInputModeGameOnly InputModeGameOnly;
	if (PlayerController) {
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->bShowMouseCursor = false;
	}
}

void UShopSystemComponent::GetStartingSpeed_Implementation()
{

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	StartingMaxWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
}

void UShopSystemComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShopSystemComponent, StartingMaxWalkSpeed);
}

void UShopSystemComponent::Interact()
{
	if (bInShop) { return; }
	bInShop = true;

	if (ItemOptions.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("No ItemOptions"));
		return;
	}

	PlayerController = Cast<APlayerController>(Character->GetController());
	FInputModeUIOnly InputModeUIOnly;
	if (PlayerController) {
		PlayerController->SetInputMode(InputModeUIOnly);
		PlayerController->bShowMouseCursor = true;
	}
	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 0.0f;
	
	SetInteractSettings();

	LerpCameraToJester();

	TArray<TSubclassOf<UItemEffectComponent>> ItemsToPickFrom;
	ItemsToPickFrom.AddUnique(ItemOptions[FMath::RandRange(0, ItemOptions.Num() - 1)]);
	ItemsToPickFrom.AddUnique(ItemOptions[FMath::RandRange(0, ItemOptions.Num() - 1)]);
	ItemsToPickFrom.AddUnique(ItemOptions[FMath::RandRange(0, ItemOptions.Num() - 1)]);

	OnShopChange.Broadcast(ItemsToPickFrom);
}

void UShopSystemComponent::SetInteractSettings_Implementation()
{
	PlayerController = Cast<APlayerController>(Character->GetController());
	FInputModeUIOnly InputModeUIOnly;
	if (PlayerController) {
		PlayerController->SetInputMode(InputModeUIOnly);
		PlayerController->bShowMouseCursor = true;
	}

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	StartingMaxWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
	CharacterMovementComponent->MaxWalkSpeed = 0.0f;
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSystemComponent.h"
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

UDialogSystemComponent::UDialogSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDialogSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is not ACharacter"));
		return;
	}

	PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InteractMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &UDialogSystemComponent::Interact);
		}
	}
}

void UDialogSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDialogSystemComponent::BeginDialog()
{
	bInDialog = true;

	ULevelAdvancedFriendsGameInstance* GameInstance = Cast<ULevelAdvancedFriendsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CurrentLevel = GameInstance->CurrentLevel;

	if (!LevelDialog.Contains(CurrentLevel)) 
 	{
		UE_LOG(LogTemp, Warning, TEXT("No dialog for level"));
		return;
 	}

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	StartingMaxWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
	CharacterMovementComponent->MaxWalkSpeed = 0.0f;

	LerpCameraToJester();

	Interact();
}

void UDialogSystemComponent::Interact()
{
	if (!bInDialog) return;

	if (!LevelDialog.Contains(CurrentLevel))
	{
		UE_LOG(LogTemp, Warning, TEXT("No dialog for level"));
		return;
	}

	if (CurrentLineIndex == LevelDialog[CurrentLevel].Lines.Num()) {
		OnEndDialog.Broadcast();

		UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
		CharacterMovementComponent->MaxWalkSpeed = StartingMaxWalkSpeed;
		GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimerHandle);

		CameraComponent = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
		CameraComponent->bUsePawnControlRotation = true;

		return;
	}
	
	FString NewText = LevelDialog[CurrentLevel].Lines[CurrentLineIndex];
	OnChangeDialogText.Broadcast(NewText);
	CurrentLineIndex++;
}

void UDialogSystemComponent::LerpCameraToJester()
{
	JesterActor = UGameplayStatics::GetActorOfClass(GetWorld(), AJesterActor::StaticClass());
	if (!JesterActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("JesterActor is not set"));
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(CameraLerpTimerHandle, this, &UDialogSystemComponent::UpdateCameraLerp, 0.01f, true);
}

void UDialogSystemComponent::UpdateCameraLerp()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	CameraComponent = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
	CameraComponent->bUsePawnControlRotation = false;
	if (!CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent is not set"));
		return;
	}

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraComponent->GetComponentLocation(), JesterActor->GetActorLocation());
	FRotator NewRotation = FMath::RInterpTo(CameraComponent->GetComponentRotation(), TargetRotation, DeltaTime, 1.0f);
	CameraComponent->SetWorldRotation(NewRotation);
}


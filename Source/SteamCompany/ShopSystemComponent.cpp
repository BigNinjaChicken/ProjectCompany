// Simplified and optimized includes, removing duplicates and unnecessary paths.
#include "ShopSystemComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "LevelAdvancedFriendsGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JesterActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShopActor.h"
#include "Net/UnrealNetwork.h"

UShopSystemComponent::UShopSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShopSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	PlayerController = Character ? Cast<APlayerController>(Character->GetController()) : nullptr;
	if (!Character || !PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), !Character ? TEXT("Owner is not ACharacter") : TEXT("PlayerController null"));
		return;
	}

	ServerGetStartingSpeed();
}

void UShopSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShopSystemComponent::EndShop()
{
	OnEndDialog.Broadcast();
	MulticastResetMovementSpeed();
	ResetInputMode();
	GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimerHandle);
	if (!CameraComponent) // Check added for safety, although it should be initialized in BeginPlay
		CameraComponent = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));

	CameraComponent->bUsePawnControlRotation = true;
	Deactivate();
}

void UShopSystemComponent::MulticastResetMovementSpeed_Implementation()
{
	if (Character)
	{
		UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->MaxWalkSpeed = StartingMaxWalkSpeed;
		}
	}
	ResetInputMode();
}

void UShopSystemComponent::ServerGetStartingSpeed_Implementation()
{
	if (Character)
	{
		UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
		StartingMaxWalkSpeed = CharacterMovementComponent ? CharacterMovementComponent->MaxWalkSpeed : 0;
	}
}

void UShopSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UShopSystemComponent::Interact()
{
	if (bInShop) return;
	bInShop = true;

	if (ItemOptions.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No ItemOptions"));
		return;
	}

	MulticastSetInteractSettings();
	LerpCameraToJester();

	PresentItemOptions();
}

void UShopSystemComponent::MulticastSetInteractSettings_Implementation()
{
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}

	if (Character)
	{
		UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			StartingMaxWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
			CharacterMovementComponent->MaxWalkSpeed = 0.0f;
		}
	}
}

void UShopSystemComponent::LerpCameraToJester() 
{
	ShopKeeperActor = UGameplayStatics::GetActorOfClass(GetWorld(), AShopActor::StaticClass());
	if (!ShopKeeperActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopKeeperActor is not set"));
		return;
	}

	if (!CameraComponent)
	{
		CameraComponent = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
		CameraComponent->bUsePawnControlRotation = false;
	}

	GetWorld()->GetTimerManager().SetTimer(CameraLerpTimerHandle, this, &UShopSystemComponent::UpdateCameraLerp, 0.01f, true);
}

void UShopSystemComponent::UpdateCameraLerp()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent is not set"));
		return;
	}

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraComponent->GetComponentLocation(), ShopKeeperActor->GetActorLocation());
	CameraComponent->SetWorldRotation(FMath::RInterpTo(CameraComponent->GetComponentRotation(), TargetRotation, DeltaTime, 1.0f));
}

void UShopSystemComponent::ResetInputMode()
{
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}
}

void UShopSystemComponent::PresentItemOptions()
{
	TArray<TSubclassOf<UItemEffectComponent>> ItemsToPickFrom;
	for (int i = 0; i < 3; ++i) // Simplified random selection.
	{
		ItemsToPickFrom.AddUnique(ItemOptions[FMath::RandRange(0, ItemOptions.Num() - 1)]);
	}
	OnShopChange.Broadcast(ItemsToPickFrom);
}


#include "PlayerInteractComponent.h"
#include "Engine/HitResult.h"
#include "GameFramework/PlayerController.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

UPlayerInteractComponent::UPlayerInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// Attempt to find Owner Character
	Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		// Handle the case where no CombatComponent is found
		UE_LOG(LogTemp, Warning, TEXT("Owner is not ACharacter"));
		return;
	}

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(InteractMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &UPlayerInteractComponent::Interact);
		}
	}
	
}

void UPlayerInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerInteractComponent::Interact()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (PlayerController)
    {
        FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
        FVector ForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
        FVector End = Start + (ForwardVector * RaycastDistance);

        TArray<FHitResult> Hits;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(GetOwner());

        if (GetWorld()->LineTraceMultiByChannel(Hits, Start, End, ECC_Visibility, CollisionParams))
        {
            for (const FHitResult& Hit : Hits)
            {
                if (AActor* HitActor = Hit.GetActor())
                {
                    // Broadcast delegate for each hit
                    OnInteract.Broadcast(Hit);
                }
            }
        }
    }
}


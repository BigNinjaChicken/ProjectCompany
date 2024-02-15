#include "DashAbilityComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/TimerHandle.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"

UDashAbilityComponent::UDashAbilityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UDashAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    // Attempt to find the CombatComponent on the same actor
    CombatComp = GetOwner()->FindComponentByClass<UCombatComponent>();
    if (!CombatComp)
    {
        // Handle the case where no CombatComponent is found
        UE_LOG(LogTemp, Warning, TEXT("DashAbilityComponent requires a CombatComponent on the same actor!"));
        return;
    }

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
            Subsystem->AddMappingContext(DashMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            // Fire
            EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &UDashAbilityComponent::Dash);
        }
    }
}

void UDashAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDashAbilityComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UDashAbilityComponent, OnComplete);
    DOREPLIFETIME(UDashAbilityComponent, OnCooldownBegin);
    DOREPLIFETIME(UDashAbilityComponent, bOnCooldown);
}

void UDashAbilityComponent::Dash()
{
    if (bOnCooldown) {
        return;
    }

    if (!Character)
    {
        return;
    }

    OnCooldownBegin.Broadcast(BiteCooldown);
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UDashAbilityComponent::CooldownComplete, BiteCooldown, false);

    Dash_Server();
}

void UDashAbilityComponent::CooldownComplete()
{
    bOnCooldown = false;
}

void UDashAbilityComponent::Dash_Server_Implementation()
{
    if (bOnCooldown) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Dash_Server_Implementation: Early exit due to ability on cooldown."));
        return;
    }

    bOnCooldown = true;
    Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dash_Server_Implementation: Early exit due to Character being null"));
        return;
    }

    FVector StartPosition = Character->GetActorLocation();
    FVector DashDirection;
    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dash_Server_Implementation: Failed to cast character controller to APlayerController."));
        return;
    }

    UPawnMovementComponent* MovementComponent = Character->GetMovementComponent();
    if (!MovementComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dash_Server_Implementation: MovementComponent is null."));
        return;
    }
    FVector CurrentVelocity = MovementComponent->Velocity;
    CurrentVelocity.Z = 0; // Ensure dash is only horizontal
    DashDirection = CurrentVelocity.IsNearlyZero() ? Character->GetActorForwardVector().GetSafeNormal() : CurrentVelocity.GetSafeNormal(); // Default direction if no velocity

    FVector EndPosition = StartPosition + DashDirection * DashDistance;
    FHitResult HitResult;

    // Perform the raycast to check for obstacles
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPosition, EndPosition, ECC_Visibility);
    UE_LOG(LogTemp, Log, TEXT("Dash_Server_Implementation: Raycast %s, Distance: %f"), bHit ? TEXT("hit") : TEXT("missed"), bHit ? HitResult.Distance : 0.0f);
    if (bHit && HitResult.Distance > 0)
    {
        EndPosition = StartPosition + DashDirection * HitResult.Distance;
    }

    // Calculate the time it should take to complete the dash
    float DashTime = DashDistance / DashSpeed;
    if (bHit)
    {
        DashTime = HitResult.Distance / DashSpeed;
    }

    CombatComp->SetInvincible(InvincibilityTime);
    bOnCooldown = true;
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UDashAbilityComponent::CooldownComplete, BiteCooldown, false); // Assume CooldownDuration is a corrected variable name

    // Adjust the dash implementation to track elapsed time
    float ElapsedTime = 0.0f; // Initialize elapsed time
    GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, [this, StartPosition, EndPosition, DashTime, ElapsedTime]() mutable {
        if (ElapsedTime < DashTime)
        {
            // Calculate the current position using the elapsed time
            float Alpha = ElapsedTime / DashTime;
            FVector NewPosition = FMath::Lerp(StartPosition, EndPosition, Alpha);
            Character->SetActorLocation(NewPosition);

            ElapsedTime += 0.01; // Increment elapsed time by the tick interval
        }
        else
        {
            // Stop the timer once the dash duration has elapsed
            GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
        }
        }, 0.01, true);

    UE_LOG(LogTemp, Log, TEXT("Dash_Server_Implementation: Dash initiated from %s to %s over %f seconds."), *StartPosition.ToString(), *EndPosition.ToString(), DashTime);
}
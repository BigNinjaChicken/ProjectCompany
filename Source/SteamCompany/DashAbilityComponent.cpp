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
    bOnCooldown = true;
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
    if (!Character)
    {
        return;
    }

    UPawnMovementComponent* MovementComponent = Character->GetMovementComponent();
    APawn* PlayerPawn = Cast<APawn>(Character);

    if (!PlayerPawn || !MovementComponent)
    {
        return;
    }

    FRotator ControlRotation = PlayerPawn->GetControlRotation();
    FVector DashDirection = ControlRotation.Vector();
    DashDirection.Normalize();
    MovementComponent->Velocity += DashDirection * DashStrength;

}

// Fill out your copyright notice in the Description page of Project Settings.

#include "HealItemComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/TimerHandle.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "CombatComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

void UHealItemComponent::BeginPlay()
{
    Super::BeginPlay();

    // Attempt to find the CombatComponent on the same actor
    CombatComp = GetOwner()->FindComponentByClass<UCombatComponent>();
    if (!CombatComp)
    {
        // Handle the case where no CombatComponent is found
        UE_LOG(LogTemp, Warning, TEXT("Requires a CombatComponent on the same actor!"));
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
            Subsystem->AddMappingContext(HealMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            // Fire
            EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Triggered, this, &UHealItemComponent::Heal);
        }
    }
}

// Called every frame
void UHealItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealItemComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealItemComponent, OnHealItemComplete);
    DOREPLIFETIME(UHealItemComponent, OnCooldownBegin);
}

void UHealItemComponent::Heal()
{
    if (bOnCooldown) {
        return;
    }

    if (!Character)
    {
        return;
    }

    UCombatComponent* PlayerCombatComponent = Cast<UCombatComponent>(Character->GetComponentByClass(UCombatComponent::StaticClass()));
    ServerRequestHeal(PlayerCombatComponent, HealAmount);

    OnCooldownBegin.Broadcast(CooldownTime);
    bOnCooldown = true;
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UHealItemComponent::CooldownComplete, CooldownTime, false);
    ItemAbilityUserWidget->StartCooldown(CooldownTime);
}

void UHealItemComponent::CooldownComplete()
{
    bOnCooldown = false;
}

void UHealItemComponent::ServerRequestHeal_Implementation(UCombatComponent* PlayerCombatComp, float HealthIncrease)
{
    if (CombatComp && CombatComp->GetOwner()->HasAuthority())
    {
        CombatComp->ServerHeal(HealthIncrease);
    }
}
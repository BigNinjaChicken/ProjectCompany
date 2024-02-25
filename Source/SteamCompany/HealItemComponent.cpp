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
            EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Triggered, this, &UHealItemComponent::Attack);
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

    DOREPLIFETIME(UHealItemComponent, OnDamageComplete);
    DOREPLIFETIME(UHealItemComponent, OnCooldownBegin);
}

void UHealItemComponent::Attack()
{
    if (bOnCooldown) {
        return;
    }

    if (!Character)
    {
        return;
    }

    Character->GetCharacterMovement()->AddImpulse(FVector::UpVector * JumpHeight);
    GetWorld()->GetTimerManager().SetTimer(GroundedTimerHandle, this, &UHealItemComponent::CheckGrounded, 0.1f, true);

    OnCooldownBegin.Broadcast(CooldownTime);
    bOnCooldown = true;
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UHealItemComponent::CooldownComplete, CooldownTime, false);
    ItemAbilityUserWidget->StartCooldown(CooldownTime);
}

void UHealItemComponent::CheckGrounded()
{
    if (Character->GetMovementComponent()->IsFalling()) {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Here"));

    GetWorld()->GetTimerManager().ClearTimer(GroundedTimerHandle);

    FVector BiteLocation = Character->GetActorLocation();
    float BiteRadius = 800.0f;

    TArray<AActor*> OverlappedActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    //     ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Overlap with other pawns

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Character); // Ignore self

    bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(
        this,
        BiteLocation,
        BiteRadius,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        OverlappedActors
    );

    /*Optional: Draw Debug Sphere for visual feedback*/
#if WITH_EDITOR
    DrawDebugSphere(GetWorld(), BiteLocation, BiteRadius, 32, FColor::Red, false, 2.0f);
#endif

    UCombatComponent* OtherCombatComp = nullptr;
    if (bOverlap)
    {
        for (AActor* OtherActor : OverlappedActors)
        {
            OtherCombatComp = Cast<UCombatComponent>(OtherActor->GetComponentByClass(UCombatComponent::StaticClass()));
            if (OtherCombatComp) {
                ServerRequestDamage(OtherCombatComp, Damage * CombatComp->StrengthMultiplier);
                CombatComp->EnterCombat();
                OnDamageComplete.Broadcast(OtherCombatComp);
            }
        }
    }
}

void UHealItemComponent::CooldownComplete()
{
    bOnCooldown = false;
}

void UHealItemComponent::ServerRequestDamage_Implementation(UCombatComponent* OtherCombatComp, float DamageAmount)
{
    if (OtherCombatComp && OtherCombatComp->GetOwner()->HasAuthority())
    {
        OtherCombatComp->ServerTakeDamage(DamageAmount);
    }
}
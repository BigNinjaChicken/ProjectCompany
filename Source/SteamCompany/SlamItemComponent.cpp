// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/TimerHandle.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "CombatComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "SlamItemComponent.h"

void USlamItemComponent::BeginPlay()
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
            Subsystem->AddMappingContext(SlamMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            // Fire
            EnhancedInputComponent->BindAction(SlamAction, ETriggerEvent::Triggered, this, &USlamItemComponent::Attack);
        }
    }
}

// Called every frame
void USlamItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USlamItemComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USlamItemComponent, OnDamageComplete);
    DOREPLIFETIME(USlamItemComponent, OnCooldownBegin);
}

void USlamItemComponent::Attack()
{
    if (bOnCooldown) {
        return;
    }

    if (!Character)
    {
        return;
    }

    Character->GetCharacterMovement()->AddImpulse(FVector::UpVector * JumpHeight);
    GetWorld()->GetTimerManager().SetTimer(GroundedTimerHandle, this, &USlamItemComponent::CheckGrounded, 0.1f, true);

    OnCooldownBegin.Broadcast(CooldownTime);
    bOnCooldown = true;
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &USlamItemComponent::CooldownComplete, CooldownTime, false);
    ItemAbilityUserWidget->StartCooldown(CooldownTime);
}

void USlamItemComponent::CheckGrounded()
{
    if (Character->GetCharacterMovement()->IsFalling()) {
        return;
    }

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

void USlamItemComponent::CooldownComplete()
{
    bOnCooldown = false;
}

void USlamItemComponent::ServerRequestDamage_Implementation(UCombatComponent* OtherCombatComp, float DamageAmount)
{
    if (OtherCombatComp && OtherCombatComp->GetOwner()->HasAuthority())
    {
        OtherCombatComp->ServerTakeDamage(DamageAmount);
    }
}
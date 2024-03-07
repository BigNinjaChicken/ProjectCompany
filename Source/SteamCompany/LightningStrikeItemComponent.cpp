// Fill out your copyright notice in the Description page of Project Settings.

#include "LightningStrikeItemComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/TimerHandle.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "CombatComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LightningButtonActor.h"

void ULightningStrikeItemComponent::BeginPlay()
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
            Subsystem->AddMappingContext(LightningMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            // Fire
            EnhancedInputComponent->BindAction(LightningAction, ETriggerEvent::Triggered, this, &ULightningStrikeItemComponent::Lightning);
        }
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this->GetOwner();
    FVector Location = FVector(0, 0, 0);
    FRotator Rotation = FRotator(0, 0, 0);
    LightningButtonActor = GetWorld()->SpawnActor<ALightningButtonActor>(LightningButtonActorClass, Location, Rotation, SpawnParams);

    GetWorld()->GetTimerManager().SetTimer(LightningTimerHandle, this, &ULightningStrikeItemComponent::CheckLightning, 1.0f, true);
}

// Called every frame
void ULightningStrikeItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULightningStrikeItemComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ULightningStrikeItemComponent, OnLightningItemComplete);
    DOREPLIFETIME(ULightningStrikeItemComponent, OnCooldownBegin);
}

void ULightningStrikeItemComponent::Lightning()
{
    if (bOnCooldown) {
        return;
    }

    if (!ClosestCharacter) {
        UE_LOG(LogTemp, Log, TEXT("Can't cast unless there is something to hit"));
        return;
    }

    if (!LightningButtonActor) {
        UE_LOG(LogTemp, Log, TEXT("LightningButtonActor Null"));
        return;
    }

    if (!bCanAttackEnemy) {
        UE_LOG(LogTemp, Log, TEXT("No enemy to attack"));
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(LightningTimerHandle);
    LightningButtonActor->SetActorHiddenInGame(true);
    UCombatComponent* ClosestCharacterCombatComponent = ClosestCharacter->GetComponentByClass<UCombatComponent>();
    ServerRequestLightning(ClosestCharacterCombatComponent, LightningDamage);
    OnLightningItemComplete.Broadcast(ClosestCharacterCombatComponent);

    OnCooldownBegin.Broadcast(CooldownTime);
    bOnCooldown = true;
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ULightningStrikeItemComponent::CooldownComplete, CooldownTime, false);
    ItemAbilityUserWidget->StartCooldown(CooldownTime);
}

void ULightningStrikeItemComponent::CooldownComplete()
{
    GetWorld()->GetTimerManager().SetTimer(LightningTimerHandle, this, &ULightningStrikeItemComponent::CheckLightning, 1.0f, true);
    bOnCooldown = false;
}

void ULightningStrikeItemComponent::ServerRequestLightning_Implementation(UCombatComponent* PlayerCombatComp, float Damage)
{
    if (PlayerCombatComp && PlayerCombatComp->GetOwner()->HasAuthority())
    {
        PlayerCombatComp->ServerTakeDamage(LightningDamage);
    }
}

void ULightningStrikeItemComponent::CheckLightning()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
    FVector PlayerLocation = CameraManager->GetCameraLocation();
    FVector2D ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
    FVector2D ScreenCenter = ScreenSize / 2;
    FVector2D InnerScreenMin = ScreenCenter - ScreenCenter / ScreenBounds;
    FVector2D InnerScreenMax = ScreenCenter + ScreenCenter / ScreenBounds;

    ClosestCharacter = nullptr;
    float MinDistanceToCenter = MAX_FLT;

    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OutActors);

    for (AActor* Actor : OutActors)
    {
        ACharacter* CharacterInLevel = Cast<ACharacter>(Actor);
        if (!CharacterInLevel) continue;

        UCombatComponent* EnemyCombatComponent = CharacterInLevel->GetComponentByClass<UCombatComponent>();
        if (EnemyCombatComponent) {
            if (EnemyCombatComponent->bIsPlayer) continue;
            if (EnemyCombatComponent->Health <= 0) continue;
        }

        FVector CharacterLocation = CharacterInLevel->GetActorLocation();
        FVector2D ScreenPosition;
        PlayerController->ProjectWorldLocationToScreen(CharacterLocation, ScreenPosition);

        // Check if the character is within the inner part of screen
        if (ScreenPosition.X >= InnerScreenMin.X && ScreenPosition.X <= InnerScreenMax.X &&
            ScreenPosition.Y >= InnerScreenMin.Y && ScreenPosition.Y <= InnerScreenMax.Y)
        {
            if (FVector::Distance(PlayerLocation, CharacterLocation) <= MinDistanceFromPlayer)
            {
                FHitResult HitResult;
                FCollisionQueryParams CollisionParams;
                CollisionParams.AddIgnoredActor(PlayerController->GetPawn());

                // Line trace to check if there is a wall between the player and the character
                if (!GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLocation, CharacterLocation, ECC_Visibility, CollisionParams))
                {
                    float DistanceToCenter = FVector2D::Distance(ScreenPosition, ScreenCenter);
                    if (DistanceToCenter < MinDistanceToCenter)
                    {
                        MinDistanceToCenter = DistanceToCenter;
                        ClosestCharacter = CharacterInLevel;
                    }
                }
            }
        }
    }

    if (LightningButtonActor) {
        if (ClosestCharacter) {
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
            LightningButtonActor->AttachToActor(ClosestCharacter, AttachmentRules);
            LightningButtonActor->SetActorHiddenInGame(false);
            bCanAttackEnemy = true;
        }
        else
        {
            LightningButtonActor->SetActorHiddenInGame(true);
            bCanAttackEnemy = false;
        }
    }
}

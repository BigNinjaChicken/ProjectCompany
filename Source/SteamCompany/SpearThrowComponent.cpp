
#include "SpearThrowComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/TimerHandle.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "CombatComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "SpearProjectileActor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

USpearThrowComponent::USpearThrowComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USpearThrowComponent::BeginPlay()
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
            Subsystem->AddMappingContext(VampireMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            // Fire
            EnhancedInputComponent->BindAction(BiteAction, ETriggerEvent::Triggered, this, &USpearThrowComponent::Attack);
        }
    }
}


// Called every frame
void USpearThrowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpearThrowComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USpearThrowComponent, OnSpearThrown);
    DOREPLIFETIME(USpearThrowComponent, OnCooldownBegin);
    DOREPLIFETIME(USpearThrowComponent, bIsThrowing);
}

void USpearThrowComponent::Attack()
{
    if (bOnCooldown) {
        UE_LOG(LogTemp, Warning, TEXT("Attack attempted during cooldown."));
        return;
    }

    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Character"));
        return;
    }

    // Log cooldown time
    UE_LOG(LogTemp, Log, TEXT("Cooldown time: %f"), CooldownTime);
    OnCooldownBegin.Broadcast(CooldownTime);
    bOnCooldown = true;
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &USpearThrowComponent::CooldownComplete, CooldownTime, false);

    SpawnSpear();
    return;
}


void USpearThrowComponent::SpawnSpear_Implementation()
{
	if (!SpearProjectileActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No SpearProjectileActor"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Character;
	SpawnParams.bNoFail = true;

	FVector SpawnLocation = Character->GetActorLocation();
	FRotator SpawnRotation = Character->GetActorRotation();

	// Log spawn location and rotation
	UE_LOG(LogTemp, Log, TEXT("Spawn Location: %s, Spawn Rotation: %s"), *SpawnLocation.ToString(), *SpawnRotation.ToString());

	SpawnedProjectile = GetWorld()->SpawnActor<ASpearProjectileActor>(SpearProjectileActor, SpawnLocation + SpawnOffset, SpawnRotation, SpawnParams);
	if (!SpawnedProjectile) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn projectile"));
		return;
	}

	// Confirm projectile spawning
	UE_LOG(LogTemp, Log, TEXT("Projectile spawned successfully"));

	OnSpearThrown.Broadcast(SpawnedProjectile);

	EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
	EAttachmentRule RotationRule = EAttachmentRule::KeepRelative;
	EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;
	FAttachmentTransformRules AttachmentTransformRules(LocationRule, RotationRule, ScaleRule, false);

	// Checking if Character is still valid before attaching
	if (Character) {
		SpawnedProjectile->AttachToComponent(Character->GetMesh(), AttachmentTransformRules, AttachName);
		// Log attachment
		UE_LOG(LogTemp, Log, TEXT("Projectile attached to %s"), *AttachName.ToString());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Character invalid when attempting to attach projectile"));
	}

	bIsThrowing = true;
	GetWorld()->GetTimerManager().SetTimer(ProjectileMovementTimerHandle, this, &USpearThrowComponent::StartProjectileMovement, SpearDelay, false);
}

void USpearThrowComponent::CooldownComplete()
{
    bOnCooldown = false;
}

void USpearThrowComponent::ServerRequestDamage_Implementation(UCombatComponent* OtherCombatComp, float DamageAmount)
{
    if (OtherCombatComp && OtherCombatComp->GetOwner()->HasAuthority())
    {
        OtherCombatComp->ServerTakeDamage(DamageAmount);
    }
}

void USpearThrowComponent::StartProjectileMovement()
{
    if (SpawnedProjectile)
    {
        UProjectileMovementComponent* ProjectileMovementComponent = SpawnedProjectile->ProjectileMovementComponent;
        if (ProjectileMovementComponent)
        {
            bIsThrowing = false;

            EDetachmentRule LocationRule = EDetachmentRule::KeepWorld;
            EDetachmentRule RotationRule = EDetachmentRule::KeepWorld;
            EDetachmentRule ScaleRule = EDetachmentRule::KeepWorld;
            FDetachmentTransformRules DetachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
            SpawnedProjectile->DetachFromActor(DetachmentTransformRules);
            SpawnedProjectile->AddActorWorldOffset(SpawnOffset, false);

            ProjectileMovementComponent->Activate();
            FVector Forward = Character->GetActorForwardVector();
            Forward *= 1000.0f;
            Forward.Z += 80.0f;
            ProjectileMovementComponent->Velocity = Forward;
            ProjectileMovementComponent->ProjectileGravityScale = 0.05f;
        }
    }
}

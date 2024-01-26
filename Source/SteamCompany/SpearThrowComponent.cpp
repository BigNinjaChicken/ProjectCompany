
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

    DOREPLIFETIME(USpearThrowComponent, OnDamageComplete);
    DOREPLIFETIME(USpearThrowComponent, OnCooldownBegin);
}

void USpearThrowComponent::Attack()
{
    if (bOnCooldown) {
        return;
    }

    if (!Character)
    {
        return;
    }

    OnCooldownBegin.Broadcast(CooldownTime);
    bOnCooldown = true;
    FTimerHandle CooldownTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &USpearThrowComponent::CooldownComplete, CooldownTime, false);

    if (!SpearProjectileActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("No SpearProjectileActor"));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = Character;

    FVector SpawnLocation = Character->GetActorLocation();
    FRotator SpawnRotation = Character->GetActorRotation();

    SpawnedProjectile = GetWorld()->SpawnActor<ASpearProjectileActor>(SpearProjectileActor, SpawnLocation, SpawnRotation, SpawnParams);

    GetWorld()->GetTimerManager().SetTimer(ProjectileMovementTimerHandle, this, &USpearThrowComponent::StartProjectileMovement, 1.0, false);
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
    UProjectileMovementComponent* ProjectileMovementComponent = SpawnedProjectile->ProjectileMovementComponent;
    if (SpawnedProjectile && ProjectileMovementComponent)
    {
        ProjectileMovementComponent->Activate();
    }
}

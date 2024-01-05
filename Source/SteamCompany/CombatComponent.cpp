#include "CombatComponent.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Set this component to be replicated
    SetIsReplicatedByDefault(true);
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize Health
    Health = MaxHealth;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::ServerTakeDamage_Implementation(float DamageAmount)
{
    if (GetOwner()->HasAuthority()) // Ensure this runs only on the server
    {
        Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
        if (Health <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor Dead: %s"), *GetOwner()->GetName());
            OnActorDead.Broadcast();
        }
        UE_LOG(LogTemp, Warning, TEXT("Damaged Actor: %s, Damage Applied: %f"), *GetOwner()->GetName(), DamageAmount);
        OnTakeDamageComplete.Broadcast();
    }
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCombatComponent, Health);
}

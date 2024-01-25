#include "CombatComponent.h"
#include "Engine/World.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (Character) {
        APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
        if (PlayerController) {
            FInputModeGameOnly InputModeGameOnly;
            PlayerController->SetInputMode(InputModeGameOnly);
        }
    }
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::ServerTakeDamage_Implementation(float DamageAmount)
{
    if (!GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("!GetOwner()->HasAuthority()"));
        return;
    }

    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    if (Health <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor Dead: %s"), *GetOwner()->GetName());
        OnActorDead.Broadcast();
    }
    else
    {
        EnterCombat();
    }
    UE_LOG(LogTemp, Warning, TEXT("Damaged Actor: %s, Damage Applied: %f"), *GetOwner()->GetName(), DamageAmount);
    OnTakeDamageComplete.Broadcast();
}

void UCombatComponent::ServerHeal_Implementation(float HealAmount)
{
    if (!GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("!GetOwner()->HasAuthority()"));
        return;
    }

    Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Healed Actor: %s, Healing Applied: %f"), *GetOwner()->GetName(), FMath::Clamp(HealAmount, 0.0f, 100000.0f));
    OnHealComplete.Broadcast();
}

void UCombatComponent::EnterCombat()
{
    if (!bInCombat)
    {
        bInCombat = true;
        OnEnterCombat.Broadcast();
    }
    GetWorld()->GetTimerManager().ClearTimer(CombatTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(CombatTimerHandle, this, &UCombatComponent::EndCombat, CombatDuration, false);
}

void UCombatComponent::EndCombat()
{
    if (bInCombat)
    {
        bInCombat = false;
        OnOutOfCombat.Broadcast();
    }
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCombatComponent, Health);
}

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamageComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealComplete);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STEAMCOMPANY_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UCombatComponent();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsFriendly = false;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTakeDamageComplete OnTakeDamageComplete;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnActorDead OnActorDead;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Combat")
    FOnEnterCombat OnEnterCombat;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Combat")
    FOnOutOfCombat OnOutOfCombat;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Combat")
    FOnHealComplete OnHealComplete;

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerTakeDamage(float DamageAmount);

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerHeal(float DamageAmount);

    float StrengthMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bInCombat = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsPlayer = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float CombatDuration = 3.0f;
    FTimerHandle CombatTimerHandle;
    void EnterCombat();
    void EndCombat();
};


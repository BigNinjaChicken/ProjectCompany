// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveItemEffectComponent.h"
#include "BlastItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlastDamageComplete, UCombatComponent*, OtherCombatComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlastCooldownBegin, float, BiteCooldown);

class UInputMappingContext;
class UInputAction;
class ACharacter;
class UCombatComponent;

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UBlastItemComponent : public UActiveItemEffectComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UBlastItemComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Reference to the CombatComponent
    UPROPERTY()
        UCombatComponent* CombatComp;

    // Reference to Owner
    UPROPERTY()
        ACharacter* Character;

    /** MappingContext */
    UPROPERTY(EditAnywhere, Category = "Input")
        UInputMappingContext* BlastMappingContext;

    /** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
        UInputAction* BlastAction;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
        void Attack();

    UFUNCTION()
        void CooldownComplete();

    UFUNCTION(Server, Reliable)
        void ServerRequestDamage(UCombatComponent* OtherCombatComp, float DamageAmount);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        float CooldownTime = 2.0f;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
        FOnBlastDamageComplete OnDamageComplete;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
        FOnBlastCooldownBegin OnCooldownBegin;

    bool bOnCooldown = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        float Damage = 10.0f;
};

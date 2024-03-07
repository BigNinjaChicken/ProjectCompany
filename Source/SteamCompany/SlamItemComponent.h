// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveItemEffectComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "SlamItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlamDamageComplete, UCombatComponent*, OtherCombatComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlamComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlamCooldownBegin, float, BiteCooldown);

class UInputMappingContext;
class UInputAction;
class ACharacter;
class UCombatComponent;

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API USlamItemComponent : public UActiveItemEffectComponent
{
    GENERATED_BODY()
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
        UInputMappingContext* SlamMappingContext;

    /** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
        UInputAction* SlamAction;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
        void Attack();

    UFUNCTION()
        void CheckGrounded();

    UFUNCTION()
        void CooldownComplete();

    UFUNCTION(Server, Reliable)
        void ServerRequestDamage(UCombatComponent* OtherCombatComp, float DamageAmount);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        float CooldownTime = 2.0f;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
        FOnSlamDamageComplete OnDamageComplete;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
        FOnSlamCooldownBegin OnCooldownBegin;

     UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
    FOnSlamComplete OnSlamComplete;

    UPROPERTY()
        FTimerHandle GroundedTimerHandle;

    bool bOnCooldown = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        float Damage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        float JumpHeight = 100.0f;
};

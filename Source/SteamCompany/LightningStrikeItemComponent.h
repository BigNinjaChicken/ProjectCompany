// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveItemEffectComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LightningButtonActor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/TimerHandle.h"

#include "LightningStrikeItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightningItemComplete, UCombatComponent*, OtherCombatComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightningItemCooldownBegin, float, BiteCooldown);

class UInputMappingContext;
class UInputAction;
class ACharacter;
class UCombatComponent;

UCLASS()
class STEAMCOMPANY_API ULightningStrikeItemComponent : public UActiveItemEffectComponent
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
        UInputMappingContext* LightningMappingContext;

    /** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
        UInputAction* LightningAction;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
        void Lightning();

    UFUNCTION()
        void CooldownComplete();

    UFUNCTION(Server, Reliable)
        void ServerRequestLightning(UCombatComponent* PlayerCombatComp, float Damage);

    UFUNCTION()
        void CheckLightning();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        float CooldownTime = 2.0f;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
        FOnLightningItemComplete OnLightningItemComplete;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
        FOnLightningItemCooldownBegin OnCooldownBegin;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        FTimerHandle LightningTimerHandle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        bool bOnCooldown = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        float LightningDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning")
        TSubclassOf<ALightningButtonActor> LightningButtonActorClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        ALightningButtonActor* LightningButtonActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        ACharacter* ClosestCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        bool bCanAttackEnemy = false;

    // If the Screen Bounds is 3 it will only put the lightning symbol on enemies within the inner 1/3 of the players screen.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning")
        float ScreenBounds = 3;

    // How close the enemy must be to be lightning stiked
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning")
        float MinDistanceFromPlayer = 3000.0f;
};

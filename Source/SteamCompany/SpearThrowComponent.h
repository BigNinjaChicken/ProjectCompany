
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "CombatComponent.h"
#include "SpearProjectileActor.h"

#include "SpearThrowComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpearThrown, ASpearProjectileActor*, Spear);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpearCooldownBegin, float, BiteCooldown);

class UInputMappingContext;
class UInputAction;
class ACharacter;
class UCombatComponent;
class ASpearProjectileActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API USpearThrowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpearThrowComponent();

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
	UInputMappingContext* VampireMappingContext;

	/** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* BiteAction;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Replication setup
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable)
	void Attack();

    void OnArrowTipOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float CooldownTime = 2.0f;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
    FOnSpearThrown OnSpearThrown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float SpearDelay = 0.54f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    FVector SpawnOffset = FVector(0, 0, 0);

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
    FOnSpearCooldownBegin OnCooldownBegin;

    void CooldownComplete();

    bool bOnCooldown = false;

    UFUNCTION(Server, Reliable)
    void ServerRequestDamage(UCombatComponent* OtherCombatComp, float DamageAmount);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float Damage = 10.0f;

    // Timer handle for delayed projectile movement
    FTimerHandle ProjectileMovementTimerHandle;

    UFUNCTION(BlueprintCallable)
    void StartProjectileMovement();

    ASpearProjectileActor* SpawnedProjectile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TSubclassOf<ASpearProjectileActor> SpearProjectileActor;

  
};

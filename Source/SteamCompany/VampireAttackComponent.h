#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.h" 
#include "Net/UnrealNetwork.h"
#include "VampireAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBiteDamageComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBiteCooldownBegin, float, BiteCooldown);

class UInputMappingContext;
class UInputAction;

UCLASS(config=Game, BlueprintType, meta=(ShortTooltip="Vampire Attack Component."))
class STEAMCOMPANY_API UVampireAttackComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UVampireAttackComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Reference to the CombatComponent
    UPROPERTY()
    UCombatComponent* CombatComp;
    
    // Reference to Owner
    UPROPERTY()
    ACharacter* Character;

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Replication setup
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    /** MappingContext */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* VampireMappingContext;

	/** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* BiteAction;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Bite();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float BiteCooldown = 2.0f;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBiteDamageComplete OnBiteDamageComplete;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBiteCooldownBegin OnBiteCooldownBegin;

    UFUNCTION(Server, Reliable)
    void ServerRequestDamage(UCombatComponent* OtherCombatComp, float DamageAmount);
};

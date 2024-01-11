#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.h" 
#include "Net/UnrealNetwork.h"
#include "VampireAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBiteDamageComplete, UCombatComponent*, OtherCombatComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBiteCooldownBegin, float, BiteCooldown);

class UInputMappingContext;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
    FOnBiteDamageComplete OnBiteDamageComplete;

    UPROPERTY(Replicated)
    UCombatComponent* OtherCombatComponent;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
    FOnBiteCooldownBegin OnBiteCooldownBegin;

    void CooldownComplete();

    bool bOnCooldown = false;

    UFUNCTION(Server, Reliable)
    void ServerRequestDamage(UCombatComponent* OtherCombatComp, float DamageAmount);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float Damage = 10.0f;

    UFUNCTION()
    void OnRep_BiteDamageComplete() {}
};

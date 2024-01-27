#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.h" 
#include "Net/UnrealNetwork.h"
#include "DashAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashCooldownBegin, float, DashCooldown);

class UInputMappingContext;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UDashAbilityComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UDashAbilityComponent();

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
	UInputMappingContext* DashMappingContext;

	/** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Replication setup
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Dash();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float BiteCooldown = 2.0f;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
    FOnDashComplete OnComplete;

    UPROPERTY(BlueprintAssignable, Replicated, Category = "Events")
    FOnDashCooldownBegin OnCooldownBegin;

    void CooldownComplete();

    bool bOnCooldown = false;

    UFUNCTION(Server, Reliable)
    void Dash_Server();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float DashStrength = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float InvincibilityTime = 1.0f;

    UFUNCTION()
    void OnRep_BiteDamageComplete() {}
};

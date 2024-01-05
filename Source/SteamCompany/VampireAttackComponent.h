#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.h" 
#include "VampireAttackComponent.generated.h"

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

    /** MappingContext */
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputMappingContext* VampireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* BiteAction;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Bite();

    UFUNCTION(Server, Reliable)
    void ServerRequestDamage(UCombatComponent* OtherCombatComp, float DamageAmount);
};

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamageComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UCombatComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Replication setup
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Health Properties
    UPROPERTY(BlueprintReadOnly, Category = "Health", Replicated)
    float Health;
    UPROPERTY(EditAnywhere, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    bool bIsFriendly = false;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTakeDamageComplete OnTakeDamageComplete;

    // Delegate for damage events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnActorDead OnActorDead;

    // Function to handle damage
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerTakeDamage(float DamageAmount);
};

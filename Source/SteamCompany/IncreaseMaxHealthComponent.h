
#pragma once

#include "CoreMinimal.h"
#include "ItemEffectComponent.h"
#include "CombatComponent.h"
#include "IncreaseMaxHealthComponent.generated.h"

class UCombatComponent;

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UIncreaseMaxHealthComponent : public UItemEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIncreaseMaxHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UpdateStats(int32 ChangeInItemAmount) override;

	UCombatComponent* CombatComponent;
};

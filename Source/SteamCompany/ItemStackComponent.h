// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemEffectComponent.h"
#include "ItemStackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateCurrentItems);

class UItemEffectComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UItemStackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemStackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddItemEffect(TSubclassOf<UItemEffectComponent> ItemType);

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TArray<TSubclassOf<UItemEffectComponent>> CurrentItems;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateCurrentItems OnUpdateCurrentItems;
};

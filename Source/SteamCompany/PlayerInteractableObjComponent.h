// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractableObjComponent.generated.h"


// Delegate declaration
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UPlayerInteractableObjComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractableObjComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteract OnInteract;
};

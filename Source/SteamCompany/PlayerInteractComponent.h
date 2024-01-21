// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractableObjComponent.h"
#include "PlayerInteractComponent.generated.h"

class UInputMappingContext;
class UInputAction;
class UPlayerInteractableObjComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UPlayerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	 // Reference to Owner
    UPROPERTY()
    ACharacter* Character;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** MappingContext */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InteractMappingContext;

	/** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UFUNCTION()
	void Interact();

	/** Raycast distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float RaycastDistance = 1000.0f;
};

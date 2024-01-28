// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CombatComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PauseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumeButtonClicked);

class UInputMappingContext;
class UInputAction;
class ACharacter;
class UCombatComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UPauseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPauseComponent();

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
	UInputMappingContext* PauseMappingContext;

	/** Pause Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UPROPERTY(BlueprintAssignable)
	FOnResumeButtonClicked OnResumeButtonClicked;
};

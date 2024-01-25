// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "ShopSystemComponent.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopChange, const TArray<TSubclassOf<UItemEffectComponent>>&, ItemOptions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopEndDialog);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UShopSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShopSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Reference to Owner
    UPROPERTY()
    ACharacter* Character;

	UPROPERTY()
	APlayerController* PlayerController;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Delegate for changing dialog text
    UPROPERTY(BlueprintAssignable, Category="Dialog")
	FOnShopChange OnShopChange;

    // Delegate for ending dialog
    UPROPERTY(BlueprintAssignable, Category="Dialog")
	FOnShopEndDialog OnEndDialog;

	UFUNCTION()
	void LerpCameraToJester();

	UFUNCTION()
	void UpdateCameraLerp();

	FTimerHandle CameraLerpTimerHandle;
	float LerpAlpha = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSubclassOf<UItemEffectComponent>> ItemOptions;

	/** MappingContext */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InteractMappingContext;

	/** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UFUNCTION()
	void Interact();

	int32 CurrentLineIndex = 0;
	int32 CurrentLevel = 0;

	float StartingMaxWalkSpeed;

	UCameraComponent* CameraComponent;

	AActor* ShopKeeperActor;

		
};

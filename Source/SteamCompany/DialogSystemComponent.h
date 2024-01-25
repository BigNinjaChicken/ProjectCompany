// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "DialogSystemComponent.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeDialogText, const FString&, DialogText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndDialog);

USTRUCT(BlueprintType)
struct FDialog
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TArray<FString> Lines;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UDialogSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogSystemComponent();

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
    FOnChangeDialogText OnChangeDialogText;

    // Delegate for ending dialog
    UPROPERTY(BlueprintAssignable, Category="Dialog")
    FOnEndDialog OnEndDialog;

	UFUNCTION(BlueprintCallable)
	void BeginDialog();

	UFUNCTION()
	void LerpCameraToJester();

	UFUNCTION()
	void UpdateCameraLerp();

	FTimerHandle CameraLerpTimerHandle;
	float LerpAlpha = 0.0f;

	// Dialog for levels number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TMap<int32, FDialog> LevelDialog;

	/** MappingContext */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InteractMappingContext;

	/** Fire Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UFUNCTION()
	void Interact();

	bool bInDialog = false;

	int32 CurrentLineIndex = 0;
	int32 CurrentLevel = 0;

	float StartingMaxWalkSpeed;

	UCameraComponent* CameraComponent;

	AActor* JesterActor;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "DialogSystemComponent.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeDialogText, const FString&, DialogText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndDialog);

USTRUCT(BlueprintType)
struct FDialog
{
    GENERATED_BODY()

	// All lines used in the dialog for a given level
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

	// Owning player character
	UPROPERTY()
		ACharacter* Character;

	// Owning player controller
	UPROPERTY()
		APlayerController* PlayerController;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//************************************
	// Method:    BeginDialog
	// FullName:  UDialogSystemComponent::BeginDialog
	// Access:    public 
	// Returns:   void
	// Qualifier: 
	// 
	// Responsible for starting the Dialog process.
	//************************************
	UFUNCTION(BlueprintCallable)
		void BeginDialog();


	//************************************
	// Method:    ServerResetMovementSpeed
	// FullName:  UDialogSystemComponent::ServerResetMovementSpeed
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// 
	// Resets the movement speed of player
	//************************************
	UFUNCTION()
		void ResetMovementSpeed(float Speed);
	UFUNCTION(Server, Reliable)
		void ServerResetMovementSpeed();


	//************************************
	// Method:    EndDialog
	// FullName:  UDialogSystemComponent::EndDialog
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// 
	// Responsible for ending the Dialog process.
	//************************************
	UFUNCTION(BlueprintCallable)
		void EndDialog();


	//************************************
	// Method:    LerpCameraToJester
	// FullName:  UDialogSystemComponent::LerpCameraToJester
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// 
	// Gets jester and starts time handle for UpdateCameraLerp
	//************************************
	UFUNCTION()
		void LerpCameraToJester();


	//************************************
	// Method:    UpdateCameraLerp
	// FullName:  UDialogSystemComponent::UpdateCameraLerp
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// 
	// Moves camera to look at jester
	//************************************
	UFUNCTION()
		void UpdateCameraLerp();


	//************************************
	// Method:    Interact
	// FullName:  UDialogSystemComponent::Interact
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// 
	// Run on player input while binDialog
	//************************************
	UFUNCTION()
		void Interact();

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
		FOnChangeDialogText OnChangeDialogText;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
		FOnEndDialog OnEndDialog;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputMappingContext* InteractMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* InteractAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		bool bDialogComplete = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		bool bInDialog = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		AActor* JesterActor;

	UPROPERTY()
		FTimerHandle CameraLerpTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Debug")
		float LerpAlpha = 0.0f;

	// Dialog for levels number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
		TMap<int32, FDialog> LevelDialog;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		int32 CurrentLineIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		int32 CurrentLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		float StartingMaxWalkSpeed;
};

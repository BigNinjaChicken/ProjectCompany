// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseComponent.h"

#include "EnhancedInputSubsystems.h"
#include "CombatComponent.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
//#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPauseComponent::UPauseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPauseComponent::BeginPlay()
{
	Super::BeginPlay();

	// Attempt to find Owner Character
	Character = Cast<APawn>(GetOwner());
	if (!Character)
	{
		// Handle the case where no CombatComponent is found
		UE_LOG(LogTemp, Warning, TEXT("Owner is not ACharacter"));
		return;
	}

	// ...
	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Pause action when using touch input
			Subsystem->AddMappingContext(PauseMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Bind PauseGame to pause action
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &UPauseComponent::PauseGame);
		}
	}
}


// Called every frame
void UPauseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPauseComponent::PauseGame()
{
	OnResumeButtonClicked.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Here1"));
}


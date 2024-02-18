// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectiveUserWidget.h"
#include "ObjectiveItemUserWidget.h"
#include "ObjectiveSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveComplete, UObjectiveItemUserWidget*, CompleteObjectiveItemUserWidgetRed);

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
    TalkJesterGod,
    TalkShop,
	DestroySacredStatues,
	KillBoss,
	EnterPortal
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UObjectiveSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObjectiveSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		APlayerController* OwningPlayerController;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		TArray<EObjectiveType> CurrentObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		TSubclassOf<UUserWidget> ObjectiveUserWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		TSubclassOf<UUserWidget> ObjectiveItemUserWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		UObjectiveUserWidget* ObjectiveUserWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
		TMap<EObjectiveType, FString> ObjectiveTextMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		TMap<EObjectiveType, UObjectiveItemUserWidget*> ObjectiveItemRefMap;

	UFUNCTION()
		void UpdateCurrentObjectives();

	UFUNCTION(BlueprintCallable)
		void CompleteObjective(EObjectiveType CompletedObjective);

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnObjectiveComplete OnObjectiveComplete;
};

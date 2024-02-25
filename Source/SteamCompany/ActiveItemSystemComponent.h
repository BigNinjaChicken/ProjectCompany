// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "ItemAbilityUserWidget.h"
#include "ActiveItemSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateActiveCurrentItems, UActiveItemEffectComponent*, MostRecentItem);

class UActiveItemsUserWidget;
class UItemAbilityUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UActiveItemSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActiveItemSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void AddItemEffect(TSubclassOf<UActiveItemEffectComponent> ItemType);

	UPROPERTY(BlueprintAssignable)
		FOnUpdateActiveCurrentItems OnUpdateActiveCurrentItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UActiveItemsUserWidget> ActiveItemsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UItemAbilityUserWidget> ItemAbilityWidgetClass;

	UPROPERTY()
		UActiveItemsUserWidget* ActiveItemWidget;

	int itemCount = 0;
};

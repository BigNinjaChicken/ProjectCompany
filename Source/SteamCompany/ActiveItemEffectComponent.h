// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemAbilityUserWidget.h"
#include "ActiveItemEffectComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UActiveItemEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActiveItemEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UTexture2D* ItemIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FString ItemName = "Null Item";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FString Button = "Null Item";

	UPROPERTY()
	UItemAbilityUserWidget* ItemAbilityUserWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FString ItemDesc = "Null Item";
};

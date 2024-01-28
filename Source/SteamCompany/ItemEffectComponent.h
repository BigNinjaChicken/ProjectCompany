// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Texture2D.h"
#include "ItemEffectComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UItemEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// How many of the item the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount = 1;

	virtual void UpdateStats(int32 ChangeInItemAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UTexture2D* ItemIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FString ItemName = "Null Item";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FString ItemDesc = "Null Item";
};

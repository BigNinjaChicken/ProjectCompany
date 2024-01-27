// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpearProjectileActor.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class STEAMCOMPANY_API ASpearProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpearProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* ArrowStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UBoxComponent* ArrowTipBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* RootCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
    void OnArrowTipOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

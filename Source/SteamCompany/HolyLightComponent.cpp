// Fill out your copyright notice in the Description page of Project Settings.


#include "HolyLightComponent.h"

#include "BehaviorTree/BlackboardComponent.h"


// Sets default values for this component's properties
UHolyLightComponent::UHolyLightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHolyLightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//TargetPlayer = Cast<APawn>(Cast<AAIController>(GetOwner())->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
	//Cast<AAIController>(GetOwner())->GetBlackboardComponent()->GetValueAsObject("TargetPlayer");
}


// Called every frame
void UHolyLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHolyLightComponent::Attack()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	//SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.bNoFail = true;
	
	FVector SpawnLocation = TargetPlayer->GetActorLocation();
	SpawnLocation.Z = 0;
	FRotator SpawnRotation = TargetPlayer->GetActorRotation();
	
	GetWorld()->SpawnActor<AActor>(HolyLightActor, SpawnLocation, SpawnRotation, SpawnParams);
}


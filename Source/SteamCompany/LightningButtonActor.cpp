// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningButtonActor.h"

// Sets default values
ALightningButtonActor::ALightningButtonActor()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ALightningButtonActor::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = WidgetComponent->GetWidget();
	if (!Widget) {
		UE_LOG(LogTemp, Warning, TEXT("Widget null"));
		return;
	}
}

// Called every frame
void ALightningButtonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


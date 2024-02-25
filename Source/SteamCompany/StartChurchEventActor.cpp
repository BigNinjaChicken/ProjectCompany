// Fill out your copyright notice in the Description page of Project Settings.


#include "StartChurchEventActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "PlayerInteractComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"
#include "PlayerInteractableObjComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "ObjectiveSystemComponent.h"

// Sets default values
AStartChurchEventActor::AStartChurchEventActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

	InteractableBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBoxComponent"));
	SetRootComponent(InteractableBoxComponent);
	InteractableWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableWidgetComponent"));
	InteractableWidgetComponent->SetupAttachment(GetRootComponent());
    DisplayRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DisplayRangeSphereComponent"));
    DisplayRangeSphereComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AStartChurchEventActor::BeginPlay()
{
	Super::BeginPlay();
    UPlayerInteractableObjComponent* PlayerInteractableObjComponent = Cast<UPlayerInteractableObjComponent>(GetComponentByClass(UPlayerInteractableObjComponent::StaticClass()));
    if (PlayerInteractableObjComponent)
    {
        // Bind to the FOnActorDead delegate
        PlayerInteractableObjComponent->OnInteract.AddDynamic(this, &AStartChurchEventActor::OnInteractHandler);
        
    }

    InteractableWidgetComponent->SetHiddenInGame(true);
    DisplayRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AStartChurchEventActor::OnOverlapBegin);
    DisplayRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AStartChurchEventActor::OnOverlapEnd);
}

// Called every frame
void AStartChurchEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartChurchEventActor::OnInteractHandler(ACharacter* InteractingPlayer)
{
	if (BossEventActor) {
        // Mark off objective
        TArray<AActor*> OutActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OutActors);
        for (AActor* OutActor : OutActors)
        {
            ACharacter* CharacterActor = Cast<ACharacter>(OutActor);
            if (CharacterActor) {
                UActorComponent* ActorComponent = CharacterActor->GetComponentByClass(UObjectiveSystemComponent::StaticClass());
                if (ActorComponent) {
                    UObjectiveSystemComponent* ObjectiveSystemComponent = Cast<UObjectiveSystemComponent>(ActorComponent);
                    ObjectiveSystemComponent->CompleteObjective(EObjectiveType::DestroySacredStatues);
                }
            }
        }

		BossEventActor->BeginBossEvent();
	}

	Destroy();
}


void AStartChurchEventActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    InteractableWidgetComponent->SetHiddenInGame(false);
}

void AStartChurchEventActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    InteractableWidgetComponent->SetHiddenInGame(true);
}
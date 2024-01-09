#include "VampireAttackComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

UVampireAttackComponent::UVampireAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UVampireAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    // Attempt to find the CombatComponent on the same actor
    CombatComp = GetOwner()->FindComponentByClass<UCombatComponent>();
    if (!CombatComp)
    {
        // Handle the case where no CombatComponent is found
        UE_LOG(LogTemp, Warning, TEXT("VampireAttackComponent requires a CombatComponent on the same actor!"));
    }

    // Attempt to find Owner Character
    Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        // Handle the case where no CombatComponent is found
        UE_LOG(LogTemp, Warning, TEXT("Owner is not ACharacter"));
    }

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(VampireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(BiteAction, ETriggerEvent::Triggered, this, &UVampireAttackComponent::Bite);
		}
	}
}

void UVampireAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Vampire attack logic that uses CombatComp...
}

void UVampireAttackComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UVampireAttackComponent, OnBiteDamageComplete);
    DOREPLIFETIME(UVampireAttackComponent, OnBiteCooldownBegin);
}

void UVampireAttackComponent::Bite()
{
    if (!Character)
    {
        return;
    }

    OnBiteCooldownBegin.Broadcast(BiteCooldown);

    FVector BiteLocation = Character->GetActorLocation(); // You might want to adjust this location
    float BiteRadius = 300.0f; // Set the radius for the bite sphere

    TArray<AActor*> OverlappedActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // Define object types to overlap with
//     ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Overlap with other pawns

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Character); // Ignore the character itself

    bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(
        this,
        BiteLocation,
        BiteRadius,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        OverlappedActors
    );

    /*Optional: Draw Debug Sphere for visual feedback*/
#if WITH_EDITOR
    DrawDebugSphere(GetWorld(), BiteLocation, BiteRadius, 32, FColor::Red, false, 2.0f);
#endif

    bool bBiteSuccessful = false;
    if (bOverlap)
    {
        for (AActor* OtherActor : OverlappedActors)
        {
            UCombatComponent* OtherCombatComp = Cast<UCombatComponent>(OtherActor->GetComponentByClass(UCombatComponent::StaticClass()));
            if (OtherCombatComp) {
                // Disables friendly fire
                if (!OtherCombatComp->bIsFriendly) {
                    ServerRequestDamage(OtherCombatComp, 10.0f);
                    bBiteSuccessful = true;
                }
            }
        }
    }

    if (bBiteSuccessful) {
        OnBiteDamageComplete.Broadcast();
    }
}

void UVampireAttackComponent::ServerRequestDamage_Implementation(UCombatComponent* OtherCombatComp, float DamageAmount)
{
    if (OtherCombatComp && OtherCombatComp->GetOwner()->HasAuthority())
    {
        OtherCombatComp->ServerTakeDamage(DamageAmount);
    }
}
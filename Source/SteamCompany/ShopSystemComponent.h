#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "ItemEffectComponent.h"
#include "Net/UnrealNetwork.h"
#include "ShopSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopChange, const TArray<TSubclassOf<UItemEffectComponent>>&, ItemOptions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopEndDialog);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UShopSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UShopSystemComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Functions for shop interaction
    UFUNCTION(BlueprintCallable)
        void EndShop();

    UFUNCTION(BlueprintCallable)
        void Interact();

    // Server functions
    UFUNCTION(NetMulticast, Reliable)
        void MulticastSetInteractSettings();

    UFUNCTION(Server, Reliable)
        void ServerGetStartingSpeed();

    UFUNCTION(NetMulticast, Reliable)
        void MulticastResetMovementSpeed();

    UFUNCTION()
        void LerpCameraToJester();

    UFUNCTION()
        void UpdateCameraLerp();

    // Helper function to reset input mode and cursor visibility
    UFUNCTION()
        void ResetInputMode();

    // Helper function to present item options in the shop
    UFUNCTION()
        void PresentItemOptions();

    // Delegate events
    UPROPERTY(BlueprintAssignable, Category = "Shop")
        FOnShopChange OnShopChange;

    UPROPERTY(BlueprintAssignable, Category = "Shop")
        FOnShopEndDialog OnEndDialog;

    // Timer handle for camera lerp
    UPROPERTY()
        FTimerHandle CameraLerpTimerHandle;

    // Replicated property
    UPROPERTY(Replicated, VisibleAnywhere, Category = "Debug")
        float StartingMaxWalkSpeed;

    // Item options for the shop
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
        TArray<TSubclassOf<UItemEffectComponent>> ItemOptions;

    // Shop state flag
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        bool bInShop = false;

    // References
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        ACharacter* Character;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        APlayerController* PlayerController;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        UCameraComponent* CameraComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
        AActor* ShopKeeperActor;

    // Replication setup
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

// TileActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileActor.generated.h"

class UStaticMeshComponent;
class UArrowComponent;

UENUM(BlueprintType)
enum class ETileShape : uint8
{
    Straight,
    Corner,
    ThreeWayIntersection,
	FourWayIntersection,
	EndCap
};

UCLASS()
class STEAMCOMPANY_API ATileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to handle changes in TileShape
	void OnTileShapeChanged();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChurchMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* ForwardArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* LeftArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* RightArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* BackwardArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileShape TileShape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsChurch = false;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

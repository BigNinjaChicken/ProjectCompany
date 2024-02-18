
#include "ObjectiveSystemComponent.h"
#include "ObjectiveItemUserWidget.h"
#include "Net/UnrealNetwork.h" // Include Unreal's networking support
#include "../../../../../../../Source/Runtime/Core/Public/Internationalization/Text.h"

UObjectiveSystemComponent::UObjectiveSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UObjectiveSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayerController = GetOwner()->GetInstigatorController<APlayerController>();
	if (!OwningPlayerController) {
		UE_LOG(LogTemp, Warning, TEXT("OwningPlayerController null"));
		return;
	}

	if (!ObjectiveUserWidgetClass) {
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveUserWidgetClass null"));
		return;
	}

	FName WidgetName = "ObjectiveUserWidget";
	ObjectiveUserWidget = CreateWidget<UObjectiveUserWidget>(OwningPlayerController, ObjectiveUserWidgetClass, WidgetName);
	ObjectiveUserWidget->AddToViewport();

	UpdateCurrentObjectives();
}


// Called every frame
void UObjectiveSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UObjectiveSystemComponent::UpdateCurrentObjectives()
{
	if (!ObjectiveUserWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveUserWidget null"));
		return;
	}

	if (!OwningPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwningPlayerController null"));
		return;
	}

	if (!ObjectiveItemUserWidgetClass) {
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveItemUserWidgetClass null"));
		return;
	}

	if (!ObjectiveUserWidget->ObjectiveVerticalBox) {
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveUserWidget->ObjectiveVerticalBox null"));
		return;
	}

	int i = 0;
	for (EObjectiveType CurrentObjective : CurrentObjectives)
	{
		FName WidgetName = FName(*FString::Printf(TEXT("ObjectiveItemUser%d"), i));
		UObjectiveItemUserWidget* ObjectiveItemUserWidget = CreateWidget<UObjectiveItemUserWidget>(OwningPlayerController, ObjectiveItemUserWidgetClass, WidgetName);

		// Check if the current objective exists in the ObjectiveTextMap before trying to access it
		if (!ObjectiveTextMap.Contains(CurrentObjective))
		{
			UE_LOG(LogTemp, Warning, TEXT("ObjectiveTextMap does not contain CurrentObjective"));
			continue;
		}

		ObjectiveItemUserWidget->ItemText = FText::FromString(ObjectiveTextMap[CurrentObjective]);
		ObjectiveItemRefMap.Add(CurrentObjective, ObjectiveItemUserWidget); 
		ObjectiveUserWidget->ObjectiveVerticalBox->AddChild(ObjectiveItemUserWidget);
		i++;
	}
}

void UObjectiveSystemComponent::CompleteObjective(EObjectiveType CompletedObjective)
{
	if (!ObjectiveItemRefMap.Contains(CompletedObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("CompletedObjective not found in ObjectiveItemRefMap."));
		return;
	}

	UObjectiveItemUserWidget* ObjectiveItemUserWidget = ObjectiveItemRefMap[CompletedObjective];
	if (!ObjectiveItemUserWidget) {
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveItemUserWidget is null for completed objective."));
		return;
	}

	ObjectiveItemUserWidget->ItemCheckBoxState = ECheckBoxState::Checked;

	OnObjectiveComplete.Broadcast(ObjectiveItemRefMap[CompletedObjective]);
}
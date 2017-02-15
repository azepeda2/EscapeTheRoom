// Copyright Alejandro Zepeda 2017.

#include "EscapeTheRoom.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing pressure plate."), *Owner->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// If enough weight is on pressure plate
	if (GetTotalWeightOnPlate() >= PressureWeightRequirement)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}



float UOpenDoor::GetTotalWeightOnPlate()
{
	float TotalWeight = 0.f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalWeight; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{
		TotalWeight += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on pressure plate."), *Actor->GetName());
	}

	return TotalWeight;
}


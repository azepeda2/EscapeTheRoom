// Copyright Alejandro Zepeda 2017.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEROOM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// how far our character can reach
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray Cast to grab item within reach
	void Grab();

	// Release the item currently held
	void Release();

	// Find physics handle
	void FindPhysicsHandleComponent();

	//Setup input component
	void SetupInputComponent();

	// Return hit for first physics object in reach
	const FHitResult GetFirstReachablePhysicsObject();

	// Return the object's location
	FVector GetReachLineStart();

	// Return the distance of the object's reach
	FVector GetReachLineEnd();

};

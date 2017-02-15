// Copyright Alejandro Zepeda 2017.

#include "EscapeTheRoom.h"
#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab()
{
	// Line Trace to see if we reach objects with physics body collision
	FHitResult HitResult = GetFirstReachablePhysicsObject();
	UPrimitiveComponent* ObjectToGrab = HitResult.GetComponent(); // mesh for object
	AActor* ActorHit = HitResult.GetActor();

	// If hit, attach physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(ObjectToGrab, NAME_None, //no bones in object
			ObjectToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Find Physics Handle component (run time)
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component."), *GetOwner()->GetName());
	}
}

// Find Input component (run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component."), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstReachablePhysicsObject()
{
	// Line-trace ray-casting
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, 
		GetReachLineStart(), 
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	return HitResult;
}

// Calculate player's location
FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation;

}

// Calculate object's reach
FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation + (PlayerViewpointRotation.Vector() * Reach);

}
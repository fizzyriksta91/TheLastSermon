// Fill out your copyright notice in the Description page of Project Settings.


#include "SharedCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"


// Sets default values
ASharedCamera::ASharedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// Create components
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = MinArmLength;
	SpringArmComponent->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ASharedCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASharedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<APawn*> Players;
	for (int32 i = 0; i < 2; ++i)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), i);
		if (PlayerPawn)
		{
			Players.Add(PlayerPawn);
		}
	}

	if (Players.Num()== 0) { return; }

	UpdateCamera(DeltaTime, Players);
}

FVector ASharedCamera::ComputeCenter(const TArray<APawn*>& Players) const
{
	if (Players.Num()== 1)
	{
		return Players[0]->GetActorLocation();
	}

	FVector Sum = FVector::ZeroVector;
	for (APawn* Player : Players)
	{
		Sum += Player->GetActorLocation();
	}
	return Sum / Players.Num();
}

float ASharedCamera::ComputeSeparation(const TArray<APawn*>& Players) const
{
	if (Players.Num() < 2)
		return 0.f;

	FVector Center = ComputeCenter(Players);
	float MaxDistance = 0.f;
	for (APawn* Player : Players)
	{
		float Distance = FVector::Dist(Player->GetActorLocation(), Center);
		MaxDistance = FMath::Max(MaxDistance, Distance);
	}
	return MaxDistance * 2.f;
}

void ASharedCamera::SetCameraPerspectiveIndex(int32 PerspectiveIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("SetCameraPerspectiveIndex called with index: %d"), PerspectiveIndex);
	UE_LOG(LogTemp, Warning, TEXT("CameraPerspectives array size: %d"), CameraPerspectives.Num());

	if (CameraPerspectives.IsValidIndex(PerspectiveIndex))
	{
		CurrentPerspectiveIndex = PerspectiveIndex;
		UE_LOG(LogTemp, Warning, TEXT("Successfully set perspective to index: %d"), PerspectiveIndex);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid perspective index: %d"), PerspectiveIndex);
	}
}

void ASharedCamera::UpdateCamera(float DeltaTime, const TArray<APawn*>& Players)
{
	FVector Center = ComputeCenter(Players);
	float Seperation = ComputeSeparation(Players);

	float Alpha = (ZoomOutSeparation > 0.f) ? FMath::Clamp(
		Seperation / ZoomOutSeparation, 0.f, 1.f) : 0.f;
	float DesiredArm = FMath::Lerp(MinArmLength, MaxArmLength, Alpha);

	float CurrentArm = SpringArmComponent->TargetArmLength;
	SpringArmComponent->TargetArmLength = FMath::FInterpTo(
		CurrentArm, DesiredArm, DeltaTime, CameraInterpSpeed);

	FVector DesiredLocation = Center + FVector(0.f, 0.f, CameraHeight);
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(),
		DesiredLocation, DeltaTime, PositionInterpSpeed);
	SetActorLocation(NewLocation);

	if (CameraPerspectives.IsValidIndex(CurrentPerspectiveIndex))
	{
		FRotator TargetRotation = CameraPerspectives[CurrentPerspectiveIndex];
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, CameraInterpSpeed);
		SetActorRotation(NewRotation);
	}

	TArray<APawn*> MutablePlayers = Players;
	ClampPlayersToBoundary(Center, MutablePlayers);
}

void ASharedCamera::ClampPlayersToBoundary(const FVector& Center, TArray<APawn*>& Players)
{
	for (APawn* Player : Players)
	{
		if (!Player) continue;

		FVector PlayerLocation = Player->GetActorLocation();
		FVector Difference = PlayerLocation - Center;
		FVector DifferenceXY = FVector(Difference.X, Difference.Y, 0.f);
		float DistanceXY = DifferenceXY.Size();

		if (DistanceXY > BoundaryRadius)
		{
			FVector ClampedXY = DifferenceXY.GetSafeNormal() * BoundaryRadius;
			FVector Target = Center + FVector(ClampedXY.X, ClampedXY.Y, Difference.Z);
			Player->SetActorLocation(Target, true);
		}
	}
}



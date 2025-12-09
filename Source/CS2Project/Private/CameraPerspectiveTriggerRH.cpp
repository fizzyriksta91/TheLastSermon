// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPerspectiveTriggerRH.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "SharedCamera.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ACameraPerspectiveTriggerRH::ACameraPerspectiveTriggerRH()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(200.f));
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACameraPerspectiveTriggerRH::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ACameraPerspectiveTriggerRH::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraPerspectiveTriggerRH::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (bRequirePlayerPawn)
	{
		APawn* Pawn = Cast<APawn>(OtherActor);
		if (!Pawn) return;

		// get the pawn's controller (works for local players / split-screen)
		APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
		if (!PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("CameraPerspectiveTrigger: Overlap actor has no player controller."));
			return;
		}

		UWorld* World = GetWorld();
		if (!World) return;

		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(World, ASharedCamera::StaticClass(), Found);
		if (Found.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("CameraPerspectiveTrigger: No ASharedCamera found in world."));
			return;
		}

		ASharedCamera* SharedCam = Cast<ASharedCamera>(Found[0]);
		if (!SharedCam) return;

		// Use the controller-aware setter so the correct player gets this view target
		SharedCam->SetCameraPerspectiveIndexForController(PC, PerspectiveIndex);
		UE_LOG(LogTemp, Log, TEXT("CameraPerspectiveTrigger: Set perspective %d for controller."), PerspectiveIndex);
		return;
	}
}



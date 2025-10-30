// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "SharedCamera.generated.h"

class UCameraComponent;

UCLASS()
class CS2PROJECT_API ASharedCamera : public AActor
{
	GENERATED_BODY()
	
	
public:	
	// Sets default values for this actor's properties
	ASharedCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinArmLength { 600.f };

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxArmLength { 800.f };

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomOutSeparation  { 900.f };
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraInterpSpeed { 3.f };

	UPROPERTY(EditAnywhere, Category = "Camera")
	float PositionInterpSpeed { 8.f };

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraHeight  { 0.f };

	UPROPERTY(EditAnywhere, Category = "Camera|Boundary")
	float BoundaryRadius  { 600.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TArray<FRotator> CameraPerspectives = {
		FRotator(-45.f, 140.f, 0.f),
		FRotator(-45.f, 90.f, 0.f),
		FRotator(-90.f, 0.f, 0.f),
		FRotator(-45.f, -90.f, 0.f)
	};

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	int32 CurrentPerspectiveIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraPerspectiveIndex(int32 PerspectiveIndex);

private:
	void UpdateCamera(float DeltaTime, const TArray<APawn*>& Players);
	
	void ClampPlayersToBoundary(const FVector& Center, TArray<APawn*>& Players);
	
	FVector ComputeCenter(const TArray<APawn*>& Players) const;
	
	float ComputeSeparation(const TArray<APawn*>& Players) const;
};

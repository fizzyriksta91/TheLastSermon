// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIControllerRH.h"

#include "AI/EnemyBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


AEnemyAIControllerRH::AEnemyAIControllerRH()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// Note: The perception delegate is now bound in BeginPlay to ensure the component is fully initialized
}

void AEnemyAIControllerRH::BeginPlay()
{
	Super::BeginPlay();

	// Bind the perception delegate here instead of constructor
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIControllerRH::OnPerceptionUpdated);
		UE_LOG(LogTemp, Warning, TEXT("Perception delegate bound"));
	}

	AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(GetPawn());
	if (Enemy && BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	
	BlackboardComp = GetBlackboardComponent();
	
	InitializeBlackboard();

	
}

void AEnemyAIControllerRH::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (CanSenseActor(Actor, EAISenseRH::Sight))
		{
			HandleSensedSight(Actor);
			UE_LOG(LogTemp, Warning, TEXT("Sense Actor Detected"));
		}
		if (CanSenseActor(Actor, EAISenseRH::Hearing))
		{
			// Logic for when the actor is heard
			UE_LOG(LogTemp, Warning, TEXT("Hearing Actor Detected"));
		}
		if (CanSenseActor(Actor, EAISenseRH::Damage))
		{
			HandleSensedDamage(Actor);
			UE_LOG(LogTemp, Warning, TEXT("Damage Actor Detected"));
		}
	}
}

void AEnemyAIControllerRH::InitializeBlackboard()
{
	
	if (!BlackboardComp)
		return;
	
	SetIdleState();
}

void AEnemyAIControllerRH::SetIdleState()
{
	if (!BlackboardComp)
		return;
	
	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"), EEnemyStatesRH::IdleState);
}

void AEnemyAIControllerRH::SetAttackingState()
{
	if (!BlackboardComp)
		return;

	APawn* PlayerPawn0 = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn* PlayerPawn1 = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);

	APawn* ClosestPawn = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	APawn* EnemyPawn = GetPawn();
	if (!EnemyPawn)
		return;
		
	if (PlayerPawn0)
	{
		ICombatRH* CombatInterface = Cast<ICombatRH>(PlayerPawn0);
		if (CombatInterface && !CombatInterface->IsDead())
		{
			float Distance = FVector::Dist(
				EnemyPawn->GetActorLocation(), PlayerPawn0->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestPawn = PlayerPawn0;
			}
		}
		
	}
	if (PlayerPawn1)
	{
		ICombatRH* CombatInterface = Cast<ICombatRH>(PlayerPawn1);
		if (CombatInterface && !CombatInterface->IsDead())
		{
			float Distance = FVector::Dist(
				EnemyPawn->GetActorLocation(), PlayerPawn1->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestPawn = PlayerPawn1;
			}
		}
		
	}
	if (ClosestPawn)
	{
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), ClosestPawn);
		BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyStatesRH::AttackingState);
	}
	else
	{
		SetIdleState();
	}
}

EEnemyStatesRH AEnemyAIControllerRH::GetCurrentState() const
{
	if (!BlackboardComp)
		return EEnemyStatesRH::IdleState;

	uint8 CurrentStateValue = BlackboardComp->GetValueAsEnum(TEXT("CurrentState"));
	return static_cast<EEnemyStatesRH>(CurrentStateValue);
}

void AEnemyAIControllerRH::HandleSensedSight(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT(
		"HandleSensedSight called for actor: %s"), *Actor->GetName());

	EEnemyStatesRH CurrentState = GetCurrentState();
	if (CurrentState == EEnemyStatesRH::IdleState)
	{
		SetAttackingState();
		UE_LOG(LogTemp, Warning, TEXT("State changed to AttackingState"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(
			"Already in state %d, not switching to attacking"), (int32)CurrentState);
	}
	
}

void AEnemyAIControllerRH::HandleSensedDamage(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT(
		"HandleSensedDamage called for actor: %s"), *Actor->GetName());
	EEnemyStatesRH CurrentState = GetCurrentState();
	if (CurrentState == EEnemyStatesRH::IdleState)
	{
		SetAttackingState();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(
			"Already in state %d, not switching to attacking"), (int32)CurrentState);
	}
}

bool AEnemyAIControllerRH::CanSenseActor(AActor* Actor, EAISenseRH Sense) const
{
	if (!Actor || !AIPerceptionComponent)
		return false;

	FActorPerceptionBlueprintInfo Info;
	
	AIPerceptionComponent->GetActorsPerception(Actor, Info);

	const FAISenseID SightID = UAISense_Sight::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID();
	const FAISenseID HearingID = UAISense_Hearing::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID();
	const FAISenseID DamageID = UAISense_Damage::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID();

	for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
	{
		switch (Sense)
		{
		case EAISenseRH::Sight:
			if (Stimulus.Type == SightID && Stimulus.WasSuccessfullySensed())
				return true;
			break;
		case EAISenseRH::Hearing:
			if (Stimulus.Type == HearingID && Stimulus.WasSuccessfullySensed())
				return true;
			break;
		case EAISenseRH::Damage:
			if (Stimulus.Type == DamageID && Stimulus.WasSuccessfullySensed())
				return true;
			break;
		default:
			break;
		}
	}
	return false;
}




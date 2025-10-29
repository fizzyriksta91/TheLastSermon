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
	
	// Initialize the blackboard component
	BlackboardComp = GetBlackboardComponent();
}

void AEnemyAIControllerRH::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess: %s possessed by %s"), *GetName(), InPawn ? *InPawn->GetName() : TEXT("null"));

	const float StartDelay = 0.15f;
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(BehaviorTreeStartTimer, this, &AEnemyAIControllerRH::StartBehaviorTreeDeferred, StartDelay, false);
	}
	
}

void AEnemyAIControllerRH::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		// Check if the actor can be sensed by sight, hearing, or damage
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

void AEnemyAIControllerRH::StartBehaviorTreeDeferred()
{
	if (!BehaviorTreeAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("No BehaviorTreeAsset assigned"));
		return;
	}

	if (RunBehaviorTree(BehaviorTreeAsset))
	{
		BlackboardComp = GetBlackboardComponent();
		InitializeBlackboard();
		UE_LOG(LogTemp, Warning, TEXT("Behavior tree started (deferred)"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to run BehaviorTreeAsset (deferred)"));
	}
}

// Initialize the blackboard and set the initial state
void AEnemyAIControllerRH::InitializeBlackboard()
{
	
	if (!BlackboardComp)
		return;
	
	SetIdleState();
}

// Set the enemy's state to Idle
void AEnemyAIControllerRH::SetIdleState()
{
	if (!BlackboardComp)
		return;
	
	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"), EEnemyStatesRH::IdleState);
}

// Set the enemy's state to Dead
void AEnemyAIControllerRH::SetDeadState()
{
	if (!BlackboardComp)
		return;
	
	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"), EEnemyStatesRH::DeadState);
}

// Set the enemy's state to Attacking and choose the closest player as the target
void AEnemyAIControllerRH::SetAttackingState()
{
	if (!BlackboardComp)
		return;
	 // Find the closest player pawn 
	APawn* PlayerPawn0 = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn* PlayerPawn1 = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);

	// If no players are found, revert to idle state
	APawn* ClosestPawn = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	// Get the enemy's pawn and ensure it's valid
	APawn* EnemyPawn = GetPawn();
	if (!EnemyPawn)
		return;
		
	// Check distance to Player 0
	if (PlayerPawn0)
	{
		// Ensure the player is alive by checking the combat interface
		ICombatRH* CombatInterface = Cast<ICombatRH>(PlayerPawn0);
		if (CombatInterface && !CombatInterface->IsDead())
		{
			// Calculate distance to Player 0
			float Distance = FVector::Dist(
				EnemyPawn->GetActorLocation(), PlayerPawn0->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				// Update closest player and distance
				ClosestDistance = Distance;
				ClosestPawn = PlayerPawn0;
			}
		}
	}
	// Check distance to Player 1
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
	// If a closest player is found, set them as the attack target and change state to Attacking
	if (ClosestPawn)
	{
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), ClosestPawn);
		BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyStatesRH::AttackingState);
	}
	else
	{
		// No valid players found, revert to idle state
		SetIdleState();
	}
}

// Get the current state from the blackboard
EEnemyStatesRH AEnemyAIControllerRH::GetCurrentState() const
{
	if (!BlackboardComp)
		return EEnemyStatesRH::IdleState;

	// Retrieve the enum value and cast it to EEnemyStatesRH
	uint8 CurrentStateValue = BlackboardComp->GetValueAsEnum(TEXT("CurrentState"));
	return static_cast<EEnemyStatesRH>(CurrentStateValue);
}

// Handle logic when an actor is sensed by sight
void AEnemyAIControllerRH::HandleSensedSight(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT(
		"HandleSensedSight called for actor: %s"), *Actor->GetName());

	// Only switch to attacking state if currently idle
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

// Handle logic when an actor is sensed by damage
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

// Check if the AI can sense the given actor with the specified sense
bool AEnemyAIControllerRH::CanSenseActor(AActor* Actor, EAISenseRH Sense) const
{
	// Ensure the actor and perception component are valid
	if (!Actor || !AIPerceptionComponent)
		return false;

	// Get perception info for the actor and check stimuli
	FActorPerceptionBlueprintInfo Info;
	AIPerceptionComponent->GetActorsPerception(Actor, Info);

	// Define sense IDs for comparison
	const FAISenseID SightID = UAISense_Sight::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID();
	const FAISenseID HearingID = UAISense_Hearing::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID();
	const FAISenseID DamageID = UAISense_Damage::StaticClass()->GetDefaultObject<UAISense>()->GetSenseID();

	// Check the last sensed stimuli for the specified sense
	for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
	{
		// Compare the stimulus type with the requested sense
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




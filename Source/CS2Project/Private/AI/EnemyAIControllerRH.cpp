// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIControllerRH.h"

#include "AI/EnemyBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Math/UnrealMathUtility.h"

AEnemyAIControllerRH::AEnemyAIControllerRH()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	AttackTarget = nullptr;
}

void AEnemyAIControllerRH::BeginPlay()
{
	Super::BeginPlay();

	// Bind the perception updated delegate
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIControllerRH::OnPerceptionUpdated);
	}
	
	BlackboardComp = GetBlackboardComponent();
}

// Called when the AI controller possesses a pawn
void AEnemyAIControllerRH::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Start the behavior tree after a short delay to ensure everything is initialized
	const float StartDelay = 0.3f;
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(BehaviorTreeStartTimer, 
			this, &AEnemyAIControllerRH::StartBehaviorTreeDeferred, 
			StartDelay, false);
	}
}

// Called when the perception component updates its sensed actors
void AEnemyAIControllerRH::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (!bBehaviorTreeStarted)
		return;

	// Iterate through all updated actors
	for (AActor* Actor : UpdatedActors)
	{
		if (!IsValidTarget(Actor))
			continue;

		// Check if the actor can be sensed by sight, hearing, or damage
		if (CanSenseActor(Actor, EAISenseRH::Sight))
		{
			HandleSensedSight(Actor);
		}
		if (CanSenseActor(Actor, EAISenseRH::Hearing))
		{
			// Logic for when the actor is heard, potential future implementation
		}
		if (CanSenseActor(Actor, EAISenseRH::Damage))
		{
			HandleSensedDamage(Actor);
		}
	}
}

// Start the behavior tree after possession is complete
void AEnemyAIControllerRH::StartBehaviorTreeDeferred()
{
	UBehaviorTree* TreeToRun = nullptr;
	
	APawn* PossessedPawn = GetPawn();
	if (PossessedPawn)
	{
		// Check for a behavior tree override on the possessed pawn
		if (AEnemyBaseCharacter* EnemyPawn = Cast<AEnemyBaseCharacter>(PossessedPawn))
		{
			TreeToRun = EnemyPawn->BehaviorTreeAssetOverride;
		}
	}

	if (!TreeToRun)
	{
		TreeToRun = BehaviorTreeAsset;
	}

	if (!TreeToRun)
		return;
	
	//  Run the behavior tree and initialize the blackboard
	if (RunBehaviorTree(TreeToRun))
	{
		BlackboardComp = GetBlackboardComponent();
		InitializeBlackboard();
		bBehaviorTreeStarted = true;

		if (AIPerceptionComponent)
		{
			AIPerceptionComponent->ForgetAll();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to run BehaviorTreeAsset (deferred)"));
	}
}

// Check if the given actor is a valid target for the AI
bool AEnemyAIControllerRH::IsValidTarget(AActor* Actor) const
{
	if (!Actor || Actor == GetPawn())
		return false;

	if (Actor->IsA(AEnemyBaseCharacter::StaticClass()))
		return false;

	APawn* TargetPawn = Cast<APawn>(Actor);
	if (!TargetPawn)
		return false;

	if (!TargetPawn->IsPlayerControlled())
		return false;

	ICombatRH* CombatInterface = Cast<ICombatRH>(Actor);
	if (CombatInterface && CombatInterface->IsDead())
		return false;

	return true;
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
	AttackTarget = nullptr;
}

// Set the enemy's state to Dead
void AEnemyAIControllerRH::SetDeadState()
{
	if (!BlackboardComp)
		return;
	
	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"), EEnemyStatesRH::DeadState);
	AttackTarget = nullptr;
}

// Set the enemy's state to Attacking and choose the closest player as the target
void AEnemyAIControllerRH::SetAttackingState()
{
	if (!BlackboardComp)
		return;

	APawn* PlayerPawn0 = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn* PlayerPawn1 = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);

	APawn* EnemyPawn = GetPawn();
	if (!EnemyPawn)
	{
		SetIdleState();
		return;
	}

	// Collect valid players
	TArray<APawn*> Candidates;
	auto AddIfValid = [&](APawn* Candidate)
	{
		if (!Candidate) 
			return;
		
		if (Candidate->IsA(AEnemyBaseCharacter::StaticClass())) 
			return;
		
		if (!Candidate->IsPlayerControlled()) 
			return;
		
		ICombatRH* CombatInterface = Cast<ICombatRH>(Candidate);
		if (CombatInterface && CombatInterface->IsDead()) 
			return;
		
		Candidates.Add(Candidate);
	};

	AddIfValid(PlayerPawn0);
	AddIfValid(PlayerPawn1);

	if (Candidates.Num() == 0)
	{
		SetIdleState();
		return;
	}

	APawn* Chosen = nullptr;

	if (Candidates.Num() == 1)
	{
		Chosen = Candidates[0];
	}
	else
	{
		// Randomly choose one of the candidates
		int32 Index = FMath::RandRange(0, Candidates.Num() - 1);
		Chosen = Candidates.IsValidIndex(Index) ? Candidates[Index] : Candidates[0];
	}

	if (Chosen)
	{
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), Chosen);
		AttackTarget = Chosen;
		BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyStatesRH::AttackingState);
	}
	else
	{
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
	if (!IsValidTarget(Actor) || !AIPerceptionComponent)
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




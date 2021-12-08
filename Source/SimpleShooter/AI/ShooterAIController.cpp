// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
//#include "SimpleShooter/Characters/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"
#include "SimpleShooter/AI/AIPerceptionComponent_SH.h"

AShooterAIController::AShooterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	//Perception = CreateDefaultSubobject<UAIPerceptionComponent_SH>(TEXT("AI Perception"));
	SetGenericTeamId(FGenericTeamId(7));
}

ETeamAttitude::Type AShooterAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	
	if (const APawn* OtherPawn = Cast<AShooterCharacter>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}

	return ETeamAttitude::Neutral;
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	/*Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);*/
	if (!AIBehavior)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ShooterAIController.h(26) Behavior_Tree doens't respond!\n (Check if is set up)"));
		return;
	}
	RunBehaviorTree(AIBehavior);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AIPawn = GetPawn();
	GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), AIPawn->GetActorLocation());
	GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), AIPawn);
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if (!ControlledCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ShooterAIController.cpp(39) failed cast to AShooterCharacter"));
		return true;
	}
	return ControlledCharacter->GetIsDead();
}
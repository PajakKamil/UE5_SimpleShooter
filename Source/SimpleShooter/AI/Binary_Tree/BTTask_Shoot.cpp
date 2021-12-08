// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot() : AIFireRate(0.3f)
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//AShooterCharacter* ShooterCharacterRef = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ShooterCharacterRef = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ShooterCharacterRef)
		return EBTNodeResult::Failed;

	ShooterCharacterRef->Shoot();
	GetWorld()->GetTimerManager().SetTimer(TimerShootPause, this, &UBTTask_Shoot::StopShooting, AIFireRate, false);

	return EBTNodeResult::Succeeded;
}

void UBTTask_Shoot::StopShooting()
{
	if (!ShooterCharacterRef)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerShootPause);
		return;
	}
	ShooterCharacterRef->StopShooting();
}
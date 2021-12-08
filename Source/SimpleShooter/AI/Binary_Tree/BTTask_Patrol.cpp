// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_Patrol::UBTTask_Patrol() : Radius(1000)
{
	NodeName = "Get Random Point In Navigable Radius";
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	//FName BlackboardKeyName = GetSelectedBlackboardKey();
	FVector Origin = OwnerComp.GetOwner()->GetActorLocation();
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", UNavigationSystemV1::GetRandomPointInNavigableRadius(this, Origin, Radius));

	return EBTNodeResult::Succeeded;
}
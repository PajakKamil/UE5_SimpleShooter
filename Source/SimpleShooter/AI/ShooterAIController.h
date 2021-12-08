// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "ShooterAIController.generated.h"

//class AShooterCharacter;

UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AShooterAIController();
	virtual void Tick(float DeltaTime) override;
	bool IsDead() const;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;
private:
	//AShooterCharacter* Player;
	/*APawn* Player;*/
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
		UBehaviorTree* AIBehavior;
	
	APawn* PlayerPawn;
	APawn* AIPawn;

	//FGenericTeamId TeamId;
	//UPROPERTY(EditAnywhere)
		//UAIPerceptionComponent_SH* Perception;
};

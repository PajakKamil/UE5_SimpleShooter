// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "SimpleShooter/AI/ShooterAIController.h"

void AKillEmAllGameMode::BeginPlay()
{
	Super::BeginPlay();
	for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
			Enemies++;
}

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	UE_LOG(LogTemp, Warning, TEXT("Pawn Killed! (This is PawnKilled function)"));

	APlayerController* PlayerControllerRef = Cast<APlayerController>(PawnKilled->GetController());

	if (PlayerControllerRef)
	{
		EndGame(false);
	}
	else
		Enemies--;
	UE_LOG(LogTemp, Warning, TEXT("%i Enemies left"), Enemies);

	if (Enemies <= 0)
		EndGame(true);

}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
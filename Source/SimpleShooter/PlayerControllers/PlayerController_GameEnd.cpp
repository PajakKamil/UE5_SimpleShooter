// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_GameEnd.h"
#include "Blueprint/UserWidget.h"

APlayerController_GameEnd::APlayerController_GameEnd() : RestartDelay(5.f)
{

}

void APlayerController_GameEnd::BeginPlay()
{
	Super::BeginPlay();
	Crosshair = CreateWidget(this, CrosshairWidget);
	if (Crosshair)
		Crosshair->AddToViewport();
}

void APlayerController_GameEnd::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner == true)
	{
		UUserWidget* LoseScreen = CreateWidget(this, WinWidget);
		if (!LoseScreen)
		{
			UE_LOG(LogTemp, Warning, TEXT("UUserWidget Lose_Widget implementation error in PlayerController_GameEnd.cpp(18)"));
			return;
		}
		Crosshair->RemoveFromViewport();
		LoseScreen->AddToViewport();
		GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &APlayerController_GameEnd::WinHandle, 1.0f, false, RestartDelay);
	}
	else
	{
		UUserWidget* WinScreen = CreateWidget(this, LoseWidget);
		if (!WinScreen)
		{
			UE_LOG(LogTemp, Warning, TEXT("UUserWidget Win_Widget implementation error in PlayerController_GameEnd.cpp(29)"));
			return;
		}
		Crosshair->RemoveFromViewport();
		WinScreen->AddToViewport();
		GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &APlayerController_GameEnd::LoseHandle, 1.0f, false, RestartDelay);
	}
}

void APlayerController_GameEnd::WinHandle()
{
	RestartLevel();
	GetWorldTimerManager().ClearTimer(RestartLevelTimerHandle);
}

void APlayerController_GameEnd::LoseHandle()
{
	RestartLevel();
	GetWorldTimerManager().ClearTimer(RestartLevelTimerHandle);
}

float APlayerController_GameEnd::GetRestartDelay() const
{
	return RestartDelay;
}
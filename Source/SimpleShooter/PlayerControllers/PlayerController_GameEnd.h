// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_GameEnd.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API APlayerController_GameEnd : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerController_GameEnd();
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	float GetRestartDelay() const;

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
		float RestartDelay;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> LoseWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> WinWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> CrosshairWidget;

	void WinHandle();
	void LoseHandle();

	FTimerHandle RestartLevelTimerHandle;
	UUserWidget* Crosshair;
};

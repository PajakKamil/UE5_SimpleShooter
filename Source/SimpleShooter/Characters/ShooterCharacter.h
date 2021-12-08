// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintPure, Category = "Player State")
		bool GetIsDead() const;
	UFUNCTION(BlueprintPure, Category = "Player State")
		float GetHealthPercent() const;
	void Shoot();
	void StopShooting();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Shooting")
		bool IsPlayerShooting;
	//UFUNCTION(BlueprintImplementableEvent, Category = "Physics")
	//	void DeadPhysics();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Immortale")
		bool bIsImmortal;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass;
	UPROPERTY()
		AGun* Gun;

	void PlayerMoveForward(float AxisValue);
	void PlayerMoveSide(float AxisValue);
	void PlayerJump();
	void PlayerLookUp(float AxisValue);
	void PlayerLookSide(float AxisValue);
	void PlayerLookUpRate(float AxisValue);
	void PlayerLookSideRate(float AxisValue);
	void DestroyHandle();
	void DeathHandle();
	void ExitGame();

	UPROPERTY(EditAnywhere)
		float RotationRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "True"))
		float HealthDefault;
	UPROPERTY(VisibleAnywhere)
		float Health;
	UPROPERTY(VisibleAnywhere)
		bool bIsDead;
};

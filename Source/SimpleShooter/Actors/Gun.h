// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UParticleSystem;
class USoundBase;
class UAmmoComponent;

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	void PullTrigger();
	virtual void Tick(float DeltaTime) override;
	void StopFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* ShootingParticle;
	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* ShootImpact;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundBase* ShootSound;
	UPROPERTY(EditANywhere, Category = "Sounds")
		USoundBase* ImpactSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
		UAmmoComponent* AmmoComponent;

	FTimerHandle TimerFullAuto;
	
	UPROPERTY(EditAnywhere)
		float Range;
	UPROPERTY(EditAnywhere)
		float Damage;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f))
		float FireRate;

	float LastShoot;

	bool GunTrace(FHitResult& Hit, FVector& ShootDirection);
	AController* GetOwnerController() const;
	void Fire();
};

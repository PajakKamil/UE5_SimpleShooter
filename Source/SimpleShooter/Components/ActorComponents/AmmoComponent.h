// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAmmoComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", Meta = (ClampMin = 0))
		int32 Ammo;
	int32 GetMagazineSize();
	void Reload();
	void AmmoCounter();
	bool GetReloadState();
	bool GetOutOfAmmo();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", Meta = (ClampMin = 0))
		int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", Meta = (ClampMin = 0))
		int32 MagazineSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", Meta = (ClampMin = 1))
		int32 AmmoCost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", Meta = (ClampMin = 0))
		float ReloadTime;

private:
	FTimerHandle ReloadTimer;
	void ReloadHandle();
	bool bReloading;
	bool bOutOfAmmo;
};

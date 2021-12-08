// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoComponent.h"

// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent() 
	: MaxAmmo(300), Ammo(30), MagazineSize(30), AmmoCost(1), ReloadTime(5.f), bReloading(false), bOutOfAmmo(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAmmoComponent::AmmoCounter()
{
	Ammo = FMath::Clamp(Ammo - AmmoCost, 0, MagazineSize);
	UE_LOG(LogTemp, Warning, TEXT("%i Ammo left"), Ammo);
}

int32 UAmmoComponent::GetMagazineSize()
{
	return MagazineSize;
}

void UAmmoComponent::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reload Function"));
	if (bReloading == false && bOutOfAmmo == false)
	{
		bReloading = true;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &UAmmoComponent::ReloadHandle, ReloadTime, false, ReloadTime);
	}
	else if (bOutOfAmmo == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of ammo SZMATO!"));
	}
}

void UAmmoComponent::ReloadHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloaded!"));
	MaxAmmo -= MagazineSize - Ammo;
	
	if (MaxAmmo <= 0)
	{
		int32 AmmoHelp = MaxAmmo + MagazineSize;
		Ammo = FMath::Clamp(Ammo + AmmoHelp, 0, MagazineSize);
		MaxAmmo = 0;
		bOutOfAmmo = true;
	}
	else
	{
		//Ammo = FMath::Clamp(Ammo + MaxAmmo, 0, MagazineSize);
		Ammo = MagazineSize;
	}
	UE_LOG(LogTemp, Warning, TEXT("Max Ammo: %i"), MaxAmmo);
	bReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
}

bool UAmmoComponent::GetReloadState()
{
	return bReloading;
}

bool UAmmoComponent::GetOutOfAmmo()
{
	return bOutOfAmmo;
}
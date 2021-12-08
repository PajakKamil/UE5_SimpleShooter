// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SimpleShooter/Components/ActorComponents/AmmoComponent.h"

// Sets default values
AGun::AGun() : Range(1000.f), Damage(10.f), FireRate(0.2f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

	AmmoComponent = CreateDefaultSubobject<UAmmoComponent>(TEXT("Ammo"));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LastShoot = GetWorld()->GetTimeSeconds();
}

void AGun::PullTrigger()
{	//You fire evry FireRate sec
	if (!GetWorldTimerManager().IsTimerActive(TimerFullAuto))
		GetWorld()->GetTimerManager().SetTimer(TimerFullAuto, this, &AGun::Fire, FireRate, true, 0.f);
	else
		GetWorldTimerManager().UnPauseTimer(TimerFullAuto);
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShootDirection)
{
	UGameplayStatics::SpawnEmitterAttached(ShootingParticle, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(ShootSound, Mesh, TEXT("MuzzleFlashSocket"));
	FVector CameraLocation;
	FRotator CameraRotation;
	if (!GetOwnerController())
		return false;
	GetOwnerController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
	ShootDirection = -CameraRotation.Vector();
	FVector End = CameraLocation + CameraRotation.Vector() * Range;
	FCollisionQueryParams IgnoreOwner;
	IgnoreOwner.AddIgnoredActor((GetOwner()));

	return GetWorld()->LineTraceSingleByChannel(HitResult, Mesh->GetComponentLocation(), End, ECC_GameTraceChannel1, IgnoreOwner);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("In Gun.cpp(68) APawn cast to Owner failed!"));
		return nullptr;
	}
	return OwnerPawn->GetController();
}

void AGun::Fire()
{
	if (AmmoComponent->Ammo > 0)
	{
		FHitResult HitResult;
		FVector ShootDirection;
		bool bSuccess = GunTrace(HitResult, ShootDirection);
		if (bSuccess)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootImpact, HitResult.Location, ShootDirection.Rotation(), true);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
			if (HitResult.GetActor())
			{
				FPointDamageEvent DamageEvent(Damage, HitResult, ShootDirection, nullptr);
				HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetOwnerController(), this->GetOwner());
			}
		}
		AmmoComponent->AmmoCounter();
	}
	else
	{
		AmmoComponent->Reload();
	}
}

void AGun::StopFire()
{
	if (!GetOwnerController())
		GetWorld()->GetTimerManager().ClearTimer(TimerFullAuto);
	else
		GetWorldTimerManager().PauseTimer(TimerFullAuto);
}
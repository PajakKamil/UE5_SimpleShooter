// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/Actor.h"
#include "SimpleShooter/Actors/Gun.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AShooterCharacter::AShooterCharacter() : RotationRate(25.f), HealthDefault(100.f), bIsDead(false), IsPlayerShooting(false),
											bIsImmortal(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_rSocket"));
	Gun->SetOwner(this);
	Gun->SetActorRelativeLocation(FVector(-2.195274f, -13.765935f, -2.183489f));
	Health = HealthDefault;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::PlayerMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &AShooterCharacter::PlayerMoveSide);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::PlayerLookUp);
	PlayerInputComponent->BindAxis(TEXT("LookSide"), this, &AShooterCharacter::PlayerLookSide);
	//PlayerInputComponent->BindAxis(TEXT("Shoot"), this, &AShooterCharacter::Shoot);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AShooterCharacter::PlayerJump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Quit"), IE_Pressed, this, &AShooterCharacter::ExitGame);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &AShooterCharacter::StopShooting);


	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::PlayerLookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookSideRate"), this, &AShooterCharacter::PlayerLookSideRate);

}

void AShooterCharacter::PlayerMoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::PlayerMoveSide(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::PlayerJump()
{
	Jump();
}

void AShooterCharacter::PlayerLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::PlayerLookSide(float AxisValue)
{
	//AddControllerYawInput(AxisValue);
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::PlayerLookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::PlayerLookSideRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
	IsPlayerShooting = true;
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsImmortal == true)
		return 0.f;
	if (bIsDead == true)
		return 0.f;
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageApplied, 0.f, HealthDefault);

	if (Health == 0)
	{
		DeathHandle();
		return DamageApplied;
	}

	UE_LOG(LogTemp, Warning, TEXT("%f HP left"), Health)
	return DamageApplied;
}

bool AShooterCharacter::GetIsDead() const
{
	return bIsDead;
}

void AShooterCharacter::DestroyHandle()
{
	Gun->Destroy();
	Destroy();
}

void AShooterCharacter::DeathHandle()
{
	/*UE_LOG(LogTemp, Warning, TEXT("You are Dead!"));*/
	bIsDead = true;
	Gun->StopFire();
	/*FTimerHandle DestroyTimeHandle;
	GetWorldTimerManager().SetTimer(DestroyTimeHandle, this, &AShooterCharacter::DestroyHandle, 1.0f, false, 10.f);*/
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/*GetOwner()->SetActorEnableCollision(false);*/

	ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("In ShooterCharacter.cpp(119) GameMode cast has failed!"));
		return;
	}
	GameMode->PawnKilled(this);
	DetachFromControllerPendingDestroy();
	this->GetMesh()->SetSimulatePhysics(true);
	/*GetWorldTimerManager().ClearTimer(DestroyTimeHandle);*/
}

void AShooterCharacter::ExitGame()
{
	APlayerController* SpecyficPlayer = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, SpecyficPlayer, EQuitPreference::Quit, true);
}

float AShooterCharacter::GetHealthPercent() const
{
	return (Health / HealthDefault);
}

void AShooterCharacter::StopShooting()
{
	Gun->StopFire();
	IsPlayerShooting = false;
}
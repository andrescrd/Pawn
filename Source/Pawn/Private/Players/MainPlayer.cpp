// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/MainPlayer.h"


#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	DeltaMovement = 100;
	Speed = 50;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap);
	MeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetRelativeLocation(FVector(100, 0, 50));
	BoxComponent->InitBoxExtent(FVector(32.f));
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Overlap);
	BoxComponent->SetupAttachment(MeshComponent);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((NewLocation - GetActorLocation()).Size() > 0 && !NewLocation.IsZero())
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), NewLocation, DeltaTime, Speed));
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &AMainPlayer::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AMainPlayer::MoveVertical);
}

void AMainPlayer::MoveHorizontal(float Value)
{
	if (Value == 0 || IsMoving()) return;
	(Value > 0) ? MoveRight() : MoveLeft();
}
void AMainPlayer::MoveVertical(float Value)
{
	if (Value == 0 || IsMoving()) return;
	(Value > 0) ? MoveUp() : MoveDown();
}

void AMainPlayer::MoveUp()
{
	if (HasWallCollision(FVector(DeltaMovement, 0, 50)))
		return;
	
	NewLocation = FVector(DeltaMovement, 0, 0) + GetActorLocation();
}

void AMainPlayer::MoveDown()
{
	if (HasWallCollision(FVector(-DeltaMovement, 0, 50)))
		return;
	
	NewLocation = FVector(-DeltaMovement, 0, 0) + GetActorLocation();
}

void AMainPlayer::MoveRight()
{
	if (HasWallCollision(FVector(0, DeltaMovement, 50)))
		return;
	
	NewLocation = FVector(0, DeltaMovement, 0) + GetActorLocation();
}

void AMainPlayer::MoveLeft()
{
	if (HasWallCollision(FVector(0, -DeltaMovement, 50)))
		return;
	
	NewLocation = FVector(0, -DeltaMovement, 0) + GetActorLocation();
}

bool AMainPlayer::HasWallCollision(const FVector RelativeLocation) const
{
	TArray<AActor*> Result;
	BoxComponent->SetRelativeLocation(RelativeLocation);
	BoxComponent->GetOverlappingActors(Result, AActor::StaticClass());
	return Result.Num() > 0;
}

bool AMainPlayer::IsMoving() const { return GetVelocity().Size() > 0; }
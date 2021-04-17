// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Follower.h"

#include "Math/UnitConversion.h"

// Sets default values
AFollower::AFollower()
{
	Speed = 50;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	MeshComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFollower::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((NewLocation - GetActorLocation()).Size() > 0 && !NewLocation.IsZero())
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), NewLocation, DeltaTime, Speed));
}

bool AFollower::CanMove() const { return OwnTarget != nullptr; }

void AFollower::SetTarget(APawn* NewTarget) { OwnTarget = NewTarget; }

void AFollower::OnProgress()
{
	if(!CanMove())
		return;

	NewLocation = OwnTarget->GetActorLocation();
}

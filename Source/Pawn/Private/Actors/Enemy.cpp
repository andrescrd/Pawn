// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
	NumberMovement = 2;
	CurrentMovement = 0;
	bIsMoveForward = true;
	DeltaMovement = 100;
	Speed = 50;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	MeshComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::OnProgress()
{
	SetupDirection();
	auto Location = GetActorLocation();
	Location += GetActorForwardVector() * DeltaMovement;
	SetActorLocation(Location, true);
}

void AEnemy::SetupDirection()
{
	if (bIsMoveForward)
	{
		if (CurrentMovement == NumberMovement)
		{
			bIsMoveForward = false;
			CurrentMovement--;
			AddActorLocalRotation(FRotator(0, -180, 0));
		}

		CurrentMovement++;
	}
	else
	{
		if (CurrentMovement == 0)
		{
			bIsMoveForward = true;
			CurrentMovement++;
			AddActorLocalRotation(FRotator(0, 180, 0));
		}

		CurrentMovement--;
	}
}

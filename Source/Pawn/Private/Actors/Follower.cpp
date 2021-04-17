// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Follower.h"

// Sets default values
AFollower::AFollower()
{
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

}


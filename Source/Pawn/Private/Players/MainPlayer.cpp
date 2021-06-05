// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/MainPlayer.h"


#include "AIController.h"
#include "Actors/Follower.h"
#include "Components/BoxComponent.h"
#include "Interfaces/Progressor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	bCanMove = true;
	DeltaMovement = 100;
	Speed = 50;
	MovementRate = 0.8f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	// MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap);
	MeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetRelativeLocation(FVector(100, 0, 50));
	BoxComponent->InitBoxExtent(FVector(32.f));
	// BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Overlap);
	BoxComponent->SetupAttachment(RootComponent);	

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	NewLocation = GetActorLocation();

	TArray<AActor*> ActorResult;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UProgressor::StaticClass(), ActorResult);

	for (auto Result : ActorResult)
		Progressors.Add(Cast<IProgressor>(Result));
}

void AMainPlayer::MoveFollowers()
{
	for (auto Prog : Progressors)
		Prog->OnProgress();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((NewLocation - GetActorLocation()).Size() > 0 && !NewLocation.IsZero())
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), NewLocation, DeltaTime, Speed),true);
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &AMainPlayer::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AMainPlayer::MoveVertical);
}

void AMainPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!OtherActor->IsA(AFollower::StaticClass()))
		return;

	auto CurrentFollower = Cast<AFollower>(OtherActor);
	if (!Followers.Contains(CurrentFollower))
	{
		if(Followers.Num() == 0)
		CurrentFollower->SetTarget(this);
		else
			CurrentFollower->SetTarget( Followers.Array()[Followers.Num() -1]);

		Followers.Add(CurrentFollower);
	}
}

void AMainPlayer::MoveHorizontal(float Value)
{
	if (Value == 0 || !CanMove()) return;
	(Value > 0) ? MoveRight() : MoveLeft();
	
}

void AMainPlayer::MoveVertical(float Value)
{
	if (Value == 0 || !CanMove()) return;
	(Value > 0) ? MoveUp() : MoveDown();	
}

void AMainPlayer::MoveUp()
{
	if (HasWallCollision(FVector(DeltaMovement, 0, 50)))
		return;

	NewLocation = FVector(DeltaMovement, 0, 0) + GetActorLocation();

	MoveFollowers();
	DisableCanMove();
}

void AMainPlayer::MoveDown()
{
	if (HasWallCollision(FVector(-DeltaMovement, 0, 50)))
		return;

	NewLocation = FVector(-DeltaMovement, 0, 0) + GetActorLocation();

	MoveFollowers();
	DisableCanMove();
}

void AMainPlayer::MoveRight()
{
	if (HasWallCollision(FVector(0, DeltaMovement, 50)))
		return;

	NewLocation = FVector(0, DeltaMovement, 0) + GetActorLocation();

	MoveFollowers();
	DisableCanMove();
}

void AMainPlayer::MoveLeft()
{
	if (HasWallCollision(FVector(0, -DeltaMovement, 50)))
		return;

	NewLocation = FVector(0, -DeltaMovement, 0) + GetActorLocation();

	MoveFollowers();
	DisableCanMove();
}

bool AMainPlayer::HasWallCollision(const FVector RelativeLocation) const
{
	TSet<AActor*> Result;
	BoxComponent->SetRelativeLocation(RelativeLocation);
	BoxComponent->GetOverlappingActors(Result);
	return Result.Num() > 0;
}

void AMainPlayer::DisableCanMove()
{
	bCanMove = false;
	GetWorld()->GetTimerManager().SetTimer(CanMove_TimerHandle, this, &AMainPlayer::EnableCanMove, MovementRate, false);
}

void AMainPlayer::EnableCanMove()
{
	bCanMove = true;
	GetWorld()->GetTimerManager().ClearTimer(CanMove_TimerHandle);
}

bool AMainPlayer::CanMove() const { return (NewLocation - GetActorLocation()).Size() == 0 && bCanMove; }

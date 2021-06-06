// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Progressor.h"

#include "Enemy.generated.h"

UCLASS()
class PAWN_API AEnemy : public AActor,  public IProgressor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	FVector NewLocation;
	int CurrentMovement;
	bool bIsMoveForward;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float Speed;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float DeltaMovement;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int NumberMovement;

	virtual void BeginPlay() override;
	bool GetDirection();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnProgress() override;
};

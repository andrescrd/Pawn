// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Progressor.h"
#include "Follower.generated.h"

UCLASS()
class PAWN_API AFollower : public AActor, public IProgressor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollower();

protected:
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	class AActor* OwnTarget;
	UPROPERTY()
	FVector NewLocation;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float Speed;

	bool CanMove() const;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(class AActor* NewTarget);
	virtual void OnProgress() override;
};

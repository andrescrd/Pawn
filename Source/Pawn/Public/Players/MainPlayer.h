// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

UCLASS()
class PAWN_API AMainPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayer();

protected:
	FTimerHandle CanMove_TimerHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleInstanceOnly)
	FVector NewLocation;
	UPROPERTY(VisibleInstanceOnly)	
	bool bCanMove;
	UPROPERTY(EditDefaultsOnly)
	float DeltaMovement;
	UPROPERTY(EditDefaultsOnly)	
	float Speed;
	UPROPERTY(EditDefaultsOnly)
	float MovementRate;

	virtual void BeginPlay() override;

	void MoveHorizontal(float Value);
	void MoveVertical(float Value);

	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

	bool HasWallCollision(const FVector RelativeLocation) const;

	bool CanMove() const;
	void EnableCanMove();
	void DisableCanMove();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

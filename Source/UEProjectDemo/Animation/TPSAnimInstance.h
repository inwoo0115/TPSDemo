// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECTDEMO_API UTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UTPSAnimInstance();
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSceonds) override;
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class AWraith> Owner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float Roll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsAccelerating;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsMontagePlaying;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Axis;
};

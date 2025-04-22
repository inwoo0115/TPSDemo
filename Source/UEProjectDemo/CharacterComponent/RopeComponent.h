// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CableComponent.h"
#include "RopeComponent.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECTDEMO_API URopeComponent : public UCableComponent
{
	GENERATED_BODY()
	
public:
	URopeComponent();

	void UpdateTension();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void OnRegister() override;

	UFUNCTION(BlueprintCallable, Category = "RopeComponent")
	void SetRopeLocation(const FVector& NewLocation);


	UFUNCTION(BlueprintCallable, Category = "RopeComponent")
	void SetIsGrappling(const bool NewIsGrappling);

	UFUNCTION(BlueprintCallable, Category = "RopeComponent")
	bool GetIsGrappling();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class AWraith> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	FVector3d RopeLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope")
	FVector LastVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope")
	FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope")
	FVector CurrentAcceleration;

	bool IsGrappling = false;
};

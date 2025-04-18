// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TPSCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "CableComponent.h"
#include "InputMappingContext.h"
#include "Wraith.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECTDEMO_API AWraith : public ATPSCharacterBase
{
	GENERATED_BODY()
	
public: 
	AWraith();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsAim() const { return IsAim; };

protected:
	
	// �Է� �׼�

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void AimIn(const FInputActionValue& Value);

	void AimOut(const FInputActionValue& Value);

	void Rope(const FInputActionValue& Value);

	void Throw(const FInputActionValue& Value);

	void Reload(const FInputActionValue& Value);

	// �̺�Ʈ �׼�
	virtual void Landed(const FHitResult& Hit) override;

	// �Է� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RopeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputContext, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	// ���� Ÿ�Ӷ���

	FTimeline AimTimeline;

	UFUNCTION()
	void AimUpdate(float Alpha);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
	UCurveFloat* FloatCurve;

	FOnTimelineFloat InterpFunction{};

	// ���� �׼�
	
	FVector3d RopeLocation;

	UPROPERTY(EditAnywhere, Category = "Cable")
	UCableComponent* CableComponent;

	void ImpulseTension();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope")
	FVector LastVelocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope")
	FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope")
	FVector CurrentAcceleration;

	// �÷���

	bool IsRun = false;

	bool IsAttack = false;

	bool IsJump = true;

	bool IsGrappling = false;

	bool IsThrow = true;

	bool IsAim = false;

	// Ÿ�̸�
	FTimerHandle DelayTimeHandle;
};

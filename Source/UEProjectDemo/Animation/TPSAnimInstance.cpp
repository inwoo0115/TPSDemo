// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TPSAnimInstance.h"
#include "GameFramework/Character.h"
#include "Character/Wraith.h"
#include "GameFramework/CharacterMovementComponent.h"

UTPSAnimInstance::UTPSAnimInstance()
{
}

void UTPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AWraith>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSceonds)
{
	Super::NativeUpdateAnimation(DeltaSceonds);

	if (Movement)
	{
		Velocity = Owner->GetVelocity().Length();
		bIsFalling = Movement->IsFalling();
		bIsAim = Owner->GetIsAim();
		bIsAccelerating = Movement->GetCurrentAcceleration().Length() > 0;
		Axis = Owner->GetActorTransform().InverseTransformVector(Owner->GetVelocity().GetSafeNormal(0.0001));

		if (Owner->GetController())
		{
			FRotator ControlRotation = Owner->GetController()->GetControlRotation();

			Yaw = ControlRotation.Yaw;
			Roll = ControlRotation.Roll;
			if (ControlRotation.Pitch > 180.0)
			{
				Pitch = ControlRotation.Pitch - 360.0;
			}
			else
			{
				Pitch = ControlRotation.Pitch;
			}
		}
	}
}

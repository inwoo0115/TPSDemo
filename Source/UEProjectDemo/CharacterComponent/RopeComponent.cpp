// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Wraith.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterComponent/RopeComponent.h"

URopeComponent::URopeComponent()
{
	// 캐릭터, 캐릭터 무브먼트 설정
	Owner = Cast<AWraith>(GetOwner());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	//머터리얼 설정
	static ConstructorHelpers::FObjectFinder<UMaterial> CableMaterial(TEXT("/Game/UtopianCity/Materials/M_HoloMatPainting01.M_HoloMatPainting01"));
	if (CableMaterial.Object)
	{
		SetMaterial(0, CableMaterial.Object);
	}

	// 케이블 설정 초기화
	RopeLocation = FVector(0.0f, 0.0f, 0.0f);
	SolverIterations = 16;
	bEnableStiffness = true;
	CableLength = 10;
	EndLocation = FVector(0.0f, 0.0f, 0.0f);
	SetVisibility(true);
	CableWidth = 3.5f;
}

void URopeComponent::OnRegister()
{
	Super::OnRegister();
	//케이블 부착
	AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Muzzle_03"));
}

void URopeComponent::UpdateTension()
{
	// 장력 + 탄성력
	FVector3d AddForce;

	//매달려 있는 물체와 액터 간의 방향벡터
	FVector3d RopeDirectionVector = RopeLocation - Owner->GetMesh()->GetSocketLocation(TEXT("head"));
	double RopeLength = RopeDirectionVector.Length();

	//특정 거리 이상일 때만 적용
	if (RopeLength > 200.0)
	{
		RopeDirectionVector.Normalize();
		//중력 벡터
		FVector3d GravityVector = FVector3d(0.0f, 0.0f, Movement->GetGravityZ());
		//내적
		double DotAcceleration = RopeDirectionVector | CurrentAcceleration; //가속도
		double DotGravity = RopeDirectionVector | GravityVector;
		double TotalForceScalar = 0.0;
		double ElasticForceScalar = 0.0;
		// 내적 값이 양수 일 경우에 더하기
		if (DotAcceleration < 0.0)
		{
			TotalForceScalar += DotAcceleration;
		}
		if (DotGravity < 0.0)
		{
			TotalForceScalar += DotGravity;
		}
		// 캐릭터 질량 적용
		TotalForceScalar *= -1.0 * Movement->Mass;

		// 특정 거리 이상일 경우 탄성력 적용
		if (RopeLength > 500.0)
		{
			ElasticForceScalar = RopeLength * 500.0;
		}

		// 장력 + 탄성력 적용
		AddForce = (TotalForceScalar + ElasticForceScalar) * RopeDirectionVector;
		Movement->AddForce(AddForce);
	}
}


void URopeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 가속도 계산
	CurrentVelocity = Movement->Velocity;
	CurrentAcceleration = (CurrentVelocity - LastVelocity) / DeltaTime;
	LastVelocity = CurrentVelocity;
	if (IsGrappling)
	{
		UpdateTension();
	}
}

void URopeComponent::SetRopeLocation(const FVector& NewLocation)
{
	RopeLocation = NewLocation;
}

void URopeComponent::SetIsGrappling(const bool NewIsGrappling)
{
	IsGrappling = NewIsGrappling;
}

bool URopeComponent::GetIsGrappling()
{
	return IsGrappling;
}

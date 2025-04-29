// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterComponent/RopeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URopeComponent::URopeComponent()
{
	// ĳ����, ĳ���� �����Ʈ ����
	Owner = Cast<ACharacter>(GetOwner());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	//���͸��� ����
	static ConstructorHelpers::FObjectFinder<UMaterial> CableMaterial(TEXT("/Game/UtopianCity/Materials/M_HoloMatPainting01.M_HoloMatPainting01"));
	if (CableMaterial.Object)
	{
		SetMaterial(0, CableMaterial.Object);
	}

	// ���̺� ���� �ʱ�ȭ
	RopeLocation = FVector(0.0f, 0.0f, 0.0f);
	SolverIterations = 16;
	bEnableStiffness = true;
	CableLength = 10;
	EndLocation = FVector(0.0f, 0.0f, 0.0f);
	SetVisibility(true);
	CableWidth = 3.0f;
}

void URopeComponent::OnRegister()
{
	Super::OnRegister();
	//���̺� ����
	AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Muzzle_03"));
}

void URopeComponent::BeginPlay()
{
	Super::BeginPlay();
	//���� �� ��Ȱ��ȭ
	UnregisterComponent();
}

void URopeComponent::UpdateTension()
{
	// ��� + ź����
	FVector3d AddForce;

	//�Ŵ޷� �ִ� ��ü�� ���� ���� ���⺤��
	FVector3d RopeDirectionVector = RopeLocation - Owner->GetMesh()->GetSocketLocation(TEXT("head"));
	double RopeLength = RopeDirectionVector.Length();

	//Ư�� �Ÿ� �̻��� ���� ����
	if (RopeLength > 200.0)
	{
		RopeDirectionVector.Normalize();
		//�߷� ����
		FVector3d GravityVector = FVector3d(0.0f, 0.0f, Movement->GetGravityZ());
		//����
		double DotAcceleration = RopeDirectionVector | CurrentAcceleration; //���ӵ�
		double DotGravity = RopeDirectionVector | GravityVector;
		double TotalForceScalar = 0.0;
		double ElasticForceScalar = 0.0;
		// ���� ���� ��� �� ��쿡 ���ϱ�
		if (DotAcceleration < 0.0)
		{
			TotalForceScalar += DotAcceleration;
		}
		if (DotGravity < 0.0)
		{
			TotalForceScalar += DotGravity;
		}
		// ĳ���� ���� ����
		TotalForceScalar *= -1.0 * Movement->Mass;

		// Ư�� �Ÿ� �̻��� ��� ź���� ����
		if (RopeLength > 100.0)
		{
			ElasticForceScalar = RopeLength * 80.0;
		}

		// ��� + ź���� ����
		AddForce = (TotalForceScalar + ElasticForceScalar) * RopeDirectionVector;
		Movement->AddForce(AddForce);
	}
}


void URopeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���ӵ� ���
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

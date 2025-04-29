// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATPSCharacterBase::ATPSCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��Ʈ�ѷ��� ȸ���� �޾Ƽ� �����ϴ� ���
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	//�����Ʈ ����
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->bEnablePhysicsInteraction = false;

	//�̴� ���� ����
	JumpMaxCount = 2;

	//ĸ�� ������Ʈ ����
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	//Mesh ��ġ ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
}

// Called when the game starts or when spawned
void ATPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


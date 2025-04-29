// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATPSCharacterBase::ATPSCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컨트롤러의 회전을 받아서 설정하는 모드
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	//무브먼트 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->bEnablePhysicsInteraction = false;

	//이단 점프 설정
	JumpMaxCount = 2;

	//캡슐 컴포넌트 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	//Mesh 위치 설정
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


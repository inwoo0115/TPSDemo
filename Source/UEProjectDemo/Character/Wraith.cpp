// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Wraith.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Props/RopeHolder.h"
#include "CharacterComponent/RopeComponent.h"

// 캐릭터 기본 설정
AWraith::AWraith()
{
	//컨트롤러의 회전을 받아서 설정하는 모드
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	//무브먼트 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//캡슐 컴포넌트 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	//Mesh 위치 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	//리소스 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith"));
	if (CharacterMesh.Object)
	{
		// Mesh컴포넌트에 접근
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/Demo/Blueprint/ABP_Wraith.ABP_Wraith_C"));
	if (CharacterAnim.Class)
	{
		GetMesh()->SetAnimClass(CharacterAnim.Class);
	}

	//Camera, Spring Arm 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(FVector(0.0f, 50.0f, 80.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//Input 설정
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/Demo/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Demo/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Demo/Input/Actions/IA_Look.IA_Look"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Game/Demo/Input/Actions/IA_Run.IA_Run"));
	if (RunActionRef.Object)
	{
		RunAction = RunActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/Demo/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(TEXT("/Game/Demo/Input/Actions/IA_Aim.IA_Aim"));
	if (AimActionRef.Object)
	{
		AimAction = AimActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> RopeActionRef(TEXT("/Game/Demo/Input/Actions/IA_Rope.IA_Rope"));
	if (RopeActionRef.Object)
	{
		RopeAction = RopeActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ThrowActionRef(TEXT("/Game/Demo/Input/Actions/IA_Throw.IA_Throw"));
	if (ThrowActionRef.Object)
	{
		ThrowAction = ThrowActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionRef(TEXT("/Game/Demo/Input/Actions/IA_Reload.IA_Reload"));
	if (ReloadActionRef.Object)
	{
		ReloadAction = ReloadActionRef.Object;
	}
	
	// Input Mapping Context 가져오기
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Game/Demo/Input/IMC_Combat.IMC_Combat"));
	if (IMCRef.Object)
	{
		InputMappingContext = IMCRef.Object;
	}

	// 캐릭터 Physic 설정
	GetCharacterMovement()->Mass = 500;

	//케이블 설정
	RopeComponent = CreateDefaultSubobject<URopeComponent>(TEXT("RopeComponent"));

	//케이블 부착 AnimMontage 설정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RopeMontageRef(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Animations/Ability_Q_Fire_Montage.Ability_Q_Fire_Montage"));
	if (RopeMontageRef.Object)
	{
		RopeMontage = RopeMontageRef.Object;
	}
}

void AWraith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Binding
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWraith::Jump);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWraith::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWraith::Look);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AWraith::Run);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWraith::Attack);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AWraith::AimIn);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AWraith::AimOut);
	EnhancedInputComponent->BindAction(RopeAction, ETriggerEvent::Triggered, this, &AWraith::Rope);
	EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &AWraith::Throw);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AWraith::Reload);
}

void AWraith::BeginPlay()
{
	Super::BeginPlay();

	// Aim 타임라인
	FloatCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass());
	FRichCurve* RichCurve = &FloatCurve->FloatCurve;
	RichCurve->AddKey(0.0f, 300.0f);
	RichCurve->AddKey(0.1f, 40.0f);
	InterpFunction.BindUFunction(this, FName("AimUpdate"));
	AimTimeline.AddInterpFloat(FloatCurve, InterpFunction);
	AimTimeline.SetLooping(false);
	AimTimeline.SetPlayRate(1.0f);

	//Input system mapping
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AWraith::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타임라인 tick 갱신
	AimTimeline.TickTimeline(DeltaTime);
}

void AWraith::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	// 컨트롤러 회전 값
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	
	// 방향 구하기.
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트 컴포넌트에 값 전달.
	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RightVector, Movement.Y);
}

void AWraith::Look(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	
	// 시점 회전
	AddControllerPitchInput(Movement.Y * -1);
	AddControllerYawInput(Movement.X);
}

void AWraith::Jump(const FInputActionValue& Value)
{
	if (IsJump)
	{
		Super::Jump();
	}
}

void AWraith::Run(const FInputActionValue& Value)
{
	if (IsRun)
	{
		IsRun = false;
		GetCharacterMovement()->MaxWalkSpeed -= 300.0;
	}
	else
	{
		IsRun = true;
		GetCharacterMovement()->MaxWalkSpeed += 300.0;
	}
}

void AWraith::AimIn(const FInputActionValue& Value)
{
	if (FloatCurve)
	{
		AimTimeline.Play();
	}
	IsAim = true;
}

void AWraith::AimOut(const FInputActionValue& Value)
{
	if (FloatCurve)
	{
		AimTimeline.Reverse();
	}
	IsAim = false;
}

void AWraith::AimUpdate(float Alpha)
{
	SpringArm->TargetArmLength = Alpha;
}

void AWraith::Rope(const FInputActionValue& Value)
{
	if (RopeComponent->GetIsGrappling())
	{
		RopeComponent->SetIsGrappling(false);
		RopeComponent->UnregisterComponent();
	}
	else
	{
		FVector3d StartLocation = GetActorLocation();
		FVector3d DirectionVector = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
		FHitResult HitResult;
		bool bHit = GetWorld()->SweepSingleByProfile(
			HitResult,
			StartLocation + DirectionVector * 50,
			StartLocation + DirectionVector * 2500,
			FQuat::Identity,
			TEXT("InteractionTrace"),
			FCollisionShape::MakeSphere(200.0f)
		);

		if (bHit)
		{
			//Rope holder로 캐스트 
			ARopeHolder* RopeHolder = Cast<ARopeHolder>(HitResult.GetActor());
			if (RopeHolder)
			{
				RopeComponent->SetIsGrappling(true);
				RopeComponent->SetRopeLocation(RopeHolder->GetActorLocation());
				// 물체와 플레이어 잇는 케이블 설치 
				RopeComponent->RegisterComponent();
				RopeComponent->SetAttachEndTo(RopeHolder, TEXT("SphereMesh"));
				PlayAnimMontage(RopeMontage);
			}
		}
	}
}

void AWraith::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	IsJump = false;

	// 바닥 착지 후 0.5초 후 점프 가능
	GetWorld()->GetTimerManager().SetTimer(
		DelayTimeHandle,
		[this]()
		{
			IsJump = true;
		},
		0.5f,
		false
		);
}


void AWraith::Throw(const FInputActionValue& Value)
{
}

void AWraith::Attack(const FInputActionValue& Value)
{
}

void AWraith::Reload(const FInputActionValue& Value)
{
}
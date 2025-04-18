// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Wraith.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "CableComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Props/RopeHolder.h"

// ĳ���� �⺻ ����
AWraith::AWraith()
{
	//��Ʈ�ѷ��� ȸ���� �޾Ƽ� �����ϴ� ���
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	//�����Ʈ ����
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//ĸ�� ������Ʈ ����
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	//Mesh ��ġ ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	//���ҽ� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith"));
	if (CharacterMesh.Object)
	{
		// Mesh������Ʈ�� ����
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/Demo/Blueprint/ABP_Wraith.ABP_Wraith_C"));
	if (CharacterAnim.Class)
	{
		GetMesh()->SetAnimClass(CharacterAnim.Class);
	}

	//���̺� ����
	CableComponent = nullptr;

	//Camera, Spring Arm ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(FVector(0.0f, 50.0f, 80.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//Input ����
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
	
	// Input Mapping Context ��������
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Game/Demo/Input/IMC_Combat.IMC_Combat"));
	if (IMCRef.Object)
	{
		InputMappingContext = IMCRef.Object;
	}

	// ĳ���� Physic ����
	GetCharacterMovement()->Mass = 500;
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

	// Ÿ�Ӷ��� �ʱ�ȭ
	FloatCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass());
	FRichCurve* RichCurve = &FloatCurve->FloatCurve;
	RichCurve->AddKey(0.0f, 300.0f);
	RichCurve->AddKey(0.2f, 40.0f);
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

	// Ÿ�Ӷ��� tick ����
	AimTimeline.TickTimeline(DeltaTime);

	// �ٿ� �Ŵ޷� ���� ��� �� ����
	CurrentVelocity = GetCharacterMovement()->Velocity;
	CurrentAcceleration = (CurrentVelocity - LastVelocity) / DeltaTime;
	LastVelocity = CurrentVelocity;
	ImpulseTension();
}

void AWraith::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	// ��Ʈ�ѷ� ȸ�� ��
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	
	// ���� ���ϱ�.
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// �����Ʈ ������Ʈ�� �� ����.
	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RightVector, Movement.Y);
}

void AWraith::Look(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	
	// ���� ȸ��
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

void AWraith::Attack(const FInputActionValue& Value)
{
	
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
	if (IsGrappling)
	{
		IsGrappling = false;
		// ���̺� ����
		if (CableComponent)
		{
			CableComponent->DestroyComponent();
			CableComponent = nullptr;
		}
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
			//Rope holder�� ĳ��Ʈ 
			ARopeHolder* RopeHolder = Cast<ARopeHolder>(HitResult.GetActor());
			if (RopeHolder)
			{
				IsGrappling = true;
				RopeLocation = RopeHolder->GetActorLocation();
				// ��ü�� �÷��̾� �մ� ���̺� ��ġ 
				// TODO: ���߿� �̰� �̸� ����°ɷ� �ٲ���ϳ�...?
				CableComponent = NewObject<UCableComponent>(this, UCableComponent::StaticClass());
				CableComponent->RegisterComponent();
				CableComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Muzzle_03"));
				CableComponent->SetAttachEndTo(RopeHolder, TEXT("SphereMesh"));
				CableComponent->SolverIterations = 16;
				CableComponent->bEnableStiffness = true;
				CableComponent->CableLength = 10;
				CableComponent->EndLocation = FVector(0.0f, 0.0f, 0.0f);
				CableComponent->SetVisibility(true);
			}
		}
	}
}


// ��� ź���� ����
void AWraith::ImpulseTension()
{
	if (IsGrappling)
	{
		// ��� + ź����
		FVector3d AddForce;

		//�Ŵ޷� �ִ� ��ü�� ���� ���� ���⺤��
		FVector3d RopeDirectionVector = RopeLocation - GetMesh()->GetSocketLocation(TEXT("head"));
		double RopeLength = RopeDirectionVector.Length();

		//Ư�� �Ÿ� �̻��� ���� ����
		if (RopeLength > 200.0)
		{
			RopeDirectionVector.Normalize();
			//�߷� ����
			FVector3d GravityVector = FVector3d(0.0f, 0.0f, GetCharacterMovement()->GetGravityZ());
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

			// �߰� ���ӵ� ����
			TotalForceScalar *= -1.0 * GetCharacterMovement()->Mass;

			// Ư�� �Ÿ� �̻��� ��� ź���� ����
			if (RopeLength > 500.0)
			{
				ElasticForceScalar = RopeLength * 500.0;
			}

			// ��� + ź���� ����
			AddForce = (TotalForceScalar + ElasticForceScalar) * RopeDirectionVector;
			GetCharacterMovement()->AddForce(AddForce);
		}
	}
}


void AWraith::Throw(const FInputActionValue& Value)
{
	// Anim Montage ����
	if (IsThrow)
	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowMontage(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Animations/Ability_E_Montage.Ability_E_Montage"));
		if (ThrowMontage.Object)
		{
			PlayAnimMontage(ThrowMontage.Object, 1.0f);
		}

		//2�� ��Ÿ�� ����
		IsThrow = false;
		GetWorld()->GetTimerManager().SetTimer(
			DelayTimeHandle,
			[this]()
			{
				IsThrow = true;
			},
			2.0f,
			false
		);
		
	}
	// TODO : ����ź ���� ���� �߰�
}

void AWraith::Reload(const FInputActionValue& Value)
{
}

void AWraith::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	IsJump = false;

	// �ٴ� ���� �� 0.5�� �� ���� ����
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

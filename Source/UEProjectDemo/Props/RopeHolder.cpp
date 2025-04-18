// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/RopeHolder.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ARopeHolder::ARopeHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision Component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->InitSphereRadius(50.0f);
	SphereCollision->SetCollisionProfileName(TEXT("InteractionProfile"));

	// Mesh Component
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere")); // 기본 엔진 구체
	if (MeshAsset.Object)
	{
		SphereMesh->SetStaticMesh(MeshAsset.Object);
		SphereMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		SphereMesh->SetWorldScale3D(FVector(1.f));
		SphereMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}
}

// Called when the game starts or when spawned
void ARopeHolder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARopeHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayerController.h"
#include "TPSPlayerController.h"

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작할 때 마우스가 뷰포트로 바로 입력되게
	FInputModeGameOnly GameInput;

	SetInputMode(GameInput);
}

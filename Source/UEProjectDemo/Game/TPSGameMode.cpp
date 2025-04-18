// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"

ATPSGameMode::ATPSGameMode()
{
	//GameMode default setting

	//Default Pawn class setting
	static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(TEXT("/Script/UEProjectDemo.Wraith"));
	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	//Default Controller Class setting
	static ConstructorHelpers::FClassFinder<APlayerController> DefaultControllerRef(TEXT("/Script/UEProjectDemo.TPSPlayerController"));
	if (DefaultControllerRef.Class)
	{
		PlayerControllerClass = DefaultControllerRef.Class;
	}
}

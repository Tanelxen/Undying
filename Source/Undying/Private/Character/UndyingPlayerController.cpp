// Fill out your copyright notice in the Description page of Project Settings.

#include "Undying.h"

void AUndyingPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis( "MoveForward", this, &AUndyingPlayerController::MoveForward );
	InputComponent->BindAxis( "MoveBackward", this, &AUndyingPlayerController::MoveBackward );
	InputComponent->BindAxis( "MoveLeft", this, &AUndyingPlayerController::MoveLeft );
	InputComponent->BindAxis( "MoveRight", this, &AUndyingPlayerController::MoveRight );

	InputComponent->BindAction( "Jump", IE_Pressed, this, &AUndyingPlayerController::Jump );

	InputComponent->BindAction( "Crouch", IE_Pressed, this, &AUndyingPlayerController::Crouch );
	InputComponent->BindAction( "Crouch", IE_Released, this, &AUndyingPlayerController::UnCrouch );

	InputComponent->BindAxis( "Turn", this, &APlayerController::AddYawInput );
	InputComponent->BindAxis( "LookUp", this, &APlayerController::AddPitchInput );

	InputComponent->BindAction( "PrevWeapon", IE_Pressed, this, &AUndyingPlayerController::PrevWeapon );
	InputComponent->BindAction( "NextWeapon", IE_Released, this, &AUndyingPlayerController::NextWeapon );

	InputComponent->BindAction( "StartFire", IE_Pressed, this, &AUndyingPlayerController::OnFire );
	InputComponent->BindAction( "StopFire", IE_Released, this, &AUndyingPlayerController::OnStopFire );

	//InputComponent->BindAction( "ShowMenu", IE_Released, this, &AUndyingPlayerController::execShowMenu );
}

void AUndyingPlayerController::SetPawn( APawn* InPawn )
{
	AController::SetPawn( InPawn );

	MyCharacter = Cast<AUndyingCharacter>( InPawn );
}

AUndyingCharacter* AUndyingPlayerController::GetUndyingCharacter()
{
	return MyCharacter;
}

void AUndyingPlayerController::PrevWeapon()
{
	if( MyCharacter != NULL )
	{
		MyCharacter->PrevWeapon();
	}
}

void AUndyingPlayerController::NextWeapon()
{
	if( MyCharacter != NULL )
	{
		MyCharacter->NextWeapon();
	}
}

void AUndyingPlayerController::OnFire()
{
	if( MyCharacter != NULL )
	{
		MyCharacter->FireWeapon();
	}
}

void AUndyingPlayerController::OnStopFire()
{
}

void AUndyingPlayerController::MoveForward( float Value )
{
	if( Value != 0.0f && MyCharacter != NULL )
	{
		MyCharacter->MoveForward( Value );
	}
}

void AUndyingPlayerController::MoveBackward( float Value )
{
	MoveForward( Value * -1 );
}

void AUndyingPlayerController::MoveRight( float Value )
{
	if( Value != 0.0f && MyCharacter != NULL )
	{
		MyCharacter->MoveRight( Value );
	}
}

void AUndyingPlayerController::MoveLeft( float Value )
{
	MoveRight( Value * -1 );
}

void AUndyingPlayerController::Jump()
{
	if( MyCharacter != NULL )
	{
		MyCharacter->Jump();
	}
}

void AUndyingPlayerController::Crouch()
{
}

void AUndyingPlayerController::UnCrouch()
{
}

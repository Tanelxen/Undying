// Fill out your copyright notice in the Description page of Project Settings.

#include "Undying.h"
#include "UTCharacter.h"


// Sets default values
AUTCharacter::AUTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUTCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AUTCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}


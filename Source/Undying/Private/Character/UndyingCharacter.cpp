// Fill out your copyright notice in the Description page of Project Settings.

#include "Undying.h"
#include "UndyingCharacter.h"

#include "Weapon.h"

#include "EngineUtils.h"

// Sets default values
AUndyingCharacter::AUndyingCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize( 42.0f, 96.0f );

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT("PlayerCamera") );
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector( 0, 0, 64.0f ); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT("PlayerMesh") );
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
}

USkeletalMeshComponent* AUndyingCharacter::GetMesh()
{
	return Mesh1P;
}

// Called when the game starts or when spawned
void AUndyingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GiveDefaultWeapon();
}

void AUndyingCharacter::MoveForward( float Value )
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput( Direction, Value );
	}
}

void AUndyingCharacter::MoveRight( float Value )
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput( Direction, Value );
	}
}


void AUndyingCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	InputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	InputComponent->BindAxis( "MoveForward", this, &AUndyingCharacter::MoveForward );
	InputComponent->BindAxis( "MoveRight", this, &AUndyingCharacter::MoveRight );
	InputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );

	InputComponent->BindAction( "Fire", IE_Pressed, this, &AUndyingCharacter::FireWeapon );
	InputComponent->BindAction( "NextWeapon", IE_Pressed, this, &AUndyingCharacter::NextWeapon );
	InputComponent->BindAction( "PrevWeapon", IE_Pressed, this, &AUndyingCharacter::PrevWeapon );

	InputComponent->BindAction( "PrintActors", IE_Pressed, this, &AUndyingCharacter::PrintActors );
}

void AUndyingCharacter::GiveDefaultWeapon()
{
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>( WeaponSpawn );

	if( Spawner )
	{
		CurrentWeapon = Spawner;
		CurrentWeapon->SetOwningPawn( this );
		CurrentWeapon->OnEquip();
	}
}

void AUndyingCharacter::FireWeapon()
{
	if( CurrentWeapon != NULL )
	{
		CurrentWeapon->Fire();
	}
}

void AUndyingCharacter::NextWeapon()
{
}

void AUndyingCharacter::PrevWeapon()
{
}

void AUndyingCharacter::EquipWeapon( AWeapon *Weapon )
{
	if( CurrentWeapon != NULL )
	{
		CurrentWeapon->OnUnEquip();

		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn( this );
		Weapon->OnEquip();
	}
	else
	{
		CurrentWeapon = Weapon;
		CurrentWeapon->SetOwningPawn( this );
		Weapon->OnEquip();
	}
}

void AUndyingCharacter::OnCollision( AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult )
{
	AWeapon *Weapon = Cast<AWeapon>( OtherActor );

	if( Weapon )
	{
		ProcessWeaponPickup( Weapon );
	}
}

void AUndyingCharacter::ProcessWeaponPickup( AWeapon *Weapon )
{
	if( Weapon == NULL ) return;

	Weapon->Destroy();
}

void AUndyingCharacter::PrintActors()
{
	APlayerController* PlayerController = Cast<APlayerController>( this->Controller );
	
	for( TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr )
	{
		GEngine->AddOnScreenDebugMessage( -1, 20.0, FColor::Blue, ActorItr->GetName() );

		PlayerController->ClientMessage( ActorItr->GetName() );
		PlayerController->ClientMessage( ActorItr->GetActorLocation().ToString() );
	}
}
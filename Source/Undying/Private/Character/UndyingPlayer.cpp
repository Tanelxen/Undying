// Fill out your copyright notice in the Description page of Project Settings.

#include "Undying.h"
#include "UndyingPlayer.h"

#include "Weapon.h"

#include "EngineUtils.h"

// Sets default values
AUndyingPlayer::AUndyingPlayer()
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

USkeletalMeshComponent* AUndyingPlayer::GetMesh()
{
	return Mesh1P;
}

// Called when the game starts or when spawned
void AUndyingPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GiveDefaultWeapon();
}

void AUndyingPlayer::MoveForward( float Value )
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput( Direction, Value );
	}
}

void AUndyingPlayer::MoveRight( float Value )
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput( Direction, Value );
	}
}


void AUndyingPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	InputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	InputComponent->BindAxis( "MoveForward", this, &AUndyingPlayer::MoveForward );
	InputComponent->BindAxis( "MoveRight", this, &AUndyingPlayer::MoveRight );
	InputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );

	InputComponent->BindAction( "Fire", IE_Pressed, this, &AUndyingPlayer::FireWeapon );
	InputComponent->BindAction( "NextWeapon", IE_Pressed, this, &AUndyingPlayer::NextWeapon );
	InputComponent->BindAction( "PrevWeapon", IE_Pressed, this, &AUndyingPlayer::PrevWeapon );

	InputComponent->BindAction( "PrintActors", IE_Pressed, this, &AUndyingPlayer::PrintActors );
}

void AUndyingPlayer::GiveDefaultWeapon()
{
	//AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>( WeaponSpawn );

	//if( Spawner )
	//{
	//	CurrentWeapon = Spawner;
	//	CurrentWeapon->SetOwningPawn( this );
	//	CurrentWeapon->OnEquip();
	//}
}

void AUndyingPlayer::FireWeapon()
{
	if( CurrentWeapon != NULL )
	{
		CurrentWeapon->Fire();
	}
}

void AUndyingPlayer::NextWeapon()
{
}

void AUndyingPlayer::PrevWeapon()
{
}

void AUndyingPlayer::EquipWeapon( AWeapon *Weapon )
{
	//if( CurrentWeapon != NULL )
	//{
	//	CurrentWeapon->OnUnEquip();

	//	CurrentWeapon = Weapon;
	//	Weapon->SetOwningPawn( this );
	//	Weapon->OnEquip();
	//}
	//else
	//{
	//	CurrentWeapon = Weapon;
	//	CurrentWeapon->SetOwningPawn( this );
	//	Weapon->OnEquip();
	//}
}

void AUndyingPlayer::OnCollision( AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult )
{
	AWeapon *Weapon = Cast<AWeapon>( OtherActor );

	if( Weapon )
	{
		ProcessWeaponPickup( Weapon );
	}
}

void AUndyingPlayer::ProcessWeaponPickup( AWeapon *Weapon )
{
	if( Weapon == NULL ) return;

	Weapon->Destroy();
}

void AUndyingPlayer::PrintActors()
{
	APlayerController* PlayerController = Cast<APlayerController>( this->Controller );
	
	for( TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr )
	{
		GEngine->AddOnScreenDebugMessage( -1, 20.0, FColor::Blue, ActorItr->GetName() );

		PlayerController->ClientMessage( ActorItr->GetName() );
		PlayerController->ClientMessage( ActorItr->GetActorLocation().ToString() );
	}
}
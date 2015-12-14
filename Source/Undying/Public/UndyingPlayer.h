// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "UndyingPlayer.generated.h"

class AWeapon;

UCLASS( config = Game )
class UNDYING_API AUndyingPlayer : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY( VisibleDefaultsOnly, Category=Mesh )
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true") )
	class UCameraComponent* FirstPersonCameraComponent;

	AUndyingPlayer();

	UFUNCTION( BlueprintCallable, Category = Event )
	virtual void BeginPlay() override;


	UFUNCTION()
	void FireWeapon();

	UPROPERTY( EditDefaultsOnly, Category = DefaultInv )
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = Weapon )
	AWeapon *CurrentWeapon;

	UFUNCTION()
	void OnCollision( AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

	void ProcessWeaponPickup( AWeapon *Weapon );

	void NextWeapon();
	void PrevWeapon();

	void EquipWeapon( AWeapon *Weapon );
	void GiveDefaultWeapon();

	USkeletalMeshComponent* GetMesh();

public:

	void MoveForward( float Value );
	void MoveRight( float Value );

	void PrintActors();

protected:

	virtual void SetupPlayerInputComponent( class UInputComponent* InputComponent ) override;
};

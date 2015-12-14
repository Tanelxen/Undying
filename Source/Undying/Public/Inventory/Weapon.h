// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImpactEffect.h"

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AUndyingCharacter;

#define TRACE_WEAPON ECC_GameTraceChannel1

/**
 * 
 */

UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet			UMETA(DisplayName = "Bullet"),
		ESpread			UMETA(DisplayName = "Spread"),
		EProjectile		UMETA(DisplayName = "Projectile")
	};
}


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 ShotCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Priority;
};


UCLASS()
class UNDYING_API AWeapon : public AActor
{
	GENERATED_UCLASS_BODY()

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Instant_Fire();

	UFUNCTION()
	virtual void ProjectileFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent *CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *FireSound;

	UPROPERTY( EditDefaultsOnly, Category=Effects )
	FName MuzzleAttachPoint;

	UPROPERTY( EditDefaultsOnly, Category=Effects )
	UParticleSystem* MuzzleFX;

	UPROPERTY( Transient )
	UParticleSystemComponent* MuzzlePSC;

	UPROPERTY( EditDefaultsOnly, Category=Effects )
	TSubclassOf<AImpactEffect> ImpactTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentClip;

	void SetOwningPawn(AUndyingCharacter *NewOwner);

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnEquip();

	void ReloadAmmo();

	UAudioComponent* PlayWeaponSound(USoundCue *Sound);

protected:

	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	void SpawnImpactEffects( const FHitResult& Impact );

	AUndyingCharacter *MyPawn;

	virtual FVector GetAdjustedAim() const;
};
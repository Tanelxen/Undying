// Fill out your copyright notice in the Description page of Project Settings.

#include "Undying.h"
#include "Weapon.h"

#include "UndyingPlayer.h"

AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{


	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->SetRelativeRotation(FRotator(0, 180, 0), false);
	WeaponMesh->AttachTo(RootComponent);
	
}

void AWeapon::Fire()
{
	if (ProjectileType == EWeaponProjectile::EBullet)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Bullet"));
			Instant_Fire();
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}
	if (ProjectileType == EWeaponProjectile::ESpread)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Spread"));
			for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
			{
				Instant_Fire();
			}
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}
	if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));
			ProjectileFire();
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}
}

void AWeapon::SetOwningPawn( AUndyingPlayer *NewOwner )
{
	if( MyPawn != NewOwner )
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::OnEquip()
{
	CollisionComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	AttachToPlayer();
}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

void AWeapon::AttachToPlayer()
{
	if( MyPawn )
	{
		DetachFromPlayer();

		USkeletalMeshComponent *Character = MyPawn->GetMesh();
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachTo(Character, "Weapon_Socket");
	}
}

void AWeapon::DetachFromPlayer()
{
	WeaponMesh->DetachFromParent();
	WeaponMesh->SetHiddenInGame(true);
}

void AWeapon::ReloadAmmo()
{
	if (CurrentAmmo > 0)
	{
		if (CurrentAmmo < WeaponConfig.MaxClip)
		{
			CurrentClip = CurrentAmmo;
		}
		else
		{
			CurrentAmmo -= WeaponConfig.MaxClip;
			CurrentClip += WeaponConfig.MaxClip;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Blue, "NO AMMO");
	}
}

void AWeapon::Instant_Fire()
{
		const int32 RandomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomSeed);
		const float CurrentSpread = WeaponConfig.WeaponSpread;
		const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);


		APlayerController* PlayerController = Cast<APlayerController>( MyPawn->Controller );

		FVector CamLoc;
		FRotator CamRot;

		PlayerController->GetPlayerViewPoint( CamLoc, CamRot );

		const FVector AimDir = CamRot.Vector();

		const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);

		const FVector StartTrace = CamLoc;
		const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;

		const FHitResult Impact = WeaponTrace( StartTrace, EndTrace );

		ProcessInstantHit( Impact, StartTrace, EndTrace, RandomSeed, CurrentSpread );


		//if( MuzzleFX )
		//{
		//	if( MuzzlePSC == NULL)
		//	{
		//		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached( MuzzleFX, WeaponMesh, MuzzleAttachPoint );
		//		MuzzlePSC->bOwnerNoSee = false;
		//		MuzzlePSC->bOnlyOwnerSee = true;					
		//	}
		//}
}

FHitResult AWeapon::WeaponTrace( const FVector &TraceFrom, const FVector &TraceTo ) const
{
	FCollisionQueryParams TraceParams( TEXT("WeaponTrace"), true, Instigator );
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor( this );

	FHitResult Hit( ForceInit );
	GetWorld()->LineTraceSingleByChannel( Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams );

	return Hit;
}

void AWeapon::ProcessInstantHit( const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread )
{
	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;


	//const FVector AimDir = WeaponMesh->GetSocketRotation(MuzzleAttachPoint).Vector();
	const FVector StartTrace = WeaponMesh->GetSocketLocation(MuzzleAttachPoint);


	DrawDebugLine( this->GetWorld(), StartTrace, EndPoint, FColor::Green, true, 2.0f, 10.0f, 1.0f );

	SpawnImpactEffects( Impact );
}

void AWeapon::SpawnImpactEffects( const FHitResult& Impact )
{
	if( ImpactTemplate && Impact.bBlockingHit )
	{
		FHitResult UseImpact = Impact;

		// trace again to find component lost during replication
		if (!Impact.Component.IsValid())
		{
			const FVector StartTrace = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;
			const FVector EndTrace = Impact.ImpactPoint - Impact.ImpactNormal * 10.0f;
			FHitResult Hit = WeaponTrace( StartTrace, EndTrace );
			UseImpact = Hit;
		}

		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint);

		AImpactEffect* EffectActor = GetWorld()->SpawnActorDeferred<AImpactEffect>(ImpactTemplate, SpawnTransform);

		if( EffectActor )
		{
			EffectActor->SurfaceHit = UseImpact;
			UGameplayStatics::FinishSpawningActor( EffectActor, SpawnTransform );
		}
	}
}

void AWeapon::ProjectileFire()
{
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue *Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::SpawnSoundAttached( Sound, MyPawn->GetRootComponent() );
	}

	return AC;
}


FVector AWeapon::GetAdjustedAim() const
{
	APlayerController* PlayerController = MyPawn ? Cast<APlayerController>( MyPawn->Controller ) : NULL;

	FVector FinalAim = FVector::ZeroVector;

	// If we have a player controller use it for the aim
	if( PlayerController )
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}

	return FinalAim;
}

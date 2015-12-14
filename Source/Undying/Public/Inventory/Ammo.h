// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ammo.generated.h"


UCLASS( abstract )
class UNDYING_API AAmmo : public APickup
{
	GENERATED_BODY()

public:

	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int AmmoAmount;

	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int MaxAmount;

	bool HandlePickupQuery( APickup Item ) override
	{
		if( Item->IsA( this::StaticClass() ) )
		{
			AddAmmo( AAmmo(Item).AmmoAmount );

			return true;
		}

		return false;
	}

	bool AddAmmo( int AmmoToAdd )
	{
		if( AmmoAmount >= MaxAmmo ) return false;

		AmmoAmount += AmmoToAdd;

		if( AmmoAmount > MaxAmmo )
		{
			AmmoAmount = MaxAmmo;
		}

		return true;
	}

	bool UseAmmo( int AmountNeeded )
	{
		if( AmmoAmount < AmountNeeded ) return false;

		AmmoAmount -= AmountNeeded;

		return true;
	}
};
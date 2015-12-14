// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/Pickup.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS( abstract )
class UNDYING_API AAmmo : public APickup
{
	GENERATED_BODY()

public:

	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int32 AmmoAmount;

	UPROPERTY( EditDefaultsOnly, Category = "Ammo" )
	int32 MaxAmount;

	bool HandlePickupQuery( APickup* Item );

	bool AddAmmo( int AmmoToAdd );

	bool UseAmmo( int AmountNeeded );
};

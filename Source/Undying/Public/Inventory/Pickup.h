// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


UCLASS( abstract )
class UNDYING_API APickup : public AInventory
{
	GENERATED_BODY()

public:

	UPROPERTY( EditDefaultsOnly, Category = "Pickup" )
	bool bCanHaveMultipleCopies;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup" )
	bool bCanActivate;

	bool HandlePickupQuery( APickup Item )
	{
		if( Item->IsA( this::StaticClass() ) )
		{
			if( bCanHaveMultipleCopies )
			{
				NumCopies++;
			}

			return true;
		}

		return false;
	}

protected:

	int NumCopies;
};
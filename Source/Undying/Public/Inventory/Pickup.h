// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/Inventory.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS( abstract )
class UNDYING_API APickup : public AInventory
{
	GENERATED_BODY()

public:
	APickup();
	
	UPROPERTY( EditDefaultsOnly, Category = "Pickup" )
	bool bCanHaveMultipleCopies;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup" )
	bool bCanActivate;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Pickup" )
	UBoxComponent *CollisionComp;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup" )
	class UStaticMeshComponent* PickupMesh;

	bool HandlePickupQuery( APickup* Item );


	int32 NumCopies;
};

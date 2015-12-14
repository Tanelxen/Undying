// Fill out your copyright notice in the Description page of Project Settings.

#include "Undying.h"
//#include "Pickup.h"

APickup::APickup()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>( TEXT("CollisionComp") );
	RootComponent = CollisionComp;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT("PickupMesh") );
	PickupMesh->AttachTo( RootComponent );
}

bool APickup::HandlePickupQuery( APickup* Item )
{
	if( Item->IsA( APickup::StaticClass() ) )
	{
		if( bCanHaveMultipleCopies )
		{
			NumCopies++;
		}

		return true;
	}

	return false;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Undying.h"


bool AAmmo::HandlePickupQuery( APickup* Item )
{
	if( Item->IsA( AAmmo::StaticClass() ) )
	{
		//AddAmmo( AAmmo(Item).AmmoAmount );

		return true;
	}

	return false;
}

bool AAmmo::AddAmmo( int AmmoToAdd )
{
	if( AmmoAmount >= MaxAmount ) return false;

	AmmoAmount += AmmoToAdd;

	if( AmmoAmount > MaxAmount )
	{
		AmmoAmount = MaxAmount;
	}

	return true;
}

bool AAmmo::UseAmmo( int AmountNeeded )
{
	if( AmmoAmount < AmountNeeded ) return false;

	AmmoAmount -= AmountNeeded;

	return true;
}


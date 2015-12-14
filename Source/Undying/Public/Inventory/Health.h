// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/Pickup.h"
#include "Health.generated.h"

/**
 * 
 */
UCLASS( abstract )
class UNDYING_API AHealth : public APickup
{
	GENERATED_BODY()

public:
	
	UPROPERTY( EditDefaultsOnly, Category = "Health" )
	int32 HealingAmount;
	
	void Activate();
};

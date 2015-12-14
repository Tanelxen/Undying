// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

UCLASS( abstract )
class UNDYING_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	//AInventory();

	/** name displayed on the HUD*/
	UPROPERTY( EditDefaultsOnly, Category = "HUD" )
	FName ItemName;

	/** icon displayed on the HUD*/
	UPROPERTY( EditDefaultsOnly, Category = "HUD" )
	FCanvasIcon ItemIcon;
};

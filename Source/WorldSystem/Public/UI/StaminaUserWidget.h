// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSYSTEM_API UStaminaUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateStaminaBar(float StaminaPercent);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* StaminaBar;
};

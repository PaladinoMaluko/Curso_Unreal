// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StaminaUserWidget.h"
#include "Components/ProgressBar.h"

void UStaminaUserWidget::UpdateStaminaBar(float StaminaPercent)
{
	if (StaminaBar) {
		StaminaBar->SetPercent(StaminaPercent);
		FLinearColor color = FLinearColor(120.f * StaminaPercent, 1, 1);
		StaminaBar->SetFillColorAndOpacity(color.HSVToLinearRGB());
	}
}

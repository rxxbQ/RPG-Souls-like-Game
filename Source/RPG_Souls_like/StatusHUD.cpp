// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusHUD.h"

void UStatusHUD::SetHealthValuePercent(float const Value)
{
	HealthValue->SetPercent(Value);
}

void UStatusHUD::SetManaValuePercent(float const Value)
{
	ManaValue->SetPercent(Value);
}

void UStatusHUD::SetStaminaValuePercent(float const Value)
{
	StaminaValue->SetPercent(Value);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

void UHealthBar::SetBarValuePercent(float const Value)
{
	HealthValue->SetPercent(Value);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/ArgoMathLibrary.h"

float UArgoMathLibrary::RoundTen(float Src)
{
	float newNum = Src / 10.f;
	newNum=FMath::RoundToFloat(newNum);
	newNum*=10.f;
	return newNum;
}

float UArgoMathLibrary::CeilingTen(float Src)
{
	float newNum = Src / 10.f;
	newNum=FMath::CeilToFloat(newNum);
	newNum*=10.f;
	return newNum;
}

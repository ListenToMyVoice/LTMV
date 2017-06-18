// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "TokenHolder.h"


UTokenHolder::UTokenHolder()
{
	PrimaryComponentTick.bCanEverTick = true;

	_Tablilla = nullptr;
}

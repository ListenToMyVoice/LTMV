// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "HandPickItem.h"


// Sets default values for this component's properties
UHandPickItem::UHandPickItem() : Super(), _locationAttach_L(0.f, 0.f, 0.f),
                                          _rotationAttach_L(0.f, 0.f, 0.f),
                                          _locationAttach_R(0.f, 0.f, 0.f), 
                                          _rotationAttach_R(0.f, 0.f, 0.f) {}


// Called when the game starts
void UHandPickItem::BeginPlay() {
	Super::BeginPlay();	
}


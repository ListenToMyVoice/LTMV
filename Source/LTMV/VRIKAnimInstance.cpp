// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "VRIKAnimInstance.h"

UVRIKAnimInstance::UVRIKAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    HeadPosition = FVector(0.f, 0.f, 160.f);
    HeadOrientation = FRotator(0.f, 0.f, 0.f);
    LeftPosition = FVector(0.f, 0.f, 0.f);;
    LeftOrientation = FRotator(0.f, 0.f, 0.f);
    RightPosition = FVector(0.f, 0.f, 0.f);;
    RightOrientation = FRotator(0.f, 0.f, 0.f);
}

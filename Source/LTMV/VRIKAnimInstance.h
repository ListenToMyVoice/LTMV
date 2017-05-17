// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "VRIKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LTMV_API UVRIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
    UVRIKAnimInstance(const FObjectInitializer& ObjectInitializer);

protected:
    /* Skeleton's bone variables */
    UPROPERTY(BlueprintReadWrite)
    FVector HeadPosition;
    UPROPERTY(BlueprintReadWrite)
    FRotator HeadOrientation;
    UPROPERTY(BlueprintReadWrite)
    FVector LeftPosition;
    UPROPERTY(BlueprintReadWrite)
    FRotator LeftOrientation;
    UPROPERTY(BlueprintReadWrite)
    FVector RightPosition;
    UPROPERTY(BlueprintReadWrite)
    FRotator RightOrientation;
	
};

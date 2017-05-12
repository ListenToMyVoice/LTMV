// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "HandAnim.generated.h"

//UENUM(BlueprintType)
//enum class EGripEnum : uint8 {
//    Open 	UMETA(DisplayName = "Open"),
//    CanGrab UMETA(DisplayName = "CanGrab"),
//    Grab	UMETA(DisplayName = "Grab")
//};

UCLASS()
class LTMV_API UHandAnim : public UAnimInstance {
    GENERATED_BODY()

//public:
//    APawn* _OwningPawn;
//
//    UHandAnim(const FObjectInitializer& OI);
//
//    void NativeInitializeAnimation() override;
//    void NativeUpdateAnimation(float DeltaSeconds) override;
//
//private:
//    EGripEnum _GripState;
//    float _InterpSpeed;
//    float _Grip;
};

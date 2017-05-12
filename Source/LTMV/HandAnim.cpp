// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "HandAnim.h"


//UHandAnim::UHandAnim(const FObjectInitializer& OI) :Super(OI) {
//    _GripState = EGripEnum::Open;
//    _InterpSpeed = 7;
//    _Grip = 0;
//}
//
//void UHandAnim::NativeInitializeAnimation() {
//    Super::NativeInitializeAnimation();
//
//    _OwningPawn = TryGetPawnOwner();
//}
//
//void UHandAnim::NativeUpdateAnimation(float DeltaSeconds) {
//    Super::NativeUpdateAnimation(DeltaSeconds);
//    if (!_OwningPawn) return;
//
//    _Grip = FMath::FInterpConstantTo(_Grip, (uint8)_GripState / 2, DeltaSeconds, _InterpSpeed);
//
//    //_OwningPawn
//
//    //ASkeletalMeshActor* Skel = Cast<ASkeletalMeshActor>(_OwningPawn);
//}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "GrabItem.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UGrabItem : public UActorComponent {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FVector _locationAttach_L;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttach_L;

    UPROPERTY(EditAnywhere)
    FVector _locationAttach_R;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttach_R;

    UGrabItem();
    virtual void BeginPlay() override;
};

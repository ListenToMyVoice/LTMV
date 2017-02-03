// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfSwitcheable.h"

#include "Engine/TriggerBox.h"
#include "TriggerBoxSwitcher.generated.h"


UCLASS()
class PROYECTO_API ATriggerBoxSwitcher : public ATriggerBox {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
    FSwitcher _switcher;
    UPROPERTY(EditAnywhere)
    int _numTimesActive;

    ATriggerBoxSwitcher();
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                        AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    //UFUNCTION()
    //void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
    //                  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

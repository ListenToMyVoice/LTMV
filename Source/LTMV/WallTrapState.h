// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "WallTrapState.generated.h"

UENUM(BlueprintType)
enum class EWallState : uint8 {
    NoPressed,
    OnePressed,
    TwoPressed
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UWallTrapState : public UActorComponent, public IItfSwitcheable {
    GENERATED_BODY()

public:
    UWallTrapState();

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    int SwitchState(int Param);
    virtual int SwitchState_Implementation(int Param) override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;

private:
    EWallState _State;
};

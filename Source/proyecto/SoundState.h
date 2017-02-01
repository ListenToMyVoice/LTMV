// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "SoundState.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO_API USoundState : public UAudioComponent, public IItfSwitcheable {
    GENERATED_BODY()

public:
    USoundState();
    virtual void BeginPlay() override;

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    int SwitchState();
    virtual int SwitchState_Implementation() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "LightState.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO_API ULightState : public UActorComponent, public IItfSwitcheable {
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Light)
    float MAX_INTENSITY;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Light)
    float _increment;

	ULightState();
	virtual void BeginPlay() override;

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    int SwitchState();
    virtual int SwitchState_Implementation() override;
};

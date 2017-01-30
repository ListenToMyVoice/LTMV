// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "LightState.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO_API ULightState : public UActorComponent, public IItfSwitcheable {
	GENERATED_BODY()

private:
    float _intensity;

    void AddIntensity();

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MAX_INTENSITY;

	ULightState();
	virtual void BeginPlay() override;

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    int SwitchState();
    virtual int SwitchState_Implementation() override;
};

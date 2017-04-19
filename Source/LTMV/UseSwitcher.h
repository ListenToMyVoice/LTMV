// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfUsable.h"
#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "UseSwitcher.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UUseSwitcher : public UActorComponent, public IItfUsable {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
    FSwitcher _switcher;

    UUseSwitcher();
    virtual void BeginPlay() override;

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use")
    void UsePressed();
    virtual void UsePressed_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pressed")
	void UseReleased();
	virtual void UseReleased_Implementation() override;


};

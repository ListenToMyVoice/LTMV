// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfUsableItem.h"

#include "Components/ActorComponent.h"
#include "Walkie.generated.h"

DECLARE_DELEGATE(FRadioDelegate)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UWalkie : public UActorComponent, public IItfUsableItem {
    GENERATED_BODY()

public:
    UWalkie();

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
    void UseItemPressed();
    virtual void UseItemPressed_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
    void UseItemReleased();
    virtual void UseItemReleased_Implementation() override;

protected:
    FRadioDelegate _RadioPressedDelegate;
    FRadioDelegate _RadioReleasedDelegate;

    virtual void BeginPlay() override;
};

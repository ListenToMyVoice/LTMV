// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ItfUsable.h"
#include "Toggle.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API UToggle : public UActorComponent, public IItfUsable
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UToggle();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ToggleComponent")
    TArray<AActor*> OtherActors;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ToggleComponent")
    TArray<bool> PropertyArray;

    UFUNCTION(BlueprintCallable, Category="ToggleComponent")
    void ToggleOn(bool &bProperty);

    UFUNCTION(BlueprintCallable, Category="ToggleComponent")
    void ToggleOff(bool &bProperty);

    UFUNCTION(BlueprintCallable, Category="ToggleComponent")
    void InvertState(bool &bProperty);

    UFUNCTION(BlueprintCallable, Category="ToggleComponent")
    bool IsToggleActive(bool bProperty);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ToggleComponent")
    bool Use();
    virtual bool Use_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ToggleComponent")
    bool Usable(bool &bProperty);
    virtual bool Usable_Implementation(bool &bProperty) override;

};
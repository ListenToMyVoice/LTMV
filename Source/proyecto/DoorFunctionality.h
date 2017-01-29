// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ItfUsable.h"
#include "DoorFunctionality.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API UDoorFunctionality : public UActorComponent, public IItfUsable
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDoorFunctionality();

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DoorFunctionality")
    bool bDoorState;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    bool Use();
    virtual bool Use_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ToggleActor")
    bool Usable(bool &bProperty);
    virtual bool Usable_Implementation(bool &bProperty) override;

    UFUNCTION(BlueprintCallable, Category = "DoorFunctionality")
    bool IsDoorOpen();
};
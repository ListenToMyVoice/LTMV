// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include"ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "DoorState.generated.h"

UENUM()
namespace EOnAxis {
    enum Type {
    X_AXIS      UMETA(DisplayName = "X Axis"),
    Y_AXIS      UMETA(DisplayName = "Y Axis"),
    Z_AXIS      UMETA(DisplayName = "Z Axis")
    };
}

UENUM()
namespace EDoorType {
    enum Type {
        SLIDABLE_DOOR    UMETA(DisplayName = "Slidable"),
        ROTABLE_DOOR     UMETA(DisplayName = "Rotable")
    };
}

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API UDoorState : public UActorComponent, public IItfSwitcheable {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation & Movement")
        TEnumAsByte<EDoorType::Type> DoorType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation & Movement")
        TEnumAsByte<EOnAxis::Type> ActOn;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
        float _Velocity;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
        float _displacement;

    UDoorState();
    virtual void BeginPlay() override;

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
        int SwitchState();
    virtual int SwitchState_Implementation() override;
};
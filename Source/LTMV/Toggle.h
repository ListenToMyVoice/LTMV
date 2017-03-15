// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Toggle.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UToggle : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UToggle();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ToggleComponent")
    TArray<AActor*> OtherActors;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ToggleComponent")
    TArray<UActorComponent*> OtherComponents;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ToggleComponent")
    TArray<bool> OtherActorsStates;

    UFUNCTION(BlueprintCallable, Category = "ToggleComponent")
    TArray<AActor*> GetArrayOfActors();

    UFUNCTION(BlueprintCallable, Category = "ToggleComponent")
    TArray<UActorComponent*> GetArrayOfComponents();

    UFUNCTION(BlueprintCallable, Category = "ToggleComponent")
    TArray<bool> GetArrayOfStates();

    UFUNCTION(BlueprintCallable, Category = "ToggleComponent")
    void SetState(bool NewState, int index);

    UFUNCTION(BlueprintCallable, Category="ToggleComponent")
    bool ToggleOn();

    UFUNCTION(BlueprintCallable, Category="ToggleComponent")
    bool ToggleOff();

    UFUNCTION(BlueprintCallable, Category="ToggleComponent")
    bool InvertState(bool bProperty);
};
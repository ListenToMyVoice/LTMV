// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ItemSave.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API UItemSave : public UActorComponent {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int _weight;

    UItemSave();
    virtual void BeginPlay() override;
};

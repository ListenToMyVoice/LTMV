// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CountPlayerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO_API UCountPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCountPlayerComponent();

    UFUNCTION(BlueprintCallable, Category = "PlayerCounter")
    int CountUp();

    UFUNCTION(BlueprintCallable, Category="PlayerCounter")
    int CountDown();

private:
    int PlayerCount;
};

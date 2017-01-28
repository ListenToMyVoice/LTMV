// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Toggle.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO_API UToggle : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToggle();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ToggleActor")
    AActor *OtherActor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ToggleActor")
    bool bProperty;
    
    UFUNCTION(BlueprintCallable, Category="ToggleActor")
    bool ToggleActorState();

};

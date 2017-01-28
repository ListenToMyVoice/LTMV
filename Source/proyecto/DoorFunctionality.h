// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ItfUsable.h"
#include "DoorFunctionality.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO_API UDoorFunctionality : public UActorComponent, public IItfUsable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorFunctionality();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="DoorFunctionality")
    bool bDoorState;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    void Use();
    virtual void Use_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "DoorFunctionality")
    bool IsDoorOpen();
};

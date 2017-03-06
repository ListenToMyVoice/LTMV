// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include"ItfSwitcheable.h"


#include "Components/ActorComponent.h"
#include "CountInteractionState.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API UCountInteractionState : public UActorComponent, public IItfSwitcheable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCountInteractionState();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
		float _NumInteractions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
		float _max_NumInteractions;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/* Interfaces */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wall")
		int SwitchState();
	virtual int SwitchState_Implementation() override;

	int SwitchState2();
	virtual int SwitchState2_Implementation() override;
	
};

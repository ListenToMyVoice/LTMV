// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItfUsable.h"
#include "Lock.h"

#include "Components/StaticMeshComponent.h"
#include "KeyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UKeyComponent : public UStaticMeshComponent, public IItfUsable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeyComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
    UPROPERTY(EditAnyWhere, Category = "Keypad")
        FString _keyNumber;
		
    void Use();
    virtual void Use_Implementation();


	void Press();
	virtual void Press_Implementation();
};

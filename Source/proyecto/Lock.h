// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "Lock.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO_API ULock : public UActorComponent {
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    FSwitcher _switcherOk;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    FSwitcher _switcherKo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    FSwitcher _switcherKey;


	ULock();

    bool solved;
    FString chain;

	virtual void BeginPlay() override;

    void insertNumber(FString number);

    UPROPERTY(EditAnywhere, Category = "Keypad")
    FString _correctPassword;	
};
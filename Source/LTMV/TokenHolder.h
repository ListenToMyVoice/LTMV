// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TokenHolder.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UTokenHolder : public UActorComponent {
	GENERATED_BODY()

public:
	UTokenHolder();

	UPROPERTY(BlueprintReadOnly)
	AActor* _Tablilla;
};

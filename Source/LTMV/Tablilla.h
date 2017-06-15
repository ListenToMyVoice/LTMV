// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItfUsableItem.h"

#include "Components/SceneComponent.h"
#include "Tablilla.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UTablilla : public USceneComponent, public IItfUsableItem {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTablilla();

	/*Inerfaces*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
	void UseItemPressed();
	virtual void UseItemPressed_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
	void UseItemReleased();
	virtual void UseItemReleased_Implementation() override;

};

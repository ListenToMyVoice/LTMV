// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItfUsableItem.h"
#include "FMODAudioComponent.h"

#include "Components/SceneComponent.h"
#include "Token.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UToken : public USceneComponent, public IItfUsableItem {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToken();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD Audio")
	UFMODAudioComponent* _TokenPlacement;

	/*Inerfaces*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
	void UseItemPressed();
	virtual void UseItemPressed_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
	void UseItemReleased();
	virtual void UseItemReleased_Implementation() override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItfUsableItem.h"

#include "Components/ActorComponent.h"
#include "Gun.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UGun : public UActorComponent, public IItfUsableItem {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetAmmo(uint8 AmountToSet);
	void AddAmmo(uint8 AmountToAdd);
	uint8 GetAmmo();

	/*Inerfaces*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
	void UseItemPressed();
	virtual void UseItemPressed_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Use Item")
	void UseItemReleased();
	virtual void UseItemReleased_Implementation() override;
		
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gun Component", meta = (AllowPrivateAccess = "true"))
	uint8 _ammo;
};

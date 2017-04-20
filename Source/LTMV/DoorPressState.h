// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include"ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "DoorPressState.generated.h"


UENUM()
namespace EOnAxis2 {
	enum Type {
		X_AXIS      UMETA(DisplayName = "X Axis"),
		Y_AXIS      UMETA(DisplayName = "Y Axis"),
		Z_AXIS      UMETA(DisplayName = "Z Axis")
	};
}

UENUM()
namespace EDoorType2 {
	enum Type {
		SLIDABLE_DOOR    UMETA(DisplayName = "Slidable"),
		ROTABLE_DOOR     UMETA(DisplayName = "Rotable")
	};
}

UENUM()
namespace EStateDoor2 {
	enum Type {
		CLOSE    UMETA(DisplayName = "Close"),
		OPENING     UMETA(DisplayName = "Opening"),
		OPEN     UMETA(DisplayName = "Open"),
		CLOSING     UMETA(DisplayName = "Closing")
	};
}

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UDoorPressState : public UActorComponent, public IItfSwitcheable {
	GENERATED_BODY()
private:
	float _start_displacement;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation & Movement")
		TEnumAsByte<EDoorType2::Type> DoorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation & Movement")
		TEnumAsByte<EOnAxis2::Type> ActOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation & Movement")
		TEnumAsByte<EStateDoor2::Type> StateDoor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
		float _displacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
		float _current_displacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
		float _max_displacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation & Movement")
		bool _block;
	// Sets default values for this component's properties
	UDoorPressState();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	/* Interfaces */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	int SwitchStatePressed();
	virtual int SwitchStatePressed_Implementation() override;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	int SwitchStateReleased();
	virtual int SwitchStateReleased_Implementation() override;
	
};

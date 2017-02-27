// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "RotateState.generated.h"

UENUM()
namespace EMyAxis {
    enum Type {
        X_AXIS  UMETA(DisplayName = "X Axis"),
        Y_AXIS  UMETA(DisplayName = "Y Axis"),
        Z_AXIS  UMETA(DisplayName = "Z Axis")
    };
}


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO_API URotateState : public UActorComponent, public IItfSwitcheable {
	GENERATED_BODY()

private:
    UStaticMeshComponent* _meshComp;
    FRotator _initialRotation;
    bool _requestOff;
    bool _opened;

    void AddRotation();

public:	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
        TEnumAsByte<EMyAxis::Type> RotateOnAxis;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation")
        float _velocity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation")
        float _displacement;

	// Sets default values for this component's properties
	URotateState();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Rotation")
        int SwitchState();
    virtual int SwitchState_Implementation() override;
	
};

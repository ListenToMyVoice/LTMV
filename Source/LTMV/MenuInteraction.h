// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "MenuInteraction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UMenuInteraction : public USceneComponent {
	GENERATED_BODY()

public:	
    float _RayParameter;

	UMenuInteraction();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void PressPointer();
    void ReleasePointer();

protected:
	virtual void BeginPlay() override;

    void Activation();
    void Deactivation();

private:
    bool _TargetLocked;
    class UInputMenu* _TargetInputMenu;
};

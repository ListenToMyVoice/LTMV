// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemOverlap.h"

#include "ItemTakeRight.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API UItemTakeRight : public UItemOverlap {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FVector _locationAttach;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttach;

    UItemTakeRight();
    virtual void BeginPlay() override;

    virtual void activateItem(UPrimitiveComponent* OverlappedComp,
                              APlayerCharacter* OtherActor,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult);
    
    virtual void deactivateItem(UPrimitiveComponent* OverlappedComp, APlayerCharacter* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void inputCB();

private:
    FInputActionBinding* _binding;
    bool _isAttached;

    void deactivateItem();
};

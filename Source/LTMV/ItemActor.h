// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "ItemActor.generated.h"

UCLASS()
class LTMV_API AItemActor : public AStaticMeshActor {
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere)
    FText _name;
    UPROPERTY(EditAnywhere)
    FText _description;
    UPROPERTY(EditAnywhere)
    UBoxComponent* _boxCollision;

    AItemActor();
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                        AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
                      AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Item.generated.h"

/**
 *
 */
UCLASS()
class PROYECTO_API AItem : public AStaticMeshActor {
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere)
    FText description;
    UPROPERTY(EditAnywhere)
    UBoxComponent* boxCollision;

    AItem();

    //virtual void Tick(float DeltaSeconds) override;
    
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                        AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnOverlapActorBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};

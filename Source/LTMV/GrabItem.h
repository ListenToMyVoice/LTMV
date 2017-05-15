// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "GrabItem.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UGrabItem : public UActorComponent {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FVector _locationAttach_L;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttach_L;
    UPROPERTY(EditAnywhere)
    FVector _locationAttach_R;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttach_R;
    UPROPERTY(EditAnyWhere)
    FVector _locationAttach_C;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttach_C;

    UGrabItem();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    void BeginGrab(USceneComponent* Target, FName SocketName);
    void EndGrab();

protected:
    virtual void BeginPlay() override;

private:
    bool _IsBeingTaked;
    UStaticMeshComponent* _SourceMesh;
    UStaticMeshComponent* _TargetMesh;
    FName _SocketName;
};

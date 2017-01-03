// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class PROYECTO_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()
public:
    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;

    APlayerCharacter();
    virtual void BeginPlay() override;

    /* OUTSIDE ACTION MAPPINGS */
    void TakeItem(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation);
    void SaveItem(AStaticMeshActor* itemActor);

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    /* ACTION MAPPINGS */
    void MoveForward(float Val);
    void MoveRight(float Val);
    /**
    * Called via input to turn at a given rate.
    * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
    */
    void TurnAtRate(float Rate);
    /**
    * Called via input to turn look up/down at a given rate.
    * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
    */
    void LookUpAtRate(float Rate);

private:
    void cleanItem(AStaticMeshActor* itemActor);
};

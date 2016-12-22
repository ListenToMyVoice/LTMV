// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef __LIBRARYUTILS_H
#include "LibraryUtils.h"
#define __LIBRARYUTILS_H
#endif

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class PROYECTO_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class USkeletalMeshComponent* Mesh1P;

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FirstPersonCameraComponent;

public:
    APlayerCharacter();

    virtual void BeginPlay() override;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;

    /** Returns Mesh1P subobject **/
    FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { 
        return FirstPersonCameraComponent; 
    }

    /* OUTSIDE ACTION MAPPINGS */
    void TakeItem(FString itemName);
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
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

//class UInputComponent;

struct ItemData {
    //GENERATED_USTRUCT_BODY()
    AItemActor* actor;
    UActorComponent* comp;
};

UCLASS(config = Game)
class PROYECTO_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()
public:
    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseLookUpRate;


    APlayerCharacter();
    virtual void BeginPlay() override;

    void ActivateScenaryItem(AItemActor* item);
    void DeactivateScenaryItem(AItemActor* item);

    /* OUTSIDE ACTION MAPPINGS */
    //bool TakeLeft(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation);
    //bool DropLeft(AStaticMeshActor* itemActor);

    //bool TakeRight(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation);
    //bool DropRight(AStaticMeshActor* itemActor);

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

    void TakeLeft();
    void TakeRight();

    void SaveLeft();
    void SaveRight();

    void Help();

private:
    AItemActor* _itemLeft;
    AItemActor* _itemRight;
    TArray<AItemActor*> _activeScenaryItems;

    void TakeItemLeft();
    void DropItemLeft();

    void TakeItemRight();
    void DropItemRight();

    void SaveInventory(AItemActor* itemActor);

    ItemData FindComponentUtil(const TSubclassOf<UActorComponent> ComponentClass);
};

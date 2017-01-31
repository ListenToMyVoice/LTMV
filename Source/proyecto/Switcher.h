// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemOverlap.h"
#include "ItfUsable.h"
#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "Switcher.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API USwitcher : public UItemOverlap, public IItfUsable {
    GENERATED_BODY()

private:
    TArray<UActorComponent*> SwitcheableComps;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    TArray<FSwitcheable> SwitcheableArr;
    
    //TMap<AActor*, FString> map;
    //TMap<AActor*, FName> map;
    //TArray<AActor*> SwitcheableArr;
    //TArray<UActorComponent*> SwitcheableArr;

    USwitcher();
    virtual void BeginPlay() override;
    
    virtual void activateItem(UPrimitiveComponent* OverlappedComp,
                              APlayerCharacter* player,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult);

    virtual void deactivateItem(UPrimitiveComponent* OverlappedComp, APlayerCharacter* player,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /* Interfaces */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    void Use();
    virtual void Use_Implementation() override;
};

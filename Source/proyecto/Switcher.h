// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemOverlap.h"
#include "ItfUsable.h"
#include "ItfSwitcheable.h"

#include "Components/ActorComponent.h"
#include "Switcher.generated.h"

USTRUCT(BlueprintType, Category = "ToggleActor")
struct FSwitcheable {
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    AActor* _actor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    TArray<FName> _components;

    FSwitcheable() {
        _actor = nullptr;
        _components = {};
    }
};

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
    bool Use();
    virtual bool Use_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ToggleActor")
    bool Usable(bool &bProperty);
    virtual bool Usable_Implementation(bool &bProperty) override;
};

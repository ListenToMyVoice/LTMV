// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Switcher.h"
#include "ItemActor.h"
#include "PlayerCharacter.h"

USwitcher::USwitcher() : Super() {
    PrimaryComponentTick.bCanEverTick = true;
    SwitcheableArr = {};
    SwitcheableComps = {};
}

void USwitcher::BeginPlay() {
    Super::BeginPlay();

    /* Fill the references to other switcheable componets */
    TArray<UActorComponent*> components = {};
    for (FSwitcheable s : SwitcheableArr) {
        s._actor->GetComponents(components);
        for (UActorComponent* comp : components) {
            if (s._components.IndexOfByKey(comp->GetFName()) != INDEX_NONE) {
                SwitcheableComps.AddUnique(comp);
            }
        }
    }
}

void USwitcher::activateItem(UPrimitiveComponent* OverlappedComp,
                            APlayerCharacter* player,
                            UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, player, OtherComp, OtherBodyIndex, bFromSweep,
                        SweepResult);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    player->ActivateScenaryItem(owner);
}

void USwitcher::deactivateItem(UPrimitiveComponent* OverlappedComp,
                              APlayerCharacter* player, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, player, OtherComp, OtherBodyIndex);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    player->DeactivateScenaryItem(owner);
}

bool USwitcher::Use_Implementation() {
    for (UActorComponent* component : SwitcheableComps) {
        if (component->GetClass()->ImplementsInterface(UItfSwitcheable::StaticClass())) {
            IItfSwitcheable* itfObject = Cast<IItfSwitcheable>(component);
            if (itfObject) itfObject->Execute_SwitchState(component);
        }
    }
    return true;
}

bool USwitcher::Usable_Implementation(bool &bProperty) {
    return true;
}
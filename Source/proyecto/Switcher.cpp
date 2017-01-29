// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Switcher.h"
#include "ItemActor.h"
#include "ItfSwitcheable.h"
#include "PlayerCharacter.h"

USwitcher::USwitcher() {
    PrimaryComponentTick.bCanEverTick = true;
    SwitcheableArr = {};
}

void USwitcher::BeginPlay() {
    Super::BeginPlay();
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

int USwitcher::Use_Implementation() {
    ULibraryUtils::Log(TEXT("Use_Implementation"));

    for (AActor* actor : SwitcheableArr) {
        const TSet <UActorComponent*> set = actor->GetComponents();
        for (UActorComponent* component : set) {
            if (component->GetClass()->ImplementsInterface(UItfSwitcheable::StaticClass())) {
                IItfSwitcheable* itfObject = Cast<IItfSwitcheable>(component);
                if (itfObject) itfObject->Execute_SwitchState(component);
            }
        }
    }

    return 0;
}
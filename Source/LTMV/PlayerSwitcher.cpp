// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerSwitcher.h"
#include "ItemActor.h"
#include "PlayerCharacter.h"

UPlayerSwitcher::UPlayerSwitcher() : Super() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerSwitcher::BeginPlay() {
    Super::BeginPlay();
    _switcher.InitSwitcher();
}

void UPlayerSwitcher::activateItem(UPrimitiveComponent* OverlappedComp,
                            APlayerCharacter* player,
                            UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, player, OtherComp, OtherBodyIndex, bFromSweep,
                        SweepResult);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    //player->ActivateScenaryItem(owner);
}

void UPlayerSwitcher::deactivateItem(UPrimitiveComponent* OverlappedComp,
                              APlayerCharacter* player, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, player, OtherComp, OtherBodyIndex);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    //player->DeactivateScenaryItem(owner);
}

void UPlayerSwitcher::Use_Implementation() {
    _switcher.ActivateSwitcher();
}


void UPlayerSwitcher::Press_Implementation() {
	_switcher.ActivateSwitcher2();
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Gun.h"


// Sets default values for this component's properties
UGun::UGun(){
	PrimaryComponentTick.bCanEverTick = true;
	_ammo = 2;
}


// Called when the game starts
void UGun::BeginPlay(){
	Super::BeginPlay();
}


// Called every frame
void UGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/*Interfaces*/
void UGun::UseItemPressed_Implementation() {
	if (GetAmmo() > 0) { AddAmmo(-1); }
	ULibraryUtils::Log(FString::Printf(TEXT("Ammo : %i"), GetAmmo()), 0, 60);
	//TODO:
	/*Make noise*/
}

void UGun::UseItemReleased_Implementation() {

}

void UGun::SetAmmo(uint8 AmountToSet) { _ammo = AmountToSet; }

void UGun::AddAmmo(uint8 AmountToAdd) { _ammo += AmountToAdd; }

uint8 UGun::GetAmmo() { return _ammo; }





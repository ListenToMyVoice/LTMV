// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Lantern.h"


ULantern::ULantern(){
	PrimaryComponentTick.bCanEverTick = true;
    _batteryLife = 2;
}


void ULantern::BeginPlay(){
	Super::BeginPlay();
}


void ULantern::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    _batteryLife -= 0.001;
    UsingBattery();
}


/***************************************ACTIONS****************************************************/
void ULantern::UsingBattery() {    

    if (_batteryLife > 0.0 && _batteryLife < 1.0) {
        Super::SetComponentTickEnabled(false);
        Super::PrimaryComponentTick.bCanEverTick = false;

        _batteryLife = 0.0;
        
        USpotLightComponent* SpotLight = Cast<USpotLightComponent>(GetOwner()->GetComponentByClass(
            USpotLightComponent::StaticClass()));
        SpotLight->SetIntensity(0);
    }
}

void ULantern::AddBatteryLife(float BatteryAmount) {
    _batteryLife += BatteryAmount;
}

float ULantern::GetBatteryLife() {
    return _batteryLife;
}


/******************Interfaces*****************/

void ULantern::UseItemPressed_Implementation() {
    Super::SetComponentTickEnabled(true);
    Super::PrimaryComponentTick.bCanEverTick = true;
}

void ULantern::UseItemReleased_Implementation() {
    Super::SetComponentTickEnabled(false);
    Super::PrimaryComponentTick.bCanEverTick = false;
}
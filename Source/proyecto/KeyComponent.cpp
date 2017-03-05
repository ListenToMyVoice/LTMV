// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "KeyComponent.h"
#include "ItfUsable.h"
#include "GameStateLTMV.h"


// Sets default values for this component's properties
UKeyComponent::UKeyComponent() : Super() {}


// Called when the game starts
void UKeyComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UKeyComponent::Use_Implementation() {
    UE_LOG(LogTemp, Warning, TEXT("You have release: %s"), *_keyNumber);
    
    UActorComponent* component = this->GetOwner()->GetComponentByClass(ULock::StaticClass());
    ULock* lockObject = Cast<ULock>(component);
    
    //BUSCAR AL LOCK Y LAMAR AL METODO INSERTNUMBER
    lockObject->insertNumber(_keyNumber);
}

void UKeyComponent::Press_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("You have pressed: %s"), *_keyNumber);

	UActorComponent* component = this->GetOwner()->GetComponentByClass(ULock::StaticClass());
	ULock* lockObject = Cast<ULock>(component);

	//BUSCAR AL LOCK Y LAMAR AL METODO INSERTNUMBER
	lockObject->insertNumber(_keyNumber);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "CountPlayerComponent.h"


// Sets default values for this component's properties
UCountPlayerComponent::UCountPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

    // ...

    PlayerCount = 0;

}

int UCountPlayerComponent::CountUp() {
    return ++PlayerCount;
}

int UCountPlayerComponent::CountDown() {
    return --PlayerCount;
}
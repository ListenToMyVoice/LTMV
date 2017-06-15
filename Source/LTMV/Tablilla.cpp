// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Tablilla.h"

UTablilla::UTablilla()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTablilla::UseItemPressed_Implementation() {
	AActor* _Player = GetOwner();
}

void UTablilla::UseItemReleased_Implementation() {}

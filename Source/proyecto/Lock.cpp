// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Lock.h"


#include "PlayerCharacter.h"
#include "GameStateLTMV.h"
#include "ItfUsable.h"
#include "ItemActor.h"

// Sets default values for this component's properties
ULock::ULock() : Super() {
    chain = "";
    solved = false;
}

// Called when the game starts
void ULock::BeginPlay()
{
    Super::BeginPlay();
}


void ULock::insertNumber(FString number) {
    chain.Append(number);

    UActorComponent* txtComponent = this->GetOwner()->GetComponentByClass(UTextRenderComponent::StaticClass());
    UTextRenderComponent* display = Cast<UTextRenderComponent>(txtComponent);
    
    display->SetText(display->Text.ToString() + number);

    if (chain.Len() == _correctPassword.Len()){
        if (chain.Equals(_correctPassword)) {
            solved = true;
            display->SetText("ACESS GRANTED");
            UE_LOG(LogTemp, Warning, TEXT("CODIGO CORRECTO - ABRIENDO PUERTAS..."));
            //ABRIR PUERTA
        }
        else {
            chain = "";
            display->SetText(chain);
            UE_LOG(LogTemp, Warning, TEXT("CODIGO ERRONEO"));
        }
    }

    if (solved) {
        //DESACTIVAR COMPONENTE
    }
}


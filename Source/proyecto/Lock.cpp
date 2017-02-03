// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Lock.h"

// Sets default values for this component's properties
ULock::ULock() : Super() {
    chain = "";
    solved = false;
}

// Called when the game starts
void ULock::BeginPlay()
{
    Super::BeginPlay();
    _switcherOk.InitSwitcher();
    _switcherKo.InitSwitcher();
    _switcherKey.InitSwitcher();
}


void ULock::insertNumber(FString number) {
    if (!solved) {
        chain.Append(number);

        UActorComponent* txtComponent = this->GetOwner()->GetComponentByClass(UTextRenderComponent::StaticClass());
        UTextRenderComponent* display = Cast<UTextRenderComponent>(txtComponent);
    
        //FVector Position;
        //Position.X = 0.0f;
        //Position.Y = 12.0f;
        //Position.Z = 3.0f;

        display->SetText(display->Text.ToString() + number);

        if (chain.Len() == _correctPassword.Len()){
            if (chain.Equals(_correctPassword)) {
                solved = true;
                display->AddRelativeLocation({0.0f,12.0f,3.0f});
                display->SetWorldSize(10);
                display->SetText("ACESS GRANTED");
                UE_LOG(LogTemp, Warning, TEXT("CODIGO CORRECTO - ABRIENDO PUERTAS..."));       
                _switcherOk.ActivateSwitcher();

            }
            else {
                chain = "";
                display->SetText(chain);
                UE_LOG(LogTemp, Warning, TEXT("CODIGO ERRONEO"));
                _switcherKo.ActivateSwitcher();
            }
        }
        else {
            _switcherKey.ActivateSwitcher();
        }
    }
}


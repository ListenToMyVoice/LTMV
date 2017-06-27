// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Walkie.h"


UWalkie::UWalkie() {
    _AreDelegatesBinded = false;
	_isMuted = false;
}

void UWalkie::BeginPlay() {
    Super::BeginPlay();

	//lights hidden
	TArray<UActorComponent*> PointLightArray;
	PointLightArray = GetOwner()->GetComponentsByClass(UPointLightComponent::StaticClass());
	
	Cast<UPointLightComponent>(PointLightArray[0])->SetIntensity(0);//POner la luz apagada
	Cast<UPointLightComponent>(PointLightArray[1])->SetIntensity(0);//POner la luz apagada

}

void UWalkie::UseItemPressed_Implementation() {
    _RadioPressedEvent.Broadcast();
}


void UWalkie::UseItemReleased_Implementation() {
    _RadioReleasedEvent.Broadcast();
}

void UWalkie::ToggleLight(bool on) {
	if (!IsMuted()) {
		TArray<UActorComponent*> PointLightArray;
		PointLightArray = GetOwner()->GetComponentsByClass(UPointLightComponent::StaticClass());
		if (on) {
			FString _name;
			for (UActorComponent* PointLight : PointLightArray) {
				PointLight->GetName();
				if (PointLight->GetName() == "White") {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HABLANDO POR EL WALKIEE3")));
					Cast<UPointLightComponent>(PointLight)->SetIntensity(50000);//POner la luz blanca encendida
				}
			}

		}
		else {
			FString _name;
			for (UActorComponent* PointLight : PointLightArray) {
				if (PointLight->GetName() == "White") {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("no POR EL WALKIEE3")));
					Cast<UPointLightComponent>(PointLight)->SetIntensity(0);//POner la luz blanca apagada
				}
			}
		}
	}
}

void UWalkie::ToggleOtherLight(bool on) {
	//Buscamos el componente de la luz del actor
	TArray<UActorComponent*> PointLightArray;
	PointLightArray = GetOwner()->GetComponentsByClass(UPointLightComponent::StaticClass());
	FString _name2;
	if (on) {
		for (UActorComponent* PointLight : PointLightArray) {
			if (PointLight->GetName() == "Red") {
				Cast<UPointLightComponent>(PointLight)->SetIntensity(5000);//POner la luz roja encendida
			}
		}

	}
	else {
		for (UActorComponent* PointLight : PointLightArray) {
			if (PointLight->GetName() == "Red") {
				Cast<UPointLightComponent>(PointLight)->SetIntensity(0);//POner la luz roja apagada
			}
		}
	}

}

bool UWalkie::IsMuted() {
	return _isMuted;
}
void UWalkie::SetMute(bool mute) {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Player mute %d"),mute));
	_isMuted = mute;
}
/*********************************************** DELEGATES ***************************************/
FDelegateHandle UWalkie::AddOnRadioDelegate(FRadioDelegate& RadioDelegate, bool IsPressed) {
    _AreDelegatesBinded = true;
    if(IsPressed) return _RadioPressedEvent.Add(RadioDelegate);
    else return _RadioReleasedEvent.Add(RadioDelegate);
}

void UWalkie::ClearOnRadioDelegate(FDelegateHandle DelegateHandle, bool IsPressed) {
    _AreDelegatesBinded = false;
    if (IsPressed) _RadioPressedEvent.Remove(DelegateHandle);
    else _RadioReleasedEvent.Remove(DelegateHandle);
}
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
				_name = Cast<UPointLightComponent>(PointLight)->GetReadableName();
				if (_name.Contains("White")) {
					Cast<UPointLightComponent>(PointLight)->SetIntensity(40);//POner la luz blanca encendida
				}
			}

		}
		else {
			FString _name;
			for (UActorComponent* PointLight : PointLightArray) {
				_name = Cast<UPointLightComponent>(PointLight)->GetReadableName();
				if (_name.Contains("White")) {
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
			_name2 = Cast<UPointLightComponent>(PointLight)->GetReadableName();
			if (_name2.Contains("Red")) {
				Cast<UPointLightComponent>(PointLight)->SetIntensity(40);//POner la luz roja encendida
			}
		}

	}
	else {
		for (UActorComponent* PointLight : PointLightArray) {
			_name2 = Cast<UPointLightComponent>(PointLight)->GetReadableName();
			if (_name2.Contains("Red")) {
				Cast<UPointLightComponent>(PointLight)->SetIntensity(0);//POner la luz roja apagada
			}
		}
	}

}

bool UWalkie::IsMuted() {
	return _isMuted;
}
void UWalkie::SetMute(bool mute) {
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
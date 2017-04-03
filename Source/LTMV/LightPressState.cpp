// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "LightPressState.h"


// Sets default values for this component's properties
ULightPressState::ULightPressState()
{
	PrimaryComponentTick.bCanEverTick = true;
	_on = false;//La luz inicia como apagada
}


// Called when the game starts
void ULightPressState::BeginPlay()
{
	Super::BeginPlay();

	//Iniciar la luz en apagado
	_lightComp = Cast<ULightComponent>(GetOwner()->GetComponentByClass(ULightComponent::StaticClass()));
	ULibraryUtils::TestNull(_lightComp);

}


// Called every frame
void ULightPressState::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	if (_on) {
		_lightComp->SetIntensity(500.0f);

	}
	else {
		_lightComp->SetIntensity(0.0f);
	}
}


int ULightPressState::SwitchState2_Implementation() {
	//Encender la luz durante el press
	_on = true;
	_current_state = _on;
	return 0;
}
int ULightPressState::SwitchState_Implementation() {
	//luz apagada en el release
	_on = false;
	_current_state = _on;
	return 0;
}

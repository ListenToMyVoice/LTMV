// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "LibraryUtils.h"

#include "NWGameInstance.h"
#include "VRCharacter.h"
#include "FPCharacter.h"
#include "TokenHolder.h"
#include "Token.h"

UToken::UToken()
{
	this->bVisible = true;
	this->bHiddenInGame = true;

	PrimaryComponentTick.bCanEverTick = true;
}

void UToken::UseItemPressed_Implementation() {
	UNWGameInstance* GI = Cast<UNWGameInstance>(GetWorld()->GetGameInstance());

	// Setting character and ActorFocused
	if (GI && GI->_IsVRMode) {
		AVRCharacter* _Player = Cast<AVRCharacter>(GetOwner()->GetAttachParentActor());

		if (_Player && _Player->GetActorFocusedLeft() &&
			_Player->GetActorFocusedLeft()->GetComponentByClass(UTokenHolder::StaticClass()) &&
			_Player->GetItemLeft()->GetComponentByClass(UToken::StaticClass())) {

			_Player->DropLeft();
		}

		else if (_Player && _Player->GetActorFocusedRight() &&
			_Player->GetActorFocusedRight()->GetComponentByClass(UTokenHolder::StaticClass()) &&
			_Player->GetItemRight()->GetComponentByClass(UToken::StaticClass())) {
			
			_Player->DropRight();
		}
	}
	else {
		AFPCharacter* _Player = Cast<AFPCharacter>(GetOwner()->GetAttachParentActor());

		if (_Player && _Player->GetItemFocused() && 
			_Player->GetItemFocused()->GetComponentByClass(UTokenHolder::StaticClass())) {
			
			if (_Player->GetItemLeft()->GetComponentByClass(UToken::StaticClass())) {
				
				_Player->TakeDropLeft();
			}
		
			else if (_Player->GetItemRight()->GetComponentByClass(UToken::StaticClass())) {
		
				_Player->TakeDropRight();
			}
		}
	}

	if (_TokenPlacement) _TokenPlacement->Play();
}

void UToken::UseItemReleased_Implementation() {}

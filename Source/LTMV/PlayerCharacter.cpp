// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerCharacter.h"

#include "ItfUsable.h"
#include "ItfUsableItem.h"
#include "HandPickItem.h"
#include "FMODAudioComponent.h"
#include "GameModePlay.h"
#include "Walkie.h"
#include "Components/WidgetInteractionComponent.h"

APlayerCharacter::APlayerCharacter() {
    bReplicates = true;
    bReplicateMovement = true;

    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;
    _ItemLeft = nullptr;
    _ItemRight = nullptr;

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    _PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    _WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
    _WidgetInteractionComp->InteractionDistance = 100000;
    _StepsAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));

    _Health = 1;
}

void APlayerCharacter::AfterPossessed(bool SetInventory) {}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInput) {
    check(PlayerInput);

    /* MOVEMENT */
    PlayerInput->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInput->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

/****************************************** ACTION MAPPINGS **************************************/
/*********** MOVEMENT ***********/
void APlayerCharacter::MoveForward(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void APlayerCharacter::MoveRight(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void APlayerCharacter::TurnAtRate(float Rate) {
    AddControllerYawInput(Rate * _baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate) {
    AddControllerPitchInput(Rate * _baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/************** USE *************/
void APlayerCharacter::UsePressed() {}
bool APlayerCharacter::SERVER_UsePressed_Validate(UActorComponent* component) { return true; }
void APlayerCharacter::SERVER_UsePressed_Implementation(UActorComponent* component) {
    MULTI_UsePressed(component);
}
void APlayerCharacter::MULTI_UsePressed_Implementation(UActorComponent* component) {
    IItfUsable* itfObject = Cast<IItfUsable>(component);;
    if (itfObject) itfObject->Execute_UsePressed(component);
}

void APlayerCharacter::UseReleased() {}
bool APlayerCharacter::SERVER_UseReleased_Validate(UActorComponent* component) { return true; }
void APlayerCharacter::SERVER_UseReleased_Implementation(UActorComponent* component) {
    MULTI_UseReleased(component);
}
void APlayerCharacter::MULTI_UseReleased_Implementation(UActorComponent* component) {
    IItfUsable* itfObject = Cast<IItfUsable>(component);
    if (itfObject) itfObject->Execute_UseReleased(component);
}

/******** USE ITEM LEFT *********/
void APlayerCharacter::UseLeftPressed(bool IsMenuHidden) {}

void APlayerCharacter::UseLeftReleased(bool IsMenuHidden) {}

/******* USE ITEM RIGHT *********/
void APlayerCharacter::UseRightPressed(bool IsMenuHidden) {}

void APlayerCharacter::UseRightReleased(bool IsMenuHidden) {}

/********** TAKE RIGHT HAND ***********/
void APlayerCharacter::TakeDropRight() {}
bool APlayerCharacter::SERVER_TakeRight_Validate(AActor* Actor) { return true; }
void APlayerCharacter::SERVER_TakeRight_Implementation(AActor* Actor) {
    MULTI_TakeRight(Actor);
}
void APlayerCharacter::MULTI_TakeRight_Implementation(AActor* Actor) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    UHandPickItem* HandPickComp = Cast<UHandPickItem>(Actor->FindComponentByClass(
                                                            UHandPickItem::StaticClass()));
    if (ItemMesh && HandPickComp) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->SetSimulatePhysics(false);
        ItemMesh->AttachToComponent(GetMesh(),
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand_r"));

        ItemMesh->RelativeLocation = HandPickComp->_locationAttach_R;
        ItemMesh->RelativeRotation = HandPickComp->_rotationAttach_R;

        Actor->SetActorEnableCollision(false);
        _ItemRight = Actor;
    }
}

/********** TAKE LEFT HAND ***********/
void APlayerCharacter::TakeDropLeft() {}
bool APlayerCharacter::SERVER_TakeLeft_Validate(AActor* Actor) { return true; }
void APlayerCharacter::SERVER_TakeLeft_Implementation(AActor* Actor) {
    MULTI_TakeLeft(Actor);
}
void APlayerCharacter::MULTI_TakeLeft_Implementation(AActor* Actor) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    UHandPickItem* HandPickComp = Cast<UHandPickItem>(Actor->FindComponentByClass(
        UHandPickItem::StaticClass()));
    if (ItemMesh && HandPickComp) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->SetSimulatePhysics(false);
        ItemMesh->AttachToComponent(GetMesh(),
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand_l"));

        ItemMesh->RelativeLocation = HandPickComp->_locationAttach_R;
        ItemMesh->RelativeRotation = HandPickComp->_rotationAttach_R;

        Actor->SetActorEnableCollision(false);
        _ItemLeft = Actor;
    }
}

/********** DROP HAND ***********/
bool APlayerCharacter::SERVER_Drop_Validate(AActor* ItemActor, int Hand) { return true; }
void APlayerCharacter::SERVER_Drop_Implementation(AActor* ItemActor, int Hand) {
    MULTI_Drop(ItemActor, Hand);
}
void APlayerCharacter::MULTI_Drop_Implementation(AActor* ItemActor, int Hand) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        ItemMesh->SetSimulatePhysics(true);

        ItemActor->SetActorEnableCollision(true);

        if (Hand == 1) _ItemLeft = nullptr;
        else if (Hand == 2) _ItemRight = nullptr;
    }
}

/****************************************** AUXILIAR FUNCTIONS ***********************************/
AActor* APlayerCharacter::GetWalkieActor() { 
    return _WalkieActor ? _WalkieActor : nullptr; 
}

bool APlayerCharacter::IsWalkieInHand() {
    if (_ItemLeft) ULibraryUtils::Log(FString::Printf(TEXT("_ItemLeft: %s"), *_ItemLeft->GetFName().ToString()));
    if (_ItemRight) ULibraryUtils::Log(FString::Printf(TEXT("_ItemRight: %s"), *_ItemRight->GetFName().ToString()));
    return _WalkieActor ? _WalkieActor == _ItemLeft || _WalkieActor == _ItemRight : false;
}

/******* Radio Delegate *******/
void APlayerCharacter::CLIENT_AddRadioDelegates_Implementation(AActor* Actor) {
    if (Actor) {
        UWalkie* Walkie = Cast<UWalkie>(Actor->GetComponentByClass(UWalkie::StaticClass()));
        if (Walkie && !Walkie->_AreDelegatesBinded) {
            _OnRadioPressedDelegateHandle = Walkie->AddOnRadioDelegate(_OnRadioPressedDelegate, true);
            _OnRadioReleasedDelegateHandle = Walkie->AddOnRadioDelegate(_OnRadioReleasedDelegate, false);

            _WalkieActor = Actor;
            
            const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
            FString myRole = EnumPtr->GetEnumName((int32)Role);
            ULibraryUtils::Log(FString::Printf(TEXT("AddRadioDelegates : %s"), *myRole), 0, 60);
        }
    }
}

void APlayerCharacter::CLIENT_ClearRadioDelegates_Implementation(AActor* Actor) {
    if (Actor) {
        UWalkie* Walkie = Cast<UWalkie>(Actor->GetComponentByClass(UWalkie::StaticClass()));
        if (Walkie && Walkie->_AreDelegatesBinded) {
            Walkie->ClearOnRadioDelegate(_OnRadioPressedDelegateHandle, true);
            Walkie->ClearOnRadioDelegate(_OnRadioReleasedDelegateHandle, false);

            _WalkieActor = Actor;

            const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
            FString myRole = EnumPtr->GetEnumName((int32)Role);
            ULibraryUtils::Log(FString::Printf(TEXT("ClearRadioDelegates : %s"), *myRole), 0, 60);
        }
    }
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                   class AController* EventInstigator, class AActor* DamageCauser) {
    //Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    _Health -= DamageAmount;
    if (_Health <= 0) {
        AGameModePlay* GameMode = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
        if (GameMode) {
            GameMode->SERVER_PlayerDead(GetController());
            MULTI_CharacterDead();
        }
    }
    return _Health;
}

void APlayerCharacter::MULTI_CharacterDead_Implementation() {
    Cast<UPrimitiveComponent>(GetRootComponent())->SetCollisionProfileName(FName("Ragdoll"));
    SetActorEnableCollision(true);
    GetMesh()->SetSimulatePhysics(true);
}

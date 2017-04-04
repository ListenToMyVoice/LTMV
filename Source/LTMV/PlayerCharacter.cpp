// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerCharacter.h"

#include "ItemTakeLeft.h"
#include "ItemTakeRight.h"
#include "ItemSave.h"
#include "Inventory.h"
#include "ItfUsable.h"
#include "KeyComponent.h"
#include "ItemActor.h"

APlayerCharacter::APlayerCharacter() {
    bReplicates = true;
    bReplicateMovement = true;

    // set our turn rates for input
    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;
    _itemLeft = nullptr;
    _itemRight = nullptr;
    _isAction = false;
    _activeScenaryItems = {};

    /*RAYCAST PARAMETERS*/
    RayParameter = 150.0f;

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
    _audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

    _Health = 3;
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
    GetOwnComponents();
}

void APlayerCharacter::GetOwnComponents() {
    TArray<UActorComponent*> Components;
    GetComponents(Components);

    for (UActorComponent* Component : Components) {
        if (Component && Component->IsA<UCameraComponent>()) {
            _PlayerCamera = Cast<UCameraComponent>(Component);
        }
    }
}

void APlayerCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    bool stop = false;
    if (GetCharacterMovement()->Velocity.Equals(FVector::ZeroVector)) {
        stop = true;
    }
    SwitchSound(_walkSound, stop);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* playerInput) {
    check(playerInput);

    /* MOVEMENT */
    playerInput->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    playerInput->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    playerInput->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    playerInput->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    playerInput->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    playerInput->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    playerInput->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    playerInput->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    /* SERVER */
    playerInput->BindAction("TakeLeft", IE_Released, this, &APlayerCharacter::TakeLeft);
    playerInput->BindAction("TakeRight", IE_Released, this, &APlayerCharacter::TakeRight);
    playerInput->BindAction("SaveLeft", IE_Released, this, &APlayerCharacter::SaveLeft);
    playerInput->BindAction("SaveRight", IE_Released, this, &APlayerCharacter::SaveRight);
    //playerInput->BindAction("Help", IE_Released, this, &APlayerCharacter::Help);
    playerInput->BindAction("Use", IE_Released, this, &APlayerCharacter::Use);
	//Mantener push
	playerInput->BindAction("Press", IE_Pressed, this, &APlayerCharacter::Press);
}

FHitResult APlayerCharacter::Raycasting() {

    bool bHitRayCastFlag;
    FHitResult HitActor;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = _PlayerCamera->GetForwardVector() * RayParameter + StartRaycast;

    bHitRayCastFlag = GetWorld()->LineTraceSingleByChannel(HitActor, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);
    //DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);

    //if (bHitRayCastFlag && HitActor.Actor.IsValid()) {
    //    // COOL STUFF TO GRAB OBJECTS
    //    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitActor.Actor->GetName()));
    //}

    return HitActor;
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
bool APlayerCharacter::RayCastCamera(FHitResult &hitActor) {
    FCollisionQueryParams CollisionInfo;
    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = StartRaycast + (_PlayerCamera->GetComponentRotation().Vector() * RayParameter);
    //DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);
    return GetWorld()->LineTraceSingleByChannel(hitActor, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);
}

void APlayerCharacter::Use() {
    /* OVERLAPPING DETECTION */
    bool found = false;
    int i = _activeScenaryItems.Num() - 1;
    while (!found && i >= 0) {
        const TSet <UActorComponent*> set = _activeScenaryItems[i]->GetComponents();
        for (UActorComponent* component : set) {
            if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_Use(component);
            }
        }
        i--;
    }

    /* RAYCASTING DETECTION */
    FHitResult hitActor;
    if (RayCastCamera(hitActor)) {
        UActorComponent* component = hitActor.GetComponent();
        if (component && component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
            SERVER_Use(component);
        }
    }
}

bool APlayerCharacter::SERVER_Use_Validate(UActorComponent* component) { return true; }
void APlayerCharacter::SERVER_Use_Implementation(UActorComponent* component) {
    MULTI_Use(component);
}

void APlayerCharacter::MULTI_Use_Implementation(UActorComponent* component) {
    IItfUsable* itfObject = Cast<IItfUsable>(component);
    if (itfObject) itfObject->Execute_Use(component);
}


/*********** PRESS *********/
void APlayerCharacter::Press() {
	/* OVERLAPPING DETECTION */
	bool found = false;
	int i = _activeScenaryItems.Num() - 1;
	while (!found && i >= 0) {
		const TSet <UActorComponent*> set = _activeScenaryItems[i]->GetComponents();
		for (UActorComponent* component : set) {
			if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
				SERVER_Press(component);
			}
		}
		i--;
	}

	/* RAYCASTING DETECTION */
	FHitResult hitActor;
	if (RayCastCamera(hitActor)) {
		UActorComponent* component = hitActor.GetComponent();
		if (component && component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
			SERVER_Press(component);
		}
	}
}
bool APlayerCharacter::SERVER_Press_Validate(UActorComponent* component) { return true; }
void APlayerCharacter::SERVER_Press_Implementation(UActorComponent* component) {
	MULTI_Press(component);
}

void APlayerCharacter::MULTI_Press_Implementation(UActorComponent* component) {
	IItfUsable* itfObject = Cast<IItfUsable>(component);
	if (itfObject) itfObject->Execute_Press(component);
}

/********** TAKE LEFT ***********/
void APlayerCharacter::TakeLeft() {
    if (_itemLeft && _activeScenaryItems.Num() > 0) {
        // REPLACE
        SERVER_DropLeft();
        ItemData data = FindItemAndComponents(UItemTakeLeft::StaticClass());
        UItemTakeLeft* takeComp = data.actor ? Cast<UItemTakeLeft>(data.components[0]) : nullptr;
        if (takeComp) SERVER_TakeLeft(data.actor, takeComp);
    }
    else if (_itemLeft && _activeScenaryItems.Num() <= 0) {
        // DROP
        SERVER_DropLeft();
    }
    else if (!_itemLeft && _activeScenaryItems.Num() > 0) {
        // TAKE
        ItemData data = FindItemAndComponents(UItemTakeLeft::StaticClass());
        UItemTakeLeft* takeComp = data.actor ? Cast<UItemTakeLeft>(data.components[0]) : nullptr;
        if (takeComp) SERVER_TakeLeft(data.actor, takeComp);
    }
}

bool APlayerCharacter::SERVER_TakeLeft_Validate(AItemActor* actor, UItemTakeLeft* takeComp) { return true; }
void APlayerCharacter::SERVER_TakeLeft_Implementation(AItemActor* actor, UItemTakeLeft* takeComp) {
    MULTI_TakeLeft(actor, takeComp);
}

void APlayerCharacter::MULTI_TakeLeft_Implementation(AItemActor* actor, UItemTakeLeft* takeComp) {
    _itemLeft = actor;
    UStaticMeshComponent* mesh = _itemLeft->GetStaticMeshComponent();
    if (mesh) {
        mesh->SetSimulatePhysics(false);

        mesh->AttachToComponent(GetMesh(),
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand_l"));

        mesh->RelativeLocation = takeComp->_locationAttach;
        mesh->RelativeRotation = takeComp->_rotationAttach;

        //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        //UE_LOG(LogTemp, Warning, TEXT("%s: TakeLeft"), *EnumPtr->GetEnumName((int32)Role));
    }
    _itemLeft->SetActorEnableCollision(false);
    _activeScenaryItems.Remove(actor);
}

bool APlayerCharacter::SERVER_DropLeft_Validate() { return true; }
void APlayerCharacter::SERVER_DropLeft_Implementation() {
    MULTI_DropLeft();
}

void APlayerCharacter::MULTI_DropLeft_Implementation() {
    UStaticMeshComponent* mesh = _itemLeft->GetStaticMeshComponent();
    if (mesh) {
        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        mesh->SetSimulatePhysics(true);

        //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        //UE_LOG(LogTemp, Warning, TEXT("%s: DropLeft"), *EnumPtr->GetEnumName((int32)Role));
    }
    _itemLeft->SetActorEnableCollision(true);
    _itemLeft = nullptr;
}

/********** TAKE RIGHT ***********/
void APlayerCharacter::TakeRight() {
    if (_itemRight && _activeScenaryItems.Num() > 0) {
        // REPLACE
        SERVER_DropRight();
        ItemData data = FindItemAndComponents(UItemTakeRight::StaticClass());
        UItemTakeRight* takeComp = data.actor ? Cast<UItemTakeRight>(data.components[0]) : nullptr;
        if (takeComp) SERVER_TakeRight(data.actor, takeComp);
    }
    else if (_itemRight && _activeScenaryItems.Num() <= 0) {
        // DROP
        SERVER_DropRight();
    }
    else if (!_itemRight && _activeScenaryItems.Num() > 0) {
        // TAKE
        ItemData data = FindItemAndComponents(UItemTakeRight::StaticClass());
        UItemTakeRight* takeComp = data.actor ? Cast<UItemTakeRight>(data.components[0]) : nullptr;
        if (takeComp) SERVER_TakeRight(data.actor, takeComp);
    }
}

bool APlayerCharacter::SERVER_TakeRight_Validate(AItemActor* actor, UItemTakeRight* takeComp) { return true; }
void APlayerCharacter::SERVER_TakeRight_Implementation(AItemActor* actor, UItemTakeRight* takeComp) {
    MULTI_TakeRight(actor, takeComp);
}

void APlayerCharacter::MULTI_TakeRight_Implementation(AItemActor* actor, UItemTakeRight* takeComp) {
    _itemRight = actor;
    UStaticMeshComponent* mesh = _itemRight->GetStaticMeshComponent();
    if (mesh) {
        mesh->SetSimulatePhysics(false);

        mesh->AttachToComponent(GetMesh(),
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand_r"));

        mesh->RelativeLocation = takeComp->_locationAttach;
        mesh->RelativeRotation = takeComp->_rotationAttach;

        //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        //UE_LOG(LogTemp, Warning, TEXT("%s: TakeRight"), *EnumPtr->GetEnumName((int32)Role));
    }
    _itemRight->SetActorEnableCollision(false);
    _activeScenaryItems.Remove(actor);
}

bool APlayerCharacter::SERVER_DropRight_Validate() { return true; }
void APlayerCharacter::SERVER_DropRight_Implementation() {
    MULTI_DropRight();
}

void APlayerCharacter::MULTI_DropRight_Implementation() {
    UStaticMeshComponent* mesh = _itemRight->GetStaticMeshComponent();
    if (mesh) {
        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        mesh->SetSimulatePhysics(true);

        //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        //UE_LOG(LogTemp, Warning, TEXT("%s: DropRight"), *EnumPtr->GetEnumName((int32)Role));
    }
    _itemRight->SetActorEnableCollision(true);
    _itemRight = nullptr;
}

/************ SAVE LEFT **********/
void APlayerCharacter::SaveLeft() {
    if (_itemLeft) SERVER_SaveLeft(_itemLeft);
}

bool APlayerCharacter::SERVER_SaveLeft_Validate(AItemActor* itemActor) { return true; }
void APlayerCharacter::SERVER_SaveLeft_Implementation(AItemActor* itemActor) {
    MULTI_SaveLeft(itemActor);
}

void APlayerCharacter::MULTI_SaveLeft_Implementation(AItemActor* itemActor) {
    //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
    //UE_LOG(LogTemp, Warning, TEXT("%s: MULTI_SaveLeft_Implementation"), *EnumPtr->GetEnumName((int32)Role));

    SaveInventory(itemActor);
    _itemLeft = nullptr;
}

/************ SAVE RIGHT **********/
void APlayerCharacter::SaveRight() {
    if (_itemRight) SERVER_SaveRight(_itemRight);
}

bool APlayerCharacter::SERVER_SaveRight_Validate(AItemActor* itemActor) { return true; }
void APlayerCharacter::SERVER_SaveRight_Implementation(AItemActor* itemActor) {
    MULTI_SaveRight(itemActor);
}

void APlayerCharacter::MULTI_SaveRight_Implementation(AItemActor* itemActor) {
    //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
    //UE_LOG(LogTemp, Warning, TEXT("%s: MULTI_SaveRight_Implementation"), *EnumPtr->GetEnumName((int32)Role));

    SaveInventory(itemActor);
    _itemRight = nullptr;
}

/****************************************** ACTIONS **********************************************/
/*** SAVING ***/
void APlayerCharacter::SaveInventory(AItemActor* item) {
    UInventory* inventory = Cast<UInventory>(FindComponentByClass(UInventory::StaticClass()));
    if (inventory && item->FindComponentByClass(UItemSave::StaticClass())) {
        inventory->AddItem(item);
    }
}

/****************************************** AUXILIAR FUNCTIONS ***********************************/

/*** OUTSIDE ***/
void APlayerCharacter::ActivateScenaryItem(AItemActor* item) {
    _activeScenaryItems.AddUnique(item);
    _isAction = true;
}

void APlayerCharacter::DeactivateScenaryItem(AItemActor* item) {
    _activeScenaryItems.Remove(item);
    _isAction = _activeScenaryItems.Num() > 0 ? true : false;
}

ItemData APlayerCharacter::FindItemAndComponents(const TSubclassOf<UActorComponent> ComponentClass) {
    ItemData res{};
    int i = _activeScenaryItems.Num() - 1;
    while (!res.actor && i >= 0) {
        res.components = _activeScenaryItems[i]->GetComponentsByClass(ComponentClass);
        if (res.components.Num() > 0) {
            res.actor = _activeScenaryItems[i];
        }
        else {
            i--;
        }
    }
    return res;
}

void APlayerCharacter::SwitchSound(USoundWave* sound, bool stop) {
    if (_audioComp) {
        if (stop && _audioComp->IsPlaying()) {
            _audioComp->Stop();
        }
        else {
            if (!_audioComp->IsPlaying()) {
                _audioComp->SetSound(sound);
                _audioComp->Play();
            }
        }
    }
}

bool APlayerCharacter::IsAction() {
    return _isAction;
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                   class AController* EventInstigator, class AActor* DamageCauser) {
    //Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    _Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
    return DamageAmount;
}
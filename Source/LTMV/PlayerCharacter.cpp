// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerCharacter.h"

//#include "ItemTakeLeft.h"
//#include "ItemTakeRight.h"
//#include "ItemSave.h"
#include "Inventory.h"
#include "ItfUsable.h"
//#include "KeyComponent.h"
//#include "ItemActor.h"
#include "InventoryItem.h"
#include "HandPickItem.h"
#include "InventoryWidget.h"


APlayerCharacter::APlayerCharacter() {
    bReplicates = true;
    bReplicateMovement = true;

    // set our turn rates for input
    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;
    _itemLeft = nullptr;
    _itemRight = nullptr;
    _inventory = nullptr;

    _isAction = false;

    _itemLeftTaken = false;
    //_activeScenaryItems = {};

    /*RAYCAST PARAMETERS*/
    RayParameter = 250.0f;

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
    _audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
    GetOwnComponents();

    // Only create the UI on the local machine (dose not excist on the server.)
    if (GetController()->IsLocalController())
    {
        if (InventoryUIClass) // Check the selected UI class is not NULL
        {
            if (!InventoryWidget) // If the widget is not created and == NULL
            {
                InventoryWidget = CreateWidget<UInventoryWidget>(Cast<APlayerController>(GetController()),
                    InventoryUIClass.LoadSynchronous()); // Create Widget

                if (!InventoryWidget)
                    return;
                InventoryWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
                InventoryWidget->SetVisibility(ESlateVisibility::Hidden); // Set it to hidden so its not open on spawn.               
                //SetHUDVisible(false);
            }
        }
    }
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

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TERMINAR: DROP Y REPLACE DE ITEMS\nTERMINAR: HUD")));

    Super::Tick(DeltaTime);

    Raycasting();

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
    playerInput->BindAction("TakeDropRight", IE_Released, this, &APlayerCharacter::TakeDropRight);
    playerInput->BindAction("TakeDropLeft", IE_Released, this, &APlayerCharacter::TakeDropLeft);
    playerInput->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::execOnStartCrouching);
    playerInput->BindAction("Crouch", IE_Released, this, &APlayerCharacter::execOnEndCrouching);

    playerInput->BindAction("Use", IE_Released, this, &APlayerCharacter::Use);
	//Mantener push
	playerInput->BindAction("Press", IE_Pressed, this, &APlayerCharacter::Press);

    playerInput->BindAction("ShowInventory", IE_Pressed, this, &APlayerCharacter::ShowInventory);
    playerInput->BindAction("ShowInventory", IE_Released, this, &APlayerCharacter::HideInventory);

    playerInput->BindAction("PickItemFromInventory", IE_Pressed, this, &APlayerCharacter::PickItemFromInventory);
}

FHitResult APlayerCharacter::Raycasting() {

    bool bHitRayCastFlag = false;

    
    
    //FHitResult HitActor;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = _PlayerCamera->GetForwardVector() * RayParameter + StartRaycast;

    bHitRayCastFlag = GetWorld()->LineTraceSingleByChannel(hitResult, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);
    DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);

    if (bHitRayCastFlag && hitResult.Actor.IsValid()) {

        _itemFocused = ItemFocused();

        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *hitResult.Actor->GetName()));

        // COOL STUFF TO GRAB OBJECTS
        UActorComponent* actorComponent = hitResult.GetComponent();

       // components = actorComponent->GetOwner()->GetComponentsByClass(UInventory::StaticClass());
        components = actorComponent->GetOwner()->GetComponentsByClass(UActorComponent::StaticClass());

            for (UActorComponent* component : components) {
                //UInventory* itemInventory = Cast<UInventory>(component);

                //Highlight outline colors:
                //GREEN: 252 | BLUE: 253 | ORANGE: 254 | WHITE: 255
                if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())){
                    lastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                        UStaticMeshComponent::StaticClass()));

                    lastMeshFocused->SetRenderCustomDepth(true);
                    lastMeshFocused->SetCustomDepthStencilValue(252);
                    bInventoryItemHit = true;
                }
                else if (component->GetClass() == UInventoryItem::StaticClass()) {
                    lastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                        UStaticMeshComponent::StaticClass()));

                    lastMeshFocused->SetRenderCustomDepth(true);
                    lastMeshFocused->SetCustomDepthStencilValue(253);
                    bInventoryItemHit = true;
                }
                else if (component->GetClass() == UHandPickItem::StaticClass()) {
                    lastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                        UStaticMeshComponent::StaticClass()));

                    lastMeshFocused->SetRenderCustomDepth(true);
                    lastMeshFocused->SetCustomDepthStencilValue(255);
                    bInventoryItemHit = true;
                }

                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *hitResult.Actor->GetName()));
        }
    }
    
    //If Raycast is not hitting any actor, disable the outline
    if (bInventoryItemHit && hitResult.Actor != lastMeshFocused->GetOwner()) {
        //UActorComponent* actorComponent = hitResult.GetComponent();
        
        lastMeshFocused->SetCustomDepthStencilValue(0);
        lastMeshFocused->SetRenderCustomDepth(false);

        bInventoryItemHit = false;
    }
    

    return hitResult;
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

/*********** MOVEMENT ***********/

void APlayerCharacter::execOnStartCrouching()
{
    OnStartCrouch(0,0);
}

void APlayerCharacter::execOnEndCrouching()
{
    OnEndCrouch(0, 0);
}

/************** USE *************/
bool APlayerCharacter::RayCastCamera(FHitResult &hitActor) {
    FCollisionQueryParams CollisionInfo;
    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = StartRaycast + (_PlayerCamera->GetComponentRotation().Vector() * RayParameter);
    DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);
    return GetWorld()->LineTraceSingleByChannel(hitActor, StartRaycast, EndRaycast, ECC_GameTraceChannel1, CollisionInfo);
}

void APlayerCharacter::Use() {
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

/********** TAKE & DROP RIGHT HAND ***********/
//Cambiar
void APlayerCharacter::TakeDropRight() {

    if (ItemFocused()) {
        UActorComponent* takeComp = hitResult.GetActor()->GetComponentByClass(
            UActorComponent::StaticClass());

        if (!_itemRight) {
            _itemRight = hitResult.GetActor();
            _itemRight->SetReplicates(true);
            _itemRight->SetReplicateMovement(true);
            //_itemLeftTaken = true;
            if(takeComp)
                SERVER_TakeDropRight(_itemRight);
        }

        else {
            /*REPLACE RIGHT HAND ITEM*/
            SERVER_DropRight();

            _itemRight = hitResult.GetActor();
            _itemRight->SetReplicates(true);
            _itemRight->SetReplicateMovement(true);

            if(takeComp)
                SERVER_TakeDropRight(_itemRight);
        }
    }

    else {
        /*DROP RIGHT ITEM*/
        if (_itemRight) {
            SERVER_DropRight();
        }
    }

}


/****TAKE RIGHT***/
bool APlayerCharacter::SERVER_TakeDropRight_Validate(AActor* actor) { return true; }
void APlayerCharacter::SERVER_TakeDropRight_Implementation(AActor* actor) {
    MULTI_TakeDropRight(actor);
}

void APlayerCharacter::MULTI_TakeDropRight_Implementation(AActor* actor) {
    _itemRight = actor;

    UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemRight->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));

    /****GUESS TAKECOMP***/
    if (mesh && _itemRight->FindComponentByClass(UInventoryItem::StaticClass())) {
        //takeComp = Cast<UInventoryItem>(hitResult.GetActor()->GetComponentByClass(
         //   UInventoryItem::StaticClass()));

        mesh->SetMobility(EComponentMobility::Movable);
        SaveInventory(_itemRight);
        _itemRight = nullptr;
    }

    else if (mesh && _itemRight->FindComponentByClass(UHandPickItem::StaticClass())) {
        //takeComp = Cast<UHandPickItem>(hitResult.GetActor()->GetComponentByClass(
        //    UHandPickItem::StaticClass()));

        UHandPickItem* HandPickComp = Cast<UHandPickItem>(_itemRight->FindComponentByClass(
            UHandPickItem::StaticClass()));

        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetIsReplicated(true);
        mesh->SetSimulatePhysics(false);

        mesh->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform,
            TEXT("itemHand_r"));

        mesh->RelativeLocation = HandPickComp->_locationAttach_R;
        mesh->RelativeRotation = HandPickComp->_rotationAttach_R;


        //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        //UE_LOG(LogTemp, Warning, TEXT("%s: TakeLeft"), *EnumPtr->GetEnumName((int32)Role));

        _itemRight->SetActorEnableCollision(false);
    }
}

/********** TAKE & DROP LEFT HAND ***********/
//Cambiar
void APlayerCharacter::TakeDropLeft() {

    if (ItemFocused()) {
        UActorComponent* takeComp = hitResult.GetActor()->GetComponentByClass(
            UActorComponent::StaticClass());

        if (!_itemLeft) {
            _itemLeft = hitResult.GetActor();
            _itemLeft->SetReplicates(true);
            _itemLeft->SetReplicateMovement(true);

            if(takeComp)
                SERVER_TakeDropLeft(_itemLeft);
        }

        else {
            /*REPLACE LEFT HAND ITEM*/
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DEBERÍA PASAR POR AQUI...")));

            SERVER_DropLeft();

            _itemLeft = hitResult.GetActor();
            _itemLeft->SetReplicates(true);
            _itemLeft->SetReplicateMovement(true);
            
            if(takeComp)
                SERVER_TakeDropLeft(_itemLeft);
        }
    }

    else {
        /*DROP LEFT ITEM*/
        if (_itemLeft) {
            SERVER_DropLeft();
        }
    }

}

/****TAKE LEFT***/
bool APlayerCharacter::SERVER_TakeDropLeft_Validate(AActor* actor) { return true; }
void APlayerCharacter::SERVER_TakeDropLeft_Implementation(AActor* actor) {
    MULTI_TakeDropLeft(actor);
}

void APlayerCharacter::MULTI_TakeDropLeft_Implementation(AActor* actor) {
    _itemLeft = actor;
 
    UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemLeft->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));

    /****GUESS TAKECOMP***/
    if (mesh && _itemLeft->FindComponentByClass(UInventoryItem::StaticClass())) {
        //takeComp = Cast<UInventoryItem>(hitResult.GetActor()->GetComponentByClass(
        //    UInventoryItem::StaticClass()));

        mesh->SetMobility(EComponentMobility::Movable);
        SaveInventory(_itemLeft);
        _itemLeft = nullptr;
    }

    else if (mesh && _itemLeft->FindComponentByClass(UHandPickItem::StaticClass())) {
        //takeComp = Cast<UHandPickItem>(hitResult.GetActor()->GetComponentByClass(
        //    UHandPickItem::StaticClass()));

        UHandPickItem* HandPickComp = Cast<UHandPickItem>(_itemLeft->FindComponentByClass(
            UHandPickItem::StaticClass()));

        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetIsReplicated(true);
        mesh->SetSimulatePhysics(false);

        mesh->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform,
            TEXT("itemHand_l"));

        mesh->RelativeLocation = HandPickComp->_locationAttach_L;
        mesh->RelativeRotation = HandPickComp->_rotationAttach_L;


        //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        //UE_LOG(LogTemp, Warning, TEXT("%s: TakeLeft"), *EnumPtr->GetEnumName((int32)Role));

        _itemLeft->SetActorEnableCollision(false);
    }
}

bool APlayerCharacter::SERVER_DropLeft_Validate() { return true; }
void APlayerCharacter::SERVER_DropLeft_Implementation() {
    MULTI_DropLeft();
}

void APlayerCharacter::MULTI_DropLeft_Implementation() {
    if (_itemLeft) {
        UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemLeft->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
    
        if (mesh) {
            mesh->SetMobility(EComponentMobility::Movable);
            mesh->SetIsReplicated(true);
            mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
            mesh->SetSimulatePhysics(true);

            //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
            //UE_LOG(LogTemp, Warning, TEXT("%s: DropLeft"), *EnumPtr->GetEnumName((int32)Role));
        }

        if (_itemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
            UInventoryItem* inventoryItem = Cast<UInventoryItem>(_itemLeft->GetComponentByClass(
                UInventoryItem::StaticClass()));
            inventoryItem->SetEquipped(false);
            this->_inventory->RemoveItem(_itemLeft);

        }

        _itemLeft->SetActorEnableCollision(true);
        //_itemLeft->SetActorHiddenInGame(false);
        _itemLeft = nullptr;    
    
    }


}

bool APlayerCharacter::SERVER_DropRight_Validate() { return true; }
void APlayerCharacter::SERVER_DropRight_Implementation() {
    MULTI_DropRight();
}

void APlayerCharacter::MULTI_DropRight_Implementation() {

    if (_itemRight) {

        UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemRight->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));

        if (mesh) {
            mesh->SetMobility(EComponentMobility::Movable);
            mesh->SetIsReplicated(true);
            mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
            mesh->SetSimulatePhysics(true);

            //const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
            //UE_LOG(LogTemp, Warning, TEXT("%s: DropRight"), *EnumPtr->GetEnumName((int32)Role));
        }

        if (_itemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
            UInventoryItem* inventoryItem = Cast<UInventoryItem>(_itemRight->GetComponentByClass(
                UInventoryItem::StaticClass()));
            inventoryItem->SetEquipped(false);
            this->_inventory->RemoveItem(_itemRight);

        }

        _itemRight->SetActorEnableCollision(true);
        _itemRight = nullptr;
    }
    
}

/****************************************** ACTIONS **********************************************/
/*** SAVING ***/
void APlayerCharacter::SaveInventory(AActor* item) {

    this->_inventory = Cast<UInventory>(this->FindComponentByClass(UInventory::StaticClass()));
    
    if (_inventory) {
        _inventory->AddItem(item);

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SAVED: %s"), *item->GetName()));
        UE_LOG(LogTemp, Warning, TEXT("Saved: %s"), *item->GetName());
    }
}

/*** SHOW INVENTORY ***/
void APlayerCharacter::ShowInventory() {

    if (this->_inventory)
        ShowInventory_Implementation(this->_inventory);
}

void APlayerCharacter::ShowInventory_Implementation(UInventory* inventory) {
    inventory->ShowAllItems();    
    
    if(InventoryWidget)
        InventoryWidget->SetVisibility(ESlateVisibility::Visible); // Set it to hidden so its not open on spawn.               
    OnShowInventory();
}

/*** HIDE INVENTORY ***/
void APlayerCharacter::HideInventory() {
    if (this->_inventory)
        HideInventory_Implementation();
}

void APlayerCharacter::HideInventory_Implementation() {
    if (InventoryWidget)
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::SetHUDVisible(bool visible) {
    _isVisible = visible;
}

bool APlayerCharacter::IsHUDVisible() {
    return _isVisible;
}

UTexture2D* APlayerCharacter::GetItemAt(int itemIndex) {
    if (this->_inventory)
        return _inventory->GetItemAt(itemIndex);
    else return nullptr;
}

/*** TAKE ITEM FROM INVENTORY TO HAND ***/
void APlayerCharacter::PickItemFromInventory() {
    if (this->_inventory)
        this->PickItemFromInventory_Implementation("Lantern3");
}

void APlayerCharacter::PickItemFromInventory_Implementation(FString name) {
    this->_inventory = Cast<UInventory>(this->FindComponentByClass(UInventory::StaticClass()));
    AActor* ItemFromInventory = nullptr;

    ItemFromInventory = _inventory->PickItem(name);

    UStaticMeshComponent* ItemMesh = nullptr;
    UInventoryItem* InventoryItemComponent = nullptr;

    /*Obtaining item components*/
    if (ItemFromInventory) {
        ItemMesh = Cast<UStaticMeshComponent>(ItemFromInventory->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
        
        InventoryItemComponent = Cast<UInventoryItem>(ItemFromInventory->GetComponentByClass(
            UInventoryItem::StaticClass()));
    }

    if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->SetIsReplicated(true);
        ItemMesh->SetSimulatePhysics(false);
    }


    /*If there is no item in hands, attach to left hand by default*/
    if (ItemMesh && !_itemLeft) {
            
        ItemMesh->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform,
            TEXT("itemHand_l"));

        ItemMesh->RelativeLocation = InventoryItemComponent->_locationAttachFromInventory_L;
        ItemMesh->RelativeRotation = InventoryItemComponent->_rotationAttachFromInventory_L;
        ItemMesh->GetOwner()->SetActorHiddenInGame(false);

        InventoryItemComponent->SetEquipped(true);

        _itemLeft = ItemFromInventory;
    }

    else if (ItemMesh && !_itemRight) {

        ItemMesh->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform,
            TEXT("itemHand_r"));

        ItemMesh->RelativeLocation = InventoryItemComponent->_locationAttachFromInventory_R;
        ItemMesh->RelativeRotation = InventoryItemComponent->_rotationAttachFromInventory_R;
        ItemMesh->GetOwner()->SetActorHiddenInGame(false);

        InventoryItemComponent->SetEquipped(true);

        _itemRight = ItemFromInventory;
    }

}


/****************************************** AUXILIAR FUNCTIONS ***********************************/
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

bool APlayerCharacter::ItemFocused() {
    if(hitResult.GetActor())
        if (hitResult.GetActor()->GetComponentByClass(UInventoryItem::StaticClass())
            || hitResult.GetActor()->GetComponentByClass(UHandPickItem::StaticClass())) {
            return true;
        }

    return false;
}

UInventory* APlayerCharacter::GetInventory() {
    return this->_inventory;
}
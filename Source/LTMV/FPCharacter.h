// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "FPCharacter.generated.h"

UCLASS()
class LTMV_API AFPCharacter : public APlayerCharacter {
    GENERATED_BODY()

public:
	UPROPERTY()
	bool bToBlack = false;

    /* HUD */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
    TSubclassOf<class UUserWidget> _HUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> _HUDClass2;

    AFPCharacter(const FObjectInitializer& OI);
    void BeginPlay() override;
    void AfterPossessed(bool SetInventory, bool respawning) override;
    void Tick(float DeltaSeconds) override;

    /************* INVENTORY ************/
    // The class that will be used for the players Inventory UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<class UInventoryWidget> _InventoryUIClass;

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    UTexture2D* GetItemTextureAt(int itemIndex);


	UFUNCTION(BlueprintCallable, Category = "FadeScreen")
	void FadeDisplay();

    /************** PICK ITEM *************/
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PickItemInventory(AActor* ItemActor, FKey KeyStruct);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_PickItemInventoryLeft(AActor* ItemActor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_PickItemInventoryLeft(AActor* ItemActor);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_PickItemInventoryRight(AActor* ItemActor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_PickItemInventoryRight(AActor* ItemActor);

    /******** USE ITEM LEFT *********/
    void UseLeftPressed(bool IsMenuHidden) override;
    void UseLeftReleased(bool IsMenuHidden) override;

    /******* USE ITEM RIGHT *********/
    void UseRightPressed(bool IsMenuHidden) override;
    void UseRightReleased(bool IsMenuHidden) override;

    void ToggleMenuInteraction(bool Activate) override;

	/********** UPDATE ANIMATIONS ***********/
	UPROPERTY(BlueprintReadOnly)
	bool GrabbingLeft;
	UPROPERTY(BlueprintReadOnly)
	bool GrabbingRight;
	UFUNCTION(BluePrintCallable)
	bool GetGrabbingLeft();
	UFUNCTION(BluePrintCallable)
	bool GetGrabbingRight();

	UFUNCTION(BluePrintCallable)
	AActor* GetItemFocused();

	/*******HUD ************/
	UUserWidget* HUD;
	UUserWidget* HUD2;
protected:

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UInventory* _Inventory;
	UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* _FirstPersonMesh;

    class UInventoryWidget* _InventoryWidget;
    bool _IsInventoryHidden;

	class UTutorial* _Tutorial;
	class UTutorialVR* _TutorialVR;

    void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    /*************************************** ACTION MAPPINGS *************************************/
    /************** USE *************/
    void UsePressed();
    void UseReleased();

public:
    /********** TAKE ITEM ***********/
    void TakeDropRight();
    void TakeDropLeft();

	virtual void TakeDropRight_Respawn(AActor* actor) override;

	/********* DROP ITEM REIMPLEMENTATION ************/
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void SERVER_Drop(AActor* ItemActor, int Hand) override;
	UFUNCTION(NetMulticast, Reliable)
		virtual void MULTI_Drop(AActor* ItemActor, int Hand) override;

	void HideInventory();

protected:

    /********** INVENTORY ***********/
    void ToggleInventory();


    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveItemInventory(AActor* ItemActor, int Hand);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveItemInventory(AActor* ItemActor, int Hand);
    /* 
    Hand = 0 => void
    Hand = 1 => _ItemLeft
    Hand = 2 => _ItemRight
    */

    /* RAYCASTING  */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    FHitResult Raycasting();

private:
    float _RayParameter;
    FHitResult _HitResult;
	FHitResult _LastPressed;

    
    UStaticMeshComponent* _LastMeshFocused = nullptr;
    bool bInventoryItemHit = false;

public:
    FORCEINLINE UInventory* AFPCharacter::GetInventory() const { return _Inventory; }
};

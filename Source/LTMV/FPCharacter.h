// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "FPCharacter.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETutorialLevelEnum : uint8
{
	TL_0_1 	UMETA(DisplayName = "0_1"),
	TL_0 	UMETA(DisplayName = "0"),
	TL_1 	UMETA(DisplayName = "1"),
	TL_2	UMETA(DisplayName = "2"),
	TL_3	UMETA(DisplayName = "3"),
	TL_4	UMETA(DisplayName = "4"),
	TL_5	UMETA(DisplayName = "5"),
	TL_6	UMETA(DisplayName = "6"),
	TL_7	UMETA(DisplayName = "7"),
	TL_8	UMETA(DisplayName = "8"),
	TL_Disabled		UMETA(DisplayName = "Disabled")
};

UCLASS()
class LTMV_API AFPCharacter : public APlayerCharacter {
    GENERATED_BODY()

public:
    /* HUD */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
    TSubclassOf<class UUserWidget> _HUDClass;

    AFPCharacter(const FObjectInitializer& OI);
    void BeginPlay() override;
    void AfterPossessed(bool SetInventory) override;
    void Tick(float DeltaSeconds) override;

    /************* INVENTORY ************/
    // The class that will be used for the players Inventory UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<class UInventoryWidget> _InventoryUIClass;

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    UTexture2D* GetItemTextureAt(int itemIndex);

	/************* TUTORIAL ************/
	// The class that will be used for the players Inventory UI
	/*
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial")
		TSubclassOf<class UUserWidget> _TutorialWidget0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial")
		TSubclassOf<class UUserWidget> _TutorialWidget1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial")
		TSubclassOf<class UUserWidget> _TutorialWidget2;
		*/
	// The enum to control Tutorial Level
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enum)
		ETutorialLevelEnum _TutorialLevelEnum;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial0;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial1;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial2;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial3;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial4;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial5;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial6;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial7;
	UPROPERTY(Category = "Tutorial", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTutorialWidgetComponent* _Tutorial8;

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

protected:

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UInventory* _Inventory;
	UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* _FirstPersonMesh;

    class UInventoryWidget* _InventoryWidget;
    bool _IsInventoryHidden;

	class UTutorial* _Tutorial;

	bool _IsTutorialRunning;
	void ToggleTutorial();
	float _TutorialTimer;

    void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    /*************************************** ACTION MAPPINGS *************************************/
    /************** USE *************/
    void UsePressed();
    void UseReleased();

    /********** TAKE ITEM ***********/
    void TakeDropRight();
    void TakeDropLeft();

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

    AActor* GetItemFocused();

    /* RAYCASTING  */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    FHitResult Raycasting();

private:
    float _RayParameter;
    FHitResult _HitResult;
    
    UStaticMeshComponent* _LastMeshFocused = nullptr;
    bool bInventoryItemHit = false;

public:
    FORCEINLINE UInventory* AFPCharacter::GetInventory() const { return _Inventory; }
};

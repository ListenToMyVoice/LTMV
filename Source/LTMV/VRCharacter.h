// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "MotionControllerComponent.h"
#include "VRCharacter.generated.h"

class UGrabItem;

UENUM(BlueprintType)
enum class EGripEnum : uint8 {
    Open 	UMETA(DisplayName = "Open"),
    CanGrab UMETA(DisplayName = "CanGrab"),
    Grab	UMETA(DisplayName = "Grab")
};

UCLASS()
class LTMV_API AVRCharacter : public APlayerCharacter {
    GENERATED_BODY()

public:
	UPROPERTY()
	bool bToBlack = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rumble")
    UForceFeedbackEffect * _RumbleOverLapLeft;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rumble")
    UForceFeedbackEffect * _RumbleOverLapRight;

    UPROPERTY(BlueprintReadOnly)
    EGripEnum _GripStateLeft;
    UPROPERTY(BlueprintReadOnly)
    EGripEnum _GripStateRight;

    AVRCharacter(const FObjectInitializer& OI);
    virtual void BeginPlay() override;
	virtual void AfterPossessed(bool SetInventory, bool respawning) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInput) override;
    virtual void Tick(float deltaTime) override;

    void ResetHMDOrigin();

	UFUNCTION(BlueprintCallable, Category = "FadeScreen")
	void FadeDisplay();

    /******** USE ITEM LEFT *********/
    void UseLeftPressed(bool IsMenuHidden) override;
    void UseLeftReleased(bool IsMenuHidden) override;

    /******* USE ITEM RIGHT *********/
    void UseRightPressed(bool IsMenuHidden) override;
    void UseRightReleased(bool IsMenuHidden) override;

	UFUNCTION(BlueprintCallable)
	AActor* GetActorFocusedLeft();
	UFUNCTION(BlueprintCallable)
	AActor* GetActorFocusedRight();

    UFUNCTION(BlueprintCallable, Category = "VR Inventory")
    void ToggleInventoryInteraction(bool bActivate);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "VR")
    bool bPositionalHeadTracking;

    /************************************* MAIN COMPONENTS ***************************************/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* _VROriginComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USteamVRChaperoneComponent* _ChaperoneComp;

    /*********************************** MOTION CONTROLLERS **************************************/
    /************ LEFT ***********/
    /* Motion Controllers */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UMotionControllerComponent* _LeftHandComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* _SM_LeftHand;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* _LeftSphere;
    /*********** RIGHT ***********/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UMotionControllerComponent* _RightHandComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* _SM_RightHand;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* _RightSphere;

    /*************** USE TRIGGER *************/
    void UseTriggerPressed(AActor* ActorFocused, USceneComponent* InParent, int Hand);
    void UseTriggerReleased(AActor* ActorFocused, USceneComponent* InParent, int Hand);

    /********** TAKE ITEM ***********/
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_GrabPress(AActor* Actor, USceneComponent* InParent, FName SocketName, int Hand);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_GrabPress(AActor* Actor, USceneComponent* InParent, FName SocketName, int Hand);

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_GrabRelease(int Hand);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_GrabRelease(int Hand);

    /********** DROP ITEM ***********/
public:
    UFUNCTION()
    void DropLeft();
    UFUNCTION()
    void DropRight();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_Drop(AActor* ItemActor, int Hand) override;
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_Drop(AActor* ItemActor, int Hand) override;

protected:
    /********* INVENTORY ********/
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UInventory* _Inventory;

    void ToggleVRInventory();

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveItemInventory(AActor* ItemActor, int Hand);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveItemInventory(AActor* ItemActor, int Hand);

public:
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

protected:
    /*********** MOVEMENT ***********/
    void MoveForward(float Value) override;
	virtual void TurnLeftComfort();
	virtual void TurnRightComfort();

    /************* IK **************/
    UFUNCTION()
    void UpdateIK();

    /********** UPDATE ANIMATIONS ***********/
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_UpdateAnimation(EGripEnum NewAnim, int Hand);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_UpdateAnimation(EGripEnum NewAnim, int Hand);

    /********** UPDATE LOCATIONS ***********/
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_UpdateComponentPosition(USceneComponent* Component, FVector Location, FRotator Rotation);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_UpdateComponentPosition(USceneComponent* Component, FVector Location, FRotator Rotation);

private:
    IHeadMountedDisplay* HMD;

    AActor* _ActorFocusedLeft;
    AActor* _ActorFocusedRight;

    AActor* _ActorGrabbing;

	AActor* _LastActorFocused = nullptr;
    
    /*** OVERLAPPING ***/
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                   bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    FGrabDelegate _GrabDelegateLeft;
    FGrabDelegate _GrabDelegateRight;

    void ItemGrabbedLeft();
    void ItemGrabbedRight();

public:
    /*** MESH FOLLOW WITH CAMERA ***/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera-Mesh follow")
    float MaxHeadTurnValue;

private:
    bool bHeadTurn;
    bool bHeadTurning;
	FVector _LastMeshPosition;

protected:
    void UpdateMeshPostitionWithCamera();
    void UpdateMeshRotationWithCamera();
	void CheckHeadTurn();
	void TurnBody();

	/* SERVER UPDATE MESH POSITION AND ROTATION */
	UFUNCTION(Server, Reliable, WithValidation)
	void SERVER_UpdateMeshWithCamera();
	UFUNCTION(NetMulticast, Reliable)
	void MULTI_UpdateMeshWithCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _MeshSpeed;

    /*** IK PROPERTIES ***/
    UPROPERTY(BlueprintReadOnly, Category = "IK")
    FRotator _HMDWorldOrientation;
    UPROPERTY(BlueprintReadOnly, Category = "IK")
    FVector _LeftControllerPosition;
    UPROPERTY(BlueprintReadOnly, Category = "IK")
    FRotator _LeftControllerOrientation;
    UPROPERTY(BlueprintReadOnly, Category = "IK")
    FVector _RightControllerPosition;
    UPROPERTY(BlueprintReadOnly, Category = "IK")
    FRotator _RightControllerOrientation;

public:
    FORCEINLINE UInventory* GetInventory() const { return _Inventory; }
};

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

    /* Toggle between Seated and Standing VR Tracking */
    void ToggleTrackingSpace();

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

    /*********************************** PSEUDOINVENTORY **************************************/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* _PouchLeft;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* _PouchRight;

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
protected:
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_Drop(AActor* ItemActor, int Hand) override;
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_Drop(AActor* ItemActor, int Hand) override;

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
	UNWGameInstance* GInstance;

    IHeadMountedDisplay* HMD;

    AActor* _ActorPouchLeft;
    AActor* _ActorPouchRight;

    AActor* _ActorFocusedLeft;
    UActorComponent* _ComponentFocusedLeft;
    AActor* _ActorFocusedRight;
    UActorComponent* _ComponentFocusedRight;
    AActor* _ActorGrabbing;

	AActor* _LastActorFocused = nullptr;

    UStaticMeshComponent* _LastMeshFocusedLeft = nullptr;
    UStaticMeshComponent* _LastMeshFocusedRight = nullptr;

    void BuildLeft();
    void BuildRight();
    
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
};

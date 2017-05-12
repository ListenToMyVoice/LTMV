// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
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
    UPROPERTY(BlueprintReadOnly)
    EGripEnum _GripStateLeft;
    UPROPERTY(BlueprintReadOnly)
    EGripEnum _GripStateRight;

    AVRCharacter(const FObjectInitializer& OI);
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInput) override;

    void SetupVROptions();
    void ResetHMDOrigin();

    /* Toggle between Seated and Standing VR Tracking */
    void ToggleTrackingSpace();

    /******** USE ITEM LEFT *********/
    void UseLeftPressed(bool IsMenuHidden) override;
    void UseLeftReleased(bool IsMenuHidden) override;

    /******* USE ITEM RIGHT *********/
    void UseRightPressed(bool IsMenuHidden) override;
    void UseRightReleased(bool IsMenuHidden) override;

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
    void UseTriggerPressed(AActor*& ActorFocused, USceneComponent* InParent, int Hand);
    void UseTriggerReleased(AActor*& ActorFocused, USceneComponent* InParent, int Hand);

    /********** TAKE ITEM ***********/
    void DropLeft();
    void DropRight();

private:
    IHeadMountedDisplay* HMD;

    AActor* _ActorFocusedLeft;
    AActor* _ActorFocusedRight;

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
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Menu.generated.h"


UCLASS()
class LTMV_API AMenu : public AActor {
	GENERATED_BODY()

public:
    bool _IsMenuHidden;

	AMenu(const class FObjectInitializer& OI);
    void ToogleMenu(FVector Location, FRotator Rotation);

    /*** BINDINGS ***/
    void OnExitGame();
    /*** GAME INSTANCE ***/
    void OnHostSession();
    void OnFindGame();
    void OnJoinGame();
    void OnBackToMenu();

    virtual void OnFindSessionComplete(FString SessionOwner);

    /*** MENU TAB ***/
    void EnableSubmenu(USceneComponent* Submenu, bool Enable);

protected:
    float MeshHeight = 40;
    FVector _SubmenuLocation = FVector(200, 0, -200);
    FRotator _SubmenuRotator = FRotator(0, 180, 0);

    /*** MENU DECORATORS ***/
    UPROPERTY(Category = "Menu Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* _TopDecorator;
    UPROPERTY(Category = "Menu Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* _BottomDecorator;
    UPROPERTY(Category = "Menu Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* _MiddleDecorator;

    virtual void BindDelegates();
    virtual void ResetMenu();
    virtual void BuildLayout();

public:
    FORCEINLINE UStaticMeshComponent* AMenu::GetTopDecorator() const { return _TopDecorator; }
    FORCEINLINE UStaticMeshComponent* AMenu::GetBottomDecorator() const { return _BottomDecorator; }
    FORCEINLINE UStaticMeshComponent* AMenu::GetMiddleDecorator() const { return _MiddleDecorator; }
};

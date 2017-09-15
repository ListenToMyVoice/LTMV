 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tutorial3D.generated.h"

UCLASS()
class LTMV_API ATutorial3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorial3D();

	void ShowTutorial(FVector Location, FRotator Rotation, int index);
	void SetLanguage(FString Language);
	void ToggleTutorial();

	UPROPERTY(Category = "Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpotLightComponent* _Light;
	UPROPERTY(Category = "Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _Decorator;
	UPROPERTY(Category = "Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* _TutCompStart;
	class UWidgetComponent* _TutComp0;
	class UWidgetComponent* _TutComp1;

	class UWidgetComponent* _TutCompStart_EN;
	class UWidgetComponent* _TutComp0_EN;
	class UWidgetComponent* _TutComp1_EN;

	class UWidgetComponent* _TutCompStart_FR;
	class UWidgetComponent* _TutComp0_FR;
	class UWidgetComponent* _TutComp1_FR;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TArray<UWidgetComponent*> _actualWidComp;
	TArray<UWidgetComponent*> _tutWidComps;
	TArray<UWidgetComponent*> _tutWidComps_EN;
	TArray<UWidgetComponent*> _tutWidComps_FR;
	TArray<bool> _tutExpected;

	UWidgetComponent* InitWidgetComp(UWidgetComponent* TutComp);
	void HideWidgets();

	TSubclassOf<class UUserWidget> _TutorialWidget;

	bool _isTutorialShown;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

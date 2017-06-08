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

	void ShowTutorial(FVector Location, FRotator Rotation);
	void HideTutorial();

	UPROPERTY(Category = "Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpotLightComponent* _Light;
	UPROPERTY(Category = "Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _Decorator;
	UPROPERTY(Category = "Decorator", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* _TutComp;

	class UUserWidget* _TutWidget;
	TSubclassOf<class UUserWidget> _TutorialWidgetStart;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<TSubclassOf<class UUserWidget>> _tutWidgets;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

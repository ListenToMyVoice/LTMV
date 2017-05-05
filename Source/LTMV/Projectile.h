// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class LTMV_API AProjectile : public AActor
{
	GENERATED_BODY()
	/** Projectile's static mesh **/
	//UPROPERTY(VisibleAnyWhere, Category = Projectile)
	//class UStaticMeshComponent* StaticMeshComponent;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	AProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int _Damage;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	//FORCEINLINE class UStaticMeshComponent* GetStaticMeshComp() const { return StaticMeshComponent; }
	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "ItemActor.h"
#include "ItemOverlap.h"
#include "PlayerCharacter.h"

AItemActor::AItemActor() : Super() {
    bReplicates = true;
    bReplicateMovement = true;
    bStaticMeshReplicateMovement = true;

    _boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBox"));
    _boxCollision->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
    //_boxCollision->SetRelativeLocation(FVector(0, 0, boxCollision->GetScaledBoxExtent().Z * 0.5f));
    _boxCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    _boxCollision->AttachToComponent(GetStaticMeshComponent(),
                                    FAttachmentTransformRules::KeepRelativeTransform);
    _boxCollision->bGenerateOverlapEvents = true;
    _boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);
    _boxCollision->OnComponentEndOverlap.AddDynamic(this, &AItemActor::OnOverlapEnd);
}

void AItemActor::BeginPlay() {
    Super::BeginPlay();
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                           AActor* OtherActor,
                           UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
        APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
        if (player != nullptr) {
            /* Activate sensed things by the actor */
            TArray<UActorComponent*> components = GetComponentsByClass(UItemOverlap::StaticClass());
            for (UActorComponent* component : components) {
                UItemOverlap* itemOverlap = Cast<UItemOverlap>(component);
                itemOverlap->activateItem(OverlappedComp, player, OtherComp, OtherBodyIndex,
                                          bFromSweep, SweepResult);
            }
        }
    }
}

void AItemActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
                         AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
        APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
        if (player != nullptr) {
            /* Deactivate sensed things by the actor */
            TArray<UActorComponent*> components = GetComponentsByClass(UItemOverlap::StaticClass());
            for (UActorComponent* component : components) {
                UItemOverlap* itemOverlap = Cast<UItemOverlap>(component);
                itemOverlap->deactivateItem(OverlappedComp, player, OtherComp, OtherBodyIndex);
            }
        }
    }
}
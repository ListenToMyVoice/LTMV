// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Item.h"

#ifndef __LIBRARYUTILS_H
#include "LibraryUtils.h"
#define __LIBRARYUTILS_H
#endif

AItem::AItem() : Super() {
    boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBox"));
    boxCollision->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
    boxCollision->SetRelativeLocation(FVector(0, 0, boxCollision->GetScaledBoxExtent().Z * 0.5f));
    boxCollision->AttachToComponent(GetStaticMeshComponent(),
                                    FAttachmentTransformRules::KeepRelativeTransform);
    boxCollision->bGenerateOverlapEvents = true;
    boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
    boxCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

void AItem::BeginPlay() {
    Super::BeginPlay();
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                           AActor* OtherActor,
                           UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
        APlayerCharacter* other = Cast<APlayerCharacter>(OtherActor);
        if (other != nullptr) {
            /* Activate sensed things by the actor */
            TArray<UActorComponent*> components = GetComponentsByClass(UItemOverlap::StaticClass());
            for (UActorComponent* component : components) {
                UItemOverlap* itemOverlap = Cast<UItemOverlap>(component);
                itemOverlap->activateItem(OverlappedComp, other, OtherComp, OtherBodyIndex,
                                          bFromSweep, SweepResult);
            }
        }
    }
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
                         AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
        APlayerCharacter* other = Cast<APlayerCharacter>(OtherActor);
        if (other != nullptr) {
            /* Deactivate sensed things by the actor */
            TArray<UActorComponent*> components = GetComponentsByClass(UItemOverlap::StaticClass());
            for (UActorComponent* component : components) {
                UItemOverlap* itemOverlap = Cast<UItemOverlap>(component);
                itemOverlap->deactivateItem(OverlappedComp, other, OtherComp, OtherBodyIndex);
            }
        }
    }
}
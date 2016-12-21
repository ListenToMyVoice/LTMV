// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Item.h"
#include "PlayerCharacter.h"
#include "LibraryUtils.h"

AItem::AItem() : Super() {
    boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBox"));
    boxCollision->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
    boxCollision->SetRelativeLocation(FVector(0, 0, boxCollision->GetScaledBoxExtent().Z * 0.5f));
    boxCollision->AttachToComponent(GetStaticMeshComponent(),
                                    FAttachmentTransformRules::KeepRelativeTransform);

    boxCollision->bGenerateOverlapEvents = true;
    boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
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
            ULibraryUtils::Log(GetName(), 1);
        }
        else {
            ULibraryUtils::Log(other->GetName());
        }
    }
}
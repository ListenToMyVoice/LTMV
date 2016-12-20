// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Item.h"
#include "PlayerCharacter.h"

AItem::AItem() : Super() {
    boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBox"));
    boxCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    boxCollision->SetRelativeLocation(FVector(0, 0, boxCollision->GetScaledBoxExtent().Z * 0.5f));
    boxCollision->AttachToComponent(GetStaticMeshComponent(),
                                    FAttachmentTransformRules::KeepRelativeTransform);

    boxCollision->bGenerateOverlapEvents = true;
    boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
}

void AItem::BeginPlay() {
    Super::BeginPlay();
    OnActorBeginOverlap.AddDynamic(this, &AItem::OnOverlapActorBegin);
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                           AActor* OtherActor,
                           UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("COMPONENT"));
    //if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
    //    //UGameplayStatics::GetPlayerController(GetWorld(), 0);
    //    APlayerCharacter* other = Cast<APlayerCharacter>(OtherActor);
    //    if (other != nullptr) {
    //        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PLAYER"));
    //    }
    //    else {
    //        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NO PLAYER"));
    //    }
    //}
}

void AItem::OnOverlapActorBegin(class AActor* OverlappedActor, class AActor* OtherActor) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ACTOR"));
}

//void AItem::Tick(float DeltaSeconds) {
//    Super::Tick(DeltaSeconds);
//    //boxCollision->IsOverlappingActor();
//}
// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include "EnemyCharacter.h"


UBTTask_Attack::UBTTask_Attack() {
    BlackboardKey.SelectedKeyName = FName("PlayerActor");
    BlackboardKey.SelectedKeyType = UBlackboardKeyType_Object::StaticClass();
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->
                                          GetValueAsObject(BlackboardKey.SelectedKeyName));
    ACharacter* Self = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->
                                                            GetValueAsObject(FName("SelfActor")));
    if (Target && Self) {
        FDamageEvent DamageEvent;
        if (Target->TakeDamage(50, DamageEvent, Self->GetController(), Self) <= 0) {
            OwnerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName,
                                                                 nullptr);
        }
        return EBTNodeResult::Type::Succeeded;
    }

    return EBTNodeResult::Type::Failed;
}
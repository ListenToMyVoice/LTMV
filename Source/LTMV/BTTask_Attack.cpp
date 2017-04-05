// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UBTTask_Attack::UBTTask_Attack() {
    BlackboardKey.SelectedKeyName = FName("TargetPawn");
    BlackboardKey.SelectedKeyType = UBlackboardKeyType_Object::StaticClass();
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->
                                          GetValueAsObject(BlackboardKey.SelectedKeyName));
    APawn* Self = Cast<APawn>(OwnerComp.GetBlackboardComponent()->
                                        GetValueAsObject(FName("SelfActor")));
    if (Target && Self) {
        FDamageEvent DamageEvent;
        Target->TakeDamage(1, DamageEvent, Self->GetController(), Self);
        return EBTNodeResult::Type::Succeeded;
    }

    return EBTNodeResult::Type::Failed;
}
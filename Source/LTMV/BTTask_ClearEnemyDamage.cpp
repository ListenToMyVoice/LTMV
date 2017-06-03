// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "BTTask_ClearEnemyDamage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "EnemyCharacter.h"


UBTTask_ClearEnemyDamage::UBTTask_ClearEnemyDamage() {
	BlackboardKey.SelectedKeyName = FName("Damaged?");
	BlackboardKey.SelectedKeyType = UBlackboardKeyType_Bool::StaticClass();
}

EBTNodeResult::Type UBTTask_ClearEnemyDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AEnemyCharacter* Self = Cast<AEnemyCharacter>(OwnerComp.GetBlackboardComponent()->
		GetValueAsObject(FName("SelfActor")));
	if (Self) {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Damaged?"), false);
		Self->damaged = false;
		return EBTNodeResult::Type::Succeeded;
	}

	return EBTNodeResult::Type::Failed;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearEnemyDamage.generated.h"

/**
 * 
 */
UCLASS()
class LTMV_API UBTTask_ClearEnemyDamage : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearEnemyDamage();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};

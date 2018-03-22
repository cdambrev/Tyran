// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "SelectorUtility.generated.h"


/**
 * 
 */
UCLASS()
class TYRAN_API USelectorUtility : public UBTComposite_Selector
{
	GENERATED_BODY()
		
	
		
public:
	USelectorUtility();
	void sortChildren(UBehaviorTreeComponent & OwnerComp);
	void updateBlackboardUtilityNode(UBehaviorTreeComponent & OwnerComp, UBTTaskNode* node);
};

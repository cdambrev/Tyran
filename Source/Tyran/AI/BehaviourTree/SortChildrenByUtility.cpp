// Fill out your copyright notice in the Description page of Project Settings.

#include "SortChildrenByUtility.h"
#include "SelectorUtility.h"
#include "BTTaskNodeUtility.h"

USortChildrenByUtility::USortChildrenByUtility()
{
	bCallTickOnSearchStart = true;
}


void USortChildrenByUtility::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	USelectorUtility *selectorUtility = Cast<USelectorUtility>(GetParentNode());
	if (selectorUtility) {
		selectorUtility->sortChildren(OwnerComp);
	}
	
}

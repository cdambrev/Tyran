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
	SortChildrenByUtility();
}

void USortChildrenByUtility::SortChildrenByUtility() {
	Children->Sort([](const FBTCompositeChild& a, const FBTCompositeChild& b) {
		return static_cast<UBTTaskNodeUtility *>(a.ChildTask)->GetUtility() > static_cast<UBTTaskNodeUtility *>(b.ChildTask)->GetUtility();
	});

}

void USortChildrenByUtility::setChildrenArray(TArray<FBTCompositeChild> * c)
{
	Children = c;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectorUtility.h"

#include <algorithm>
#include "BTTaskNodeUtility.h"

USelectorUtility::USelectorUtility() {
	UE_LOG(LogTemp, Warning, TEXT("Services Size : %d"), Services.Num());
}

void USelectorUtility::sortChildren(UBehaviorTreeComponent & OwnerComp)
{
	for (FBTCompositeChild c : Children) {
		dynamic_cast<UBTTaskNodeUtility *>(c.ChildTask)->CalculUtility(OwnerComp);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Sorting, Children Size : %d"), Children.Num());
	Children.Sort([](const FBTCompositeChild& a, const FBTCompositeChild& b) {
		return dynamic_cast<UBTTaskNodeUtility *>(a.ChildTask)->GetUtility() > dynamic_cast<UBTTaskNodeUtility *>(b.ChildTask)->GetUtility();
	});
	for (FBTCompositeChild c : Children) {
		//UE_LOG(LogTemp, Error, TEXT("%s  - Utility : %f"), *(c.ChildTask->GetStaticDescription()), static_cast<UBTTaskNodeUtility *>(c.ChildTask)->GetUtility());
	}
}




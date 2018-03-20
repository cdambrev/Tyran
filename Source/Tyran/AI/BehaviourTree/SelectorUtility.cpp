// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectorUtility.h"

#include <algorithm>
#include "BTTaskNodeUtility.h"
#include "AI/AIGuardController.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 

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
	updateBlackboardUtilityNode(OwnerComp, Children[0].ChildTask);
}


void USelectorUtility::updateBlackboardUtilityNode(UBehaviorTreeComponent & OwnerComp, UBTTaskNode* node) {
	FString str = node->GetClass()->GetName();
	static_cast<AAIGuardController*>(OwnerComp.GetAIOwner())->GetBlackboardComponent()->SetValueAsString("UtilityNode", str);
}

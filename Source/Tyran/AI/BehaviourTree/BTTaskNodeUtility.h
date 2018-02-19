// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeUtility.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API UBTTaskNodeUtility : public UBTTaskNode
{
	GENERATED_BODY()
public:
		float GetUtility() const;
		virtual void CalculUtility(UBehaviorTreeComponent & OwnerComp);
		void clampUtility();
		bool operator>(const UBTTaskNodeUtility &autre);
protected:
	float utility = 0.0f;
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SortChildrenByUtility.generated.h"
struct FBTCompositeChild;
/**
 * 
 */
UCLASS()
class TYRAN_API USortChildrenByUtility : public UBTService
{

	GENERATED_BODY()

private:
	TArray<FBTCompositeChild> * Children;

public:
	USortChildrenByUtility();
	//Sera appele a chaque update du service
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	void SortChildrenByUtility();
	void setChildrenArray(TArray<FBTCompositeChild> * c);

	
};

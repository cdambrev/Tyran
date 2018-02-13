// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectorUtility.h"

#include <algorithm>
#include "BTTaskNodeUtility.h"

USelectorUtility::USelectorUtility() {
	sortChildren = CreateDefaultSubobject<USortChildrenByUtility>(TEXT("SortChildren"));
	sortChildren->setChildrenArray(&Children);
	Services.Add(sortChildren);
}





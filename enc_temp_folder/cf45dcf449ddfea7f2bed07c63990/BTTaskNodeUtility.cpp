// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNodeUtility.h"


float UBTTaskNodeUtility::GetUtility() const {
	return utility;
}

void UBTTaskNodeUtility::CalculUtility() {
	utility = 0;
}

bool UBTTaskNodeUtility::operator<(const UBTTaskNodeUtility &autre) {
	return GetUtility() < autre.GetUtility();
}
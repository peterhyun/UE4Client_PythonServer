#pragma once
#include "CoreMinimal.h"
#include "def.h"

FVector Orthogonormalize(FVector v1, FVector v2);
FMatrix ToRotMatrix(FVector v1, FVector v2);
void HermiteSpline(TArray<FVector> vectors, TArray<FVector>* arr, int sample, float speed);
FVector AverageVector(TArray<FVector> arr, int start);
void CurveFitting(TArray<FVector>* ret, TArray<FVector> arr, FVector startPoint, FVector endPoint);
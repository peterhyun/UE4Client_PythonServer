#include "MathFunc.h"

FMatrix ToRotMatrix(FVector v1, FVector v2)
{
	FVector q1 = v1;
	FVector q2 = Orthogonormalize(v1, v2);
	FVector q3 = FVector::CrossProduct(q1, q2).GetSafeNormal();
	FVector w = FVector(0, 0, 0);
	FMatrix rotMat = FMatrix(q1, q2, q3, w);

	return rotMat;
}

FVector Orthogonormalize(FVector v1, FVector v2)
{
	FVector orth = v2 - (FVector::DotProduct(v1, v2) / v1.Size()) * v1;
	FVector norm = orth.GetSafeNormal();
	return norm;
}

void HermiteSpline(TArray<FVector> vectors, TArray<FVector>* arr, int sample, float speed)
{
	for (int tick = 0; tick < sample; tick++)
	{
		float t = (1.0 * tick) / sample;
		float t2 = t * t;
		float t3 = t * t * t;

		FVector interp_point = (2 * t3 - 3 * t2 + 1) * vectors[0] +
			(-2 * t3 + 3 * t2) * vectors[1] +
			(t3 - 2 * t2 + t) * (vectors[2] - vectors[0]) * speed +
			(t3 - t2) * (vectors[3] - vectors[1]) * speed;
		(*arr).Add(interp_point);
	}
}

FVector AverageVector(TArray<FVector> arr, int start)
{
	FVector average = FVector(0, 0, 0);

	for (int i = 0; i < HERMITE_AVERAGE_LEN; i++)
	{
		average += arr[start + i];
	}

	average /= (1.0 * HERMITE_AVERAGE_LEN);

	return average;
}

void CurveFitting(TArray<FVector>* ret, TArray<FVector> arr, FVector startPoint, FVector endPoint)
{
	int num_of_blocks = (arr.Num() - 1) / HERMITE_SAMPLE_RATE - 1;

	for (int i = 0; i < num_of_blocks; i++)
	{
		TArray<FVector> points;

		if (i == 0) points.Add(startPoint);
		else points.Add(AverageVector(arr, i * HERMITE_SAMPLE_RATE));

		points.Add(AverageVector(arr, (i + 1) * HERMITE_SAMPLE_RATE));

		if (i == 0) points.Add(startPoint); // 0 speed
		else points.Add(AverageVector(arr, i * HERMITE_SAMPLE_RATE + 1));

		points.Add(AverageVector(arr, (i + 1) * HERMITE_SAMPLE_RATE + 1));

		HermiteSpline(points, ret, HERMITE_SAMPLE_RATE, HERMITE_SAMPLE_RATE);
	}

	// Last
	{
		TArray<FVector> points;

		points.Add(AverageVector(arr, num_of_blocks * HERMITE_SAMPLE_RATE));
		points.Add(endPoint);
		points.Add(AverageVector(arr, num_of_blocks * HERMITE_SAMPLE_RATE + 1));
		points.Add(endPoint); // 0 speed

		HermiteSpline(points, ret, arr.Num() - num_of_blocks * HERMITE_SAMPLE_RATE, arr.Num() - num_of_blocks * HERMITE_SAMPLE_RATE);
	}
}
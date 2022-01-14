// Fill out your copyright notice in the Description page of Project Settings.

#include "FAnimNode_CustomControl.h"
#include "AnimationRuntime.h"
#include "Animation/AnimInstanceProxy.h"

void FAnimNode_CustomControl::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	//Context.AnimInstanceProxy->GetAnimInstanceObject();
	Pose.Initialize(Context);
	Character = (AFastIKCharacter *)((UAnimInstance *)Context.AnimInstanceProxy->GetAnimInstanceObject())->GetOwningActor();
	UE_LOG(LogTemp, Warning, TEXT("Initialize_AnyThread Called"));
}

void FAnimNode_CustomControl::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	GetEvaluateGraphExposedInputs().Execute(Context);
	UE_LOG(LogTemp, Warning, TEXT("Update_AnyThread Called"));
	Pose.Update(Context);

	Character->SetActorLocationAndRotation(Character->baseTransform.GetLocation(), Character->baseTransform.GetRotation());
}

void FAnimNode_CustomControl::EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output)
{
	Pose.EvaluateComponentSpace(Output);
	UE_LOG(LogTemp, Warning, TEXT("EvaluateComponentSpace_AnyThread Called"));

	if (Character == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Character is NULL"));
	}


	if (Character != NULL && Character->State == 0 && Character->boneTransforms.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("SafeSetCSboneTransfomrs Called"));
		
		Output.Pose.SafeSetCSBoneTransforms(Character->boneTransforms);
	}
}
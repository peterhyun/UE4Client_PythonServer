// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimNodeBase.h"
#include "Animation/AnimInstance.h"
#include "FastIKCharacter.h"
#include "FAnimNode_CustomControl.generated.h"

/**
 * 
 */
USTRUCT(BlueprintInternalUseOnly)
struct FASTIKCLIENT_API FAnimNode_CustomControl : public FAnimNode_Base
{
	GENERATED_BODY()

private:
	AFastIKCharacter* Character;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
		FComponentSpacePoseLink Pose;
public:
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output) override;
};

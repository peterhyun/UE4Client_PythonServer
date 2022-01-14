// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "FAnimNode_CustomControl.h"
#include "AnimGraphNode_CustomControl.generated.h"

/**
 * 
 */
UCLASS()
class FASTIKCLIENT_API UAnimGraphNode_CustomControl : public UAnimGraphNode_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
		FAnimNode_CustomControl Node;
public:
	virtual void CreateOutputPins() final;
};

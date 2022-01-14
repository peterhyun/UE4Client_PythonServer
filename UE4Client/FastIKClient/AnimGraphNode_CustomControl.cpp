// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimGraphNode_CustomControl.h"
#include "AnimationGraphSchema.h"

//Node Output Pin (Change at own RISK!)
void UAnimGraphNode_CustomControl::CreateOutputPins()
{
    const UAnimationGraphSchema* Schema = GetDefault<UAnimationGraphSchema>();
    CreatePin(EGPD_Output, Schema->PC_Struct, TEXT("Hello World"), FComponentSpacePoseLink::StaticStruct(), /*bIsArray=*/ false, /*bIsReference=*/ false, TEXT("OutputPose"));
}
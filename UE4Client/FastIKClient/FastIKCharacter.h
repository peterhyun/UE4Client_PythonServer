// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TCPClient.h"
#include "FastIKCharacter.generated.h"

#define NUM_JOINT 22
#define TOTAL_FRAMES 707

UCLASS()
class FASTIKCLIENT_API AFastIKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFastIKCharacter();

	TArray<FBoneTransform> boneTransforms;
	FTransform baseTransform;

	UTCPTalker* tcpClient;
	char* buffer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type reason) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimFrame")
		int animFrame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variable")
		int State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursorLoc")
		FVector cursorGlobal;

	void updateBoneTransforms();
};

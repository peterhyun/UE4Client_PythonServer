// Fill out your copyright notice in the Description page of Project Settings.


#include "FastIKCharacter.h"
#include "Engine.h"
// Sets default values
AFastIKCharacter::AFastIKCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void AFastIKCharacter::BeginPlay()
{
	Super::BeginPlay();

	animFrame = 0;

	State = -1;

	boneTransforms = TArray<FBoneTransform>();

	TArray<FName> bone_name;
	//Set the bones you will rotate. If you assign the names they will find it.
	bone_name.Add(FName("SHE_Hips"));
	bone_name.Add(FName("SHE_RightUpLeg"));
	bone_name.Add(FName("SHE_RightLeg"));
	bone_name.Add(FName("SHE_RightFoot"));
	bone_name.Add(FName("SHE_RightFootMiddle1"));
	bone_name.Add(FName("SHE_LeftUpLeg"));
	bone_name.Add(FName("SHE_LeftLeg"));
	bone_name.Add(FName("SHE_LeftFoot"));
	bone_name.Add(FName("SHE_LeftFootMiddle1"));
	bone_name.Add(FName("SHE_Spine"));
	bone_name.Add(FName("SHE_Spine1"));
	bone_name.Add(FName("SHE_Spine2"));
	bone_name.Add(FName("SHE_RightShoulder"));
	bone_name.Add(FName("SHE_RightArm"));
	bone_name.Add(FName("SHE_RightForeArm"));
	bone_name.Add(FName("SHE_RightHand"));
	bone_name.Add(FName("SHE_LeftShoulder"));
	bone_name.Add(FName("SHE_LeftArm"));
	bone_name.Add(FName("SHE_LeftForeArm"));
	bone_name.Add(FName("SHE_LeftHand"));
	bone_name.Add(FName("SHE_Neck"));
	bone_name.Add(FName("SHE_Head"));

	for (int i = 0; i < bone_name.Num(); i++) {
		boneTransforms.Add(FBoneTransform(FCompactPoseBoneIndex(GetMesh()->GetBoneIndex(bone_name[i])), FTransform::Identity));
	}

	tcpClient = new UTCPTalker();
	
	buffer = (char*)malloc(sizeof(char) * 2000);

	State = tcpClient->clientTalk();
}

// Called every frame
void AFastIKCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Uncomment this when you want to print the animation per frame
	/*
	if (tcpClient->sendIndexAndReceiveData(buffer, ((NUM_JOINT + 1) * 4 * 4) * sizeof(float), animFrame++)) {
		//UE_LOG(LogTemp, Warning, TEXT("Mouse Cursor: %f, %f, %f"), cursorGlobal.X, cursorGlobal.Y, cursorGlobal.Z);
		updateBoneTransforms();
		if (animFrame == 707) {
			animFrame = 0;
		}
	}
	*/
	// Use this for TF
	if (tcpClient->sendCursorPosAndReceiveData(buffer, ((NUM_JOINT + 1) * 4 * 4) * sizeof(float), cursorGlobal)) {
		//UE_LOG(LogTemp, Warning, TEXT("Mouse Cursor: %f, %f, %f"), cursorGlobal.X, cursorGlobal.Y, cursorGlobal.Z);
		updateBoneTransforms();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Sending and Receiving Data failed. Check connection"));
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
		return;
	}
}

// Called to bind functionality to input
void AFastIKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FTransform ChangeUEOrientation(FMatrix transform, bool bAdjust=false)
{
	FTransform joint_transform(transform);
	if (bAdjust) {
		FVector rotVect = joint_transform.GetRotation().Euler(); // Roll, Pitch, Yaw
		FQuat newRot = FQuat(FRotator(rotVect[1], rotVect[2], rotVect[0] + 90.0)); // Pitch, Yaw, Roll	
		joint_transform.SetRotation(newRot);
	}
	
	return joint_transform;
}

//index is joint index
void AFastIKCharacter::updateBoneTransforms() {
	std::vector<float> data = tcpClient->DataFromBinaryArray(buffer, (NUM_JOINT + 1) * 4 * 4);

	// Root motion
	FMatrix rootMotion;
	for (int column = 0; column < 4; column++) {
		for (int row = 0; row < 4; row++) {
			rootMotion.M[row][column] = data[column * 4 + row];
		}
	}	
	baseTransform = ChangeUEOrientation(rootMotion);
	
	// Pose
	for (int jointIndex = 0; jointIndex < boneTransforms.Num(); jointIndex++) {
		FMatrix transform;
		for (int column = 0;column < 4; column++) {
			for (int row = 0;row < 4; row++) {
				transform.M[row][column] = data[(jointIndex + 1) * 4 * 4 + column * 4 + row ];
			}
		}
		boneTransforms[jointIndex].Transform = ChangeUEOrientation(transform, true);
	}
}

void AFastIKCharacter::EndPlay(EEndPlayReason::Type reason) {
	free (buffer);
}
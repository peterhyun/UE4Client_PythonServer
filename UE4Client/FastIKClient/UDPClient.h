// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Networking/Public/Networking.h"
#include <string>
#include "UDPClient.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class FASTIKCLIENT_API UUDPClient : public UObject
{
	GENERATED_BODY()

public:
	//////////////////////////////////////////////////////////////////////////
	// Construction

	/**
	* Creates and initializes a new UDPNetworking object.
	*
	* param Description - The description of the socket for debug purposes.
	* param SenderSocketName - Name of the sender socket for debug purposes.
	* param TheIP - IP of the the machine you want to send a message too.
	* param ThePort - The port of the machine you are trying to talk to.
	* param BufferSize - The size of the buffer for the socket
	* param AllowBroadCast - Allow broadcasting on this socket?
	* param Bound - Bind socket to the ip and port?
	* param Reusable - Is this socket reusable?
	* param Blocking - Is this socket blocking other data?
	*/
	UFUNCTION(BlueprintPure, Category = "UDPNetworking")
		static UUDPClient* ConstructUDPClient(const FString& Description, const FString& SenderSocketName, const FString& TheIP, const int32 ThePort, const int32 BufferSize,
			const bool AllowBroadcast, const bool Bound, const bool Reusable, const bool Blocking);

	static UUDPClient* Constructor();

	/**
	* Sends the supplied message
	* param Message The message to be sent.
	*/
	UFUNCTION(BlueprintCallable, Category = "UDPNetworking")
		bool sendMessage(FString Message);

	UFUNCTION(BlueprintCallable, Category = "UDPNetworking")
		bool sendData(const TArray<float>& DataArray);


	//////////////////////////////////////////////////////////////////////////
	// Destruction and reset

	/** Destroys all data */
	UFUNCTION(BlueprintCallable, Category = "UDPNetworking")
		void UDPDestructor();

	//// Grab Data
	UFUNCTION(BlueprintCallable, Category = "UDPNetworking")
		bool anyMessages();


	/** Test Look for message */
	UFUNCTION(BlueprintCallable, Category = "UDPNetworking")
		FString GrabWaitingMessage();

	/** Test Look for pending Datas */
	UFUNCTION(BlueprintCallable, Category = "UDPNetworking")
		TArray<float> GrabWaitingData();

	static FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);

	static TArray<uint8> BytesFromDataArray(const TArray<float>& DataArray);

	//Implement this 1/22 !!
	static TArray<float> DataFromBinaryArray(TArray<uint8>& BinaryArray);

	static bool isBigEndian();

private:

	// Holds the socket we are sending on
	FSocket* SenderSocket;

	// Description for debugging
	FString SocketDescription;

	// Remote Address
	FIPv4Endpoint RemoteEndPoint;
	FIPv4Address RemoteAdress;

	// Socket Subsystem
	ISocketSubsystem* SocketSubsystem;

	//UDPReceiveWorker* myRecieverWorker;
	// The data
	TArray<uint8> ReceivedData;
};
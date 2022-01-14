// Fill out your copyright notice in the Description page of Project Settings.


#include "UDPClient.h"

UUDPClient* UUDPClient::Constructor()
{
    return (UUDPClient*)StaticConstructObject_Internal(UUDPClient::StaticClass());
}

UUDPClient* UUDPClient::ConstructUDPClient(const FString& Description, const FString& SenderSocketName, const FString& TheIP, const int32 ThePort, const int32 BufferSize,
    const bool AllowBroadcast, const bool Bound, const bool Reusable, const bool Blocking)
{

    UUDPClient* wrapper = Constructor();

    UE_LOG(LogTemp, Log, TEXT("My Log"));
      
    //Set Subsystem to windows
    wrapper->SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

    TArray<FString> Array;
    TheIP.ParseIntoArray(Array, TEXT("."), true);

    //Set remote address
    wrapper->RemoteAdress = FIPv4Address(FCString::Atoi(*Array[0]), FCString::Atoi(*Array[1]), FCString::Atoi(*Array[2]), FCString::Atoi(*Array[3]));

    //Set remote endpoint
    wrapper->RemoteEndPoint = FIPv4Endpoint(wrapper->RemoteAdress, ThePort);

    // First set our socket null
    wrapper->SenderSocket = nullptr;

    if (wrapper->SocketSubsystem != nullptr) // If socket subsytem is good
    {
        UE_LOG(LogTemp, Log, TEXT("socket subsystem is good"));
        wrapper->SenderSocket = wrapper->SocketSubsystem->CreateSocket(NAME_DGram, *Description, true);
        //wrapper->SenderSocket = wrapper->SocketSubsystem->CreateSocket(NAME_Stream, *Description, false);
        //FString SocketName = TEXT("SocketName");
        //FIPv4Endpoint Endpoint(FIPv4Address::Any, 9801);
        //wrapper->SenderSocket = FTcpSocketBuilder(*SenderSocketName)
        //    .AsNonBlocking()
        //    .AsReusable()
        //    .BoundToEndpoint(Endpoint)
        //    .WithReceiveBufferSize(2 * 1024 * 1024);

        if (wrapper->SenderSocket != nullptr) // Is our socket created
        {
            UE_LOG(LogTemp, Log, TEXT("socket is created"));
            // Setup the socket
            bool Error = !wrapper->SenderSocket->SetNonBlocking(!Blocking) ||
                !wrapper->SenderSocket->SetReuseAddr(Reusable) ||
                !wrapper->SenderSocket->SetBroadcast(AllowBroadcast) ||
                !wrapper->SenderSocket->SetRecvErr();

            if (!Error)
            {
                if (Bound)
                {
                    Error = !wrapper->SenderSocket->Bind(*wrapper->RemoteEndPoint.ToInternetAddr());
                }
            }

            if (!Error)
            {
                int32 OutNewSize;

                wrapper->SenderSocket->SetReceiveBufferSize(BufferSize, OutNewSize);
                wrapper->SenderSocket->SetSendBufferSize(BufferSize, OutNewSize);
            }

            if (Error)
            {
                UE_LOG(LogTemp, Log, TEXT("FUdpSocketBuilder: Failed to create the socket as configured"));

                wrapper->SocketSubsystem->DestroySocket(wrapper->SenderSocket);
                wrapper->SenderSocket = nullptr;
            }
        }
    }
    return wrapper;
}

bool UUDPClient::sendMessage(FString Message)
{
    if (!SenderSocket) return false;

    UE_LOG(LogTemp, Log, TEXT("sendMessage called!"));

    int32 BytesSent;
    FTimespan waitTime = FTimespan(10);
    TCHAR* serializedChar = Message.GetCharArray().GetData();
    int32 size = FCString::Strlen(serializedChar);
    int32 sent = 0;

    // Send to
    //myRecieverWorker = UDPReceiveWorker::JoyInit(SenderSocket, waitTime);
    bool success = SenderSocket->SendTo((uint8*)TCHAR_TO_UTF8(serializedChar), size, BytesSent, *RemoteEndPoint.ToInternetAddr());
    if (success && BytesSent > 0) // Success
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool UUDPClient::sendData(const TArray<float>& dataArray)
{
    if (!SenderSocket) return false;

    UE_LOG(LogTemp, Log, TEXT("sendData called!"));

    int32 BytesSent;
    FTimespan waitTime = FTimespan(10);
    int32 size = dataArray.Num() * sizeof(float);
    int32 sent = 0;

    // Send to
    //myRecieverWorker = UDPReceiveWorker::JoyInit(SenderSocket, waitTime);
    TArray<uint8> bytes = BytesFromDataArray(dataArray);
    uint8* buffer = &bytes[0];
    bool success = SenderSocket->SendTo(buffer, size, BytesSent, *RemoteEndPoint.ToInternetAddr());
    if (success && BytesSent > 0) // Success
    {
        return true;
    }
    else
    {
        return false;
    }
}

FString UUDPClient::GrabWaitingMessage()
{
    uint32 Size;

    TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();

    while (SenderSocket->HasPendingData(Size))
    {
        UE_LOG(LogTemp, Log, TEXT("The size of the message is %d"), Size);
        int32 Read = 0;
        uint8 element = 0;
        ReceivedData.Init(element, FMath::Min(Size, 65507u));
        SenderSocket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), Read, *Sender);
    }

    return StringFromBinaryArray(ReceivedData);
}

TArray<float> UUDPClient::GrabWaitingData()
{
    uint32 Size;

    TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();

    while (SenderSocket->HasPendingData(Size))
    {
        UE_LOG(LogTemp, Log, TEXT("The size of the message is %d"), Size);
        int32 Read = 0;
        uint8 element = 0;
        ReceivedData.Init(element, FMath::Min(Size, 65507u));
        SenderSocket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), Read, *Sender);
    }

    return DataFromBinaryArray(ReceivedData);
}

bool UUDPClient::anyMessages()
{

    uint32 Size;

    if (SenderSocket->HasPendingData(Size))
    {
        return true;
    }

    return false;
}

FString UUDPClient::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
    //Create a string from a byte array!
    const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());
    FString layerName(cstr.c_str());
    UE_LOG(LogTemp, Log, TEXT("Converted String from binary array is: "));
    UE_LOG(LogTemp, Log, TEXT("%s"), *layerName);
    //FString can take in the c_str() of a std::string
    return FString(cstr.c_str());
}

TArray<float> UUDPClient::DataFromBinaryArray(TArray<uint8>& BinaryArray)
{

    UE_LOG(LogTemp, Log, TEXT("Called dataFromBinaryArray! "));

    unsigned int charID = BinaryArray[0];
    unsigned int numOfFloats = (BinaryArray[1]) + (BinaryArray[2] << 8) + (BinaryArray[3] << 16) + (BinaryArray[4] << 24);
    TArray<float> dataArray;
    dataArray.Add(charID);
    dataArray.Add(numOfFloats);

    if (isBigEndian()) {
        UE_LOG(LogTemp, Log, TEXT("Client is Big Endian!"));
        for (int i = 5; i < BinaryArray.Num(); i += 4) {
            BinaryArray.Swap(i, i + 3);
            BinaryArray.Swap(i + 1, i + 2);
        }
    }
    float const* data = reinterpret_cast<float const*>(&BinaryArray[5]);
    dataArray.Append(data, (BinaryArray.Num()-5) / 4);
    return dataArray;
}

bool UUDPClient::isBigEndian()
{
    int checkEndianness = 1;
    if (*(char*)&checkEndianness != 1) {
        return true;
    }
    return false;
}

TArray<uint8> UUDPClient::BytesFromDataArray(const TArray<float>& dataArray)
{
    UE_LOG(LogTemp, Log, TEXT("Called BytesFromdataArray! "));
   
    TArray<uint8> header;
    //Add the character id here
    header.Add((uint8)dataArray[0]);
    //Add the number of floats for the character here
    unsigned int numOfFloat = (unsigned int)dataArray[1];
    for (int i = 0; i < 4; i++)
        header.Add(numOfFloat >> (i * 8));

    //Add the floats here
    unsigned char const* p = reinterpret_cast<unsigned char const*>(&dataArray[2]);
    TArray<uint8> bytesOfFloats(p, dataArray.Num()*4);

    //Have to change the endianness to little endian cause that's the standard of this program.
    if (isBigEndian())
    {
        UE_LOG(LogTemp, Log, TEXT("Client is Big Endian! Gonna change the byte order to little endian"));
        for (int i = 1;i<5; i++) {
            header.Swap(i, i + 3);
            header.Swap(i + 1, i + 2);
        }
        for (int i = 5;i<bytesOfFloats.Num(); i += 4) {
            bytesOfFloats.Swap(i, i + 3);
            bytesOfFloats.Swap(i+1, i+2);
        }
    }

    header.Append(&bytesOfFloats[0], bytesOfFloats.Num());
    return header;
}

void UUDPClient::UDPDestructor()
{
    SocketSubsystem->DestroySocket(SenderSocket);
    SenderSocket = nullptr;
    SocketSubsystem = nullptr;
    //myRecieverWorker = nullptr;
}
// Fill out your copyright notice in the Description page of Project Settings.
#include "TCPClient.h"

//Convert the bytes to floats
std::vector<float> UTCPTalker::DataFromBinaryArray(char * binaryArray, int numFloats)
{
    /*
    unsigned int charID = BinaryArray[0];
    unsigned int numOfFloats = (BinaryArray[1]) + (BinaryArray[2] << 8) + (BinaryArray[3] << 16) + (BinaryArray[4] << 24);
    std::vector<float> dataArray;
    dataArray.push_back(charID);
    dataArray.push_back(numOfFloats);
    */

    std::vector<float> dataArray;
    float* data = reinterpret_cast<float*>(binaryArray);
    dataArray.insert(dataArray.end(), data, data + numFloats);
    return dataArray;
}

std::vector<std::vector<float>> UTCPTalker::DataFromBinaryArrayProtocol(char* binaryArray)
{
    std::vector<std::vector<float>> parsedData;
    int charNum = binaryArray[0];
    int indicator = 1;
    for (int i = 0; i < charNum; i++) {
        unsigned int charID = binaryArray[indicator];
        unsigned int numOfFloats = (binaryArray[indicator+1]) + (binaryArray[indicator+2] << 8) + (binaryArray[indicator+3] << 16) + (binaryArray[indicator+4] << 24);
        std::vector<float> dataArray;
        dataArray.push_back(charID);
        dataArray.push_back(numOfFloats);
        float* data = reinterpret_cast<float*>(binaryArray + indicator + 5);
        dataArray.insert(dataArray.end(), data, data + numOfFloats);
        parsedData.push_back(dataArray);
        indicator = (indicator + 5) + (numOfFloats * sizeof(float));
    }
    return parsedData;
}

std::vector<char> UTCPTalker::BytesFromFrameIndex(int frameIndex)
{
    std::vector<char> frameIndexBytes;
    char* i = reinterpret_cast<char*>(&frameIndex);
    frameIndexBytes.insert(frameIndexBytes.end(), i, i + sizeof(int));
    return frameIndexBytes;
}

std::vector<char> UTCPTalker::BytesFromDataArrayProtocol(std::vector<std::vector<float>> dataArray)
{
    std::vector<char> bytesArray;
    char numOfCharacters = dataArray.size();
    bytesArray.push_back(numOfCharacters);
    for (int i = 0;i<numOfCharacters; i++) {
       std::vector<float> charInfo = dataArray[i];
       char charID = (char)charInfo[0];
       int numFloats = (int)charInfo[1];
       std::vector<float> sliced = std::vector<float>(charInfo.begin() + 2, charInfo.end());
       bytesArray.push_back(charID);
       char * j = reinterpret_cast<char*>(&numFloats);
       bytesArray.insert(bytesArray.end(), j, j + sizeof(int));
       char * k = reinterpret_cast<char*>(&sliced[0]);
       bytesArray.insert(bytesArray.end(), k, k + sizeof(float)*numFloats);
    }
    return bytesArray;
}

std::vector<char> BytesFromDataArray(std::vector<float>& dataArray)
{
    /*
    std::vector<char> header;
    //push_back the character id here
    header.push_back((char)dataArray[0]);
    //push_back the number of floats for the character here
    unsigned int numOfFloat = (unsigned int)dataArray[1];
    for (int i = 0; i < 4; i++)
        header.push_back(numOfFloat >> (i * 8));
    */
    //push_back the floats here
    unsigned char* p = reinterpret_cast<unsigned char*>(&dataArray[0]);
    std::vector<char> bytesOfFloats;
    bytesOfFloats.insert(bytesOfFloats.end(), p, p + (dataArray.size() * sizeof(float)));
    return bytesOfFloats;
}

int UTCPTalker::clientTalk() {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        std::cout << "Error at WSAStartup().\n" << std::endl;
    else
        std::cout << "Winsock dll is available.\n" << std::endl;

    // Create a SOCKET for listening for
    // incoming connection requests
    
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket()" << std::endl;
        WSACleanup();
        return 2;
    }
    else
    {
        std::cout << "socket() is OK!\n" << std::endl;
    }

    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    // Address family - internet IPv4
    service.sin_family = AF_INET;
    // IP address
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Port number
    service.sin_port = htons(9801);

    //Connect to remote server
    if (connect(ListenSocket, (struct sockaddr*) & service, sizeof(service)) < 0)
    {
        std::cout << "connect error " << std::endl;
        return 1;
    }

    std::cout << "Connected!" << std::endl;

    return 0;
}

bool UTCPTalker::sendIndexAndReceiveData(char * buffer, int bufferSize ,int frameIndex) {
    //Send some data
    std::vector<char> bytesVector = BytesFromFrameIndex(frameIndex);
    if (send(ListenSocket, &bytesVector[0], bytesVector.size(), 0) < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Send Failed!"));
        return false;
    }
    UE_LOG(LogTemp, Warning, TEXT("Data Sent!"));
    //Receive a reply from the server
    int recv_size;
    if ((recv_size = recv(ListenSocket, buffer, bufferSize, 0)) == SOCKET_ERROR)
    {
        UE_LOG(LogTemp, Warning, TEXT("Recv Failed"));
        return false;
    }
    UE_LOG(LogTemp, Warning, TEXT("Size of data recieved from server: %d"), recv_size);
    return true;
}

bool UTCPTalker::sendCursorPosAndReceiveData(char* buffer, int bufferSize, FVector cursorPos)
{
    //Send some data
    std::vector<float> cursorPosVector;
    cursorPosVector.push_back(cursorPos.X);
    cursorPosVector.push_back(cursorPos.Y);
    cursorPosVector.push_back(cursorPos.Z);
    std::vector<char> bytesVector = BytesFromDataArray(cursorPosVector);
    if (send(ListenSocket, &bytesVector[0], bytesVector.size(), 0) < 0)
    {
        std::cout << "Send failed" << std::endl;
        return false;
    }
    UE_LOG(LogTemp, Warning, TEXT("Data Sent!"));
    //Receive a reply from the server
    int recv_size;
    if ((recv_size = recv(ListenSocket, buffer, bufferSize, 0)) == SOCKET_ERROR)
    {
        std::cout << "Recv failed" << std::endl;
        return false;
    }
    UE_LOG(LogTemp, Warning, TEXT("Size of data recieved from server: %d"), recv_size);
    return true;
}

#include "AllowWindowsPlatformTypes.h"
#include "prewindowsapi.h"

// these are our includes, yours may be different
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

//#include "INetwork.h"
#include "Networking.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//
//
// YOUR TCP HEADER CODE HERE
//
//

// UE4: disallow windows platform types
//  this was enabled at the top of the file
#include "PostWindowsApi.h"
#include "HideWindowsPlatformTypes.h"

/**
 *
 */
class FASTIKCLIENT_API UTCPTalker
{
public:
    UTCPTalker() {};

   int clientTalk();


    float* frameData;
    SOCKET ListenSocket;
    bool sendIndexAndReceiveData(char * buffer, int bufferSize, int frameIndex);
    bool sendCursorPosAndReceiveData(char* buffer, int bufferSize, FVector cursorPos);
    std::vector<float> DataFromBinaryArray(char* binaryArray, int numFloats);
    std::vector<std::vector<float>> DataFromBinaryArrayProtocol(char* binaryArray);
    std::vector<char> BytesFromFrameIndex(int frameIndex);
    std::vector<char> BytesFromDataArrayProtocol(std::vector<std::vector<float>> dataArray);
};

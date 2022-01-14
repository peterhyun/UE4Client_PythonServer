import socket
from TCPUtils import *
import numpy as np

def loadData(npa, frameIndex):
    flattenedArray = npa[frameIndex].flatten()
    return flattenedArray

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 9801        # Port to listen on (non-privileged ports are > 1023)

transformData = np.load('test_anim.npy') # 707, 27, 4, 4
reformedTransformData = transformData[:,0:22,:,:]
npa = np.array(reformedTransformData, dtype=np.float32)

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind((HOST, PORT))
serversocket.listen(1)

(conn, address) = serversocket.accept()
while True:
    print('client connected to server')
    recvdata = conn.recv(4)
    print('data received from client: ', recvdata)
    frameIndex = convertBytes2Data(recvdata)
    frame = loadData(npa, frameIndex)
    senddata = convertData2Bytes(frame)
    conn.sendall(senddata)
    print('data sended to client: ', senddata)
    #print("sent back a reply!")
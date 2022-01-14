#############################
#Utils for UDP communication#
#############################
import struct

#Get frame index
def convertBytes2Data(data):
    floats = struct.unpack_from("<"+"I", data, 0)
    return floats

#Changes Data to bytes. Format is same as convertBytes2Floats(data)
def convertData2Bytes(data):
    bunchOfBytes =  struct.pack("<"+ ("f" * (22*4*4)), *data)
    return bunchOfBytes
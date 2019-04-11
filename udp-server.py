import socket
import struct

localIP     = "0.0.0.0"
localPort   = 33000
bufferSize  = 1024

UDPServerSocket = socket.socket(
    family=socket.AF_INET,
    type=socket.SOCK_DGRAM)

UDPServerSocket.bind((localIP, localPort))

print("UDP server up and listening")

first = True
while(True):
    message, address = UDPServerSocket.recvfrom(bufferSize)
    if first:
        clientIP  = "Client IP Address:{}".format(address)
    print(
        (ord(message[0]) << 8) +
        (ord(message[1]))
    )


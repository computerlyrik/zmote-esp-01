import socket
UDP_IP = "255.255.255.255"
UDP_PORT = 9130
MESSAGE = bytearray([0xbe, 0xef])

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))


data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
print "received message:", data

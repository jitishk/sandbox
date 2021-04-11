import fcntl
import socket
import struct

srcIntf = 'Eth1-49'

s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW)
s.bind((srcIntf, 0))

ethernet  = b'\x00\x3a\x9c\x03\xb8\xc7'
ethernet += b'\x00\x3a\x9c\x07\x4d\x87'
ethernet += b'\x08\x00'                 # Protocol-Type: IPv4

ip_header  = b'\x45\x00\x00\x28'  # Version, IHL, Type of Service | Total Length
ip_header += b'\xab\xcd\x00\x00'  # Identification | Flags, Fragment Offset
ip_header += b'\x40\x06\xa6\xec'  # TTL, Protocol | Header Checksum
ip_header += b'\x0a\x0a\x0a\x02'  # Source Address
ip_header += b'\x0a\x0a\x0a\x01'  # Destination Address

payload = b'\xde\xad\xbe\xef'

tcp_header  = b'\x30\x39\x00\x50' # Source Port | Destination Port
tcp_header += b'\x00\x00\x00\x00' # Sequence Number
tcp_header += b'\x00\x00\x00\x00' # Acknowledgement Number
tcp_header += b'\x50\x02\x71\x10' # Data Offset, Reserved, Flags | Window Size
tcp_header += b'\xe6\x32\x00\x00' # Checksum | Urgent Pointer

srcMac = '\x00\x3a\x9c\x03\xb8\xc7'
dstMac = '\x00\x3a\x9c\x07\x4d\x87'
ethertype = '\x7a\x05'
#payload = 'hello world'

packet = ethernet + ip_header + tcp_header
#packet = srcMac + dstMac + ethertype + payload
packet = ethernet + ip_header + payload
s.send(packet)

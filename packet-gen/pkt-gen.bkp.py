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

tcp_header  = b'\x30\x39\x00\x50' # Source Port | Destination Port
tcp_header += b'\x00\x00\x00\x00' # Sequence Number
tcp_header += b'\x00\x00\x00\x00' # Acknowledgement Number
tcp_header += b'\x50\x02\x71\x10' # Data Offset, Reserved, Flags | Window Size
tcp_header += b'\xe6\x32\x00\x00' # Checksum | Urgent Pointer

packet = ethernet + ip_header + tcp_header
s.send(packet)



import fcntl
import socket
import struct

def getMac(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    info = fcntl.ioctl(s.fileno(), 0x8927,  struct.pack('256s', bytes(ifname, 'utf-8')[:15]))
    return ':'.join('%02x' % b for b in info[18:24])

def getIP(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    info = fcntl.ioctl( s.fileno(), 0x8915, struct.pack('256s', ifname[:15]))[20:24]
    return socket.inet_ntoa(info)


import os
srcIntf = 'Eth1-49'

# L2 MAC Header
srcMac = getMac(srcIntf)
dstMac = os.popen('arp -a | grep ' + srcIntf + ' | cut -d" " -f4').read().strip()
ethType = '0x0800' # EtherType IP payload
#destMac = stream.read().strip()

# IP Header
srcIP = getIP(srcIntf)
dstIP = os.popen('arp -a | grep "Eth1-49" | cut -d" " -f2 | sed  "s/[()]//g"')
ipIhl = 5
ipVer = 4
ipTOS = 0
ipTotalLen = 0    # kernel will fill the correct total length
ip_id = 54321    #Id of this packet
ip_frag_off = 0
ip_ttl = 255
ip_proto = socket.IPPROTO_TCP
ip_check = 0    # kernel will fill the correct checksum
ip_saddr = socket.inet_aton (srcIP)    #Spoof the source ip address if you want to
ip_daddr = socket.inet_aton (dstIP)
ipIhl_ver = (ipVer << 4) + ipIhl

# the ! in the pack format string means network order
ip_header = pack('!BBHHHBBH4s4s' , ipIhl_ver, ipTOS, ipTotalLen, ip_id, ip_frag_off, ip_ttl, ip_proto, ip_check, ip_saddr, ip_daddr)

def sendRawPacket(srcMac, dstMac, type, payload, srcIntf):
  # 48-bit Ethernet addresses
  assert(len(src) == len(dst) == 6)
  # 16-bit Ethernet type
  assert(len(type) == 2) # 16-bit Ethernet type
  s = socket(AF_PACKET, SOCK_RAW)
  s.bind((interface, 0))
  return s.send(dst + src + type + payload)

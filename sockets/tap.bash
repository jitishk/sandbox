ip tuntap add nfmtd-intf mode tap
ip tuntap add nfm-intf mode tap
ifconfig nfmtd-intf up
ifconfig nfm-intf up
brctl addbr nfm-br
brctl addif nfm-br nfmtd-intf
brctl addif nfm-br nfm-intf
iptables -A INPUT -i nfmtd-intf -j ACCEPT
iptables -A INPUT -i nfm-intf -j ACCEPT
iptables -A INPUT -i nfm-br -j ACCEPT
iptables -A FORWARD -i nfm-br -j ACCEPT

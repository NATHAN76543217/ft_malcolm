# ft_malcolm
A man-in-the-middle attack script by ARP spoofing.

# Build
//TODO add SIGTERM handler
Clone repository:
	`git clone https://NATHAN76543217/ft_malcom.git && cd ft_malcolm/ && git submodule init && git submodule update`
Run the command `make`.
Usage: `./ft_malcolm src_ip src_mac_address target_ip target_mac_address`

# options
	Usage: 
		./ft_malcolm [options] [--] source_ip source_mac dest_ip    
		./ft_malcolm [options] [--] source_ip source_mac target_ip target_mac
	ARP spoofing (poisoning) program.

		-h, --help                show this help message and exit

	Basic options
		-v, --verbose             enable verbose mode.
		-r, --reverse             enable verbose mode.
		-i, --interface=<str>     force to use a specific interface.
	
# Processus

The Address Resolution Protocol (ARP) is a `request-response` process by which a known IP address is mapped to an unknow MAC address.   
In a Linux system, arp cache is accessible into /proc/net/arp (stored in RAM).

## Packet structure
	An ARP message is always related to only one (1) address resolution.   
	The size of the ARP message depends on the link layer and network layer address sizes. For IPv4 it's 28 bytes.   
	 
Two query: 
- Request:
- Reply: 

### Header
	The message header specifies the types of network in use at each layer as well as the size of addresses of each.   
	The message header is completed with the operation code for request (1) and reply (2).    
	The payload of the packset consists of four addresses, the hardware and protocol address of the sender and receiver hosts.  
```
  /*
   *  This structure defines an ethernet header.
   */

typedef struct __attribute__((packed)) ether_header {
    uint8_t  ether_dhost[ETHER_ADDR_LEN];
    uint8_t  ether_shost[ETHER_ADDR_LEN];
    uint16_t ether_type;
} ETHERHDR;
```
```
  /*
   *  This structure defines an ethernet arp header.
   */
  
  struct arpPacket {
      __be16     		ar_hrd;     /* format of hardware address   */
      __be16     		ar_pro;     /* format of protocol address   */
      unsigned char		ar_hln;     /* length of hardware address   */
      unsigned char		ar_pln;     /* length of protocol address   */
      __be16      		ar_op;      /* ARP operation code (1 | 2)     */
  
      unsigned char		ar_sha[ETH_ALEN];	/* sender hardware address  */
      unsigned char		ar_sip[4];			/* sender IP address        */
      unsigned char		ar_tha[ETH_ALEN];	/* target hardware address  */
      unsigned char		ar_tip[4];			/* target IP address        */
  
  };
```
	- Htype 2 byte
	- Ptype 2 byte
		for IPv4 the value is :`0x0800`
	- Hlen 1 byte
	- Plen 1 byte
	- Operation 2 byte
		The value is either 1 for a request or 2 for a reply.
### Sender
	Total: 10 bytes   
	- SHA 6 bytes
	- SPA 4 bytes
### Target
	Total: 10 bytes   
	- THA 6 bytes
	- TPA 4 bytes
	 
Congif:
- PC A
- PC B
- PC C
# TODO
	Detect arp request on an interface
	then
	send an ARP reply to Target saying we are src => now target think we are SRC 
	send an ARP reply to SRC saying we are Target => now SRC think we are Target 

IP:  192.168.1.98
this IP boadcast:  192.168.1.255
standard IP boadcast:  255.255.255.255
MAC broadcast: FF:FF:FF:FF:FF:FF
Avant:
	A et B communique
Après:
	A envoie à B(C) et C transmet à B(B).
Je dois envoyer une reply normalement.


- compile argparse in Makefile
- link lib
- add option (-v, -i, -t , -p, etc...)

# Ressource
https://en.wikipedia.org/wiki/Address_Resolution_Protocol
https://stackoverflow.com/questions/12515005/receiving-udp-broadcast
https://stackoverflow.com/questions/1955198/when-is-ipproto-udp-required
https://github.com/cofyc/argparse
https://stackoverflow.com/questions/21721145/how-to-send-a-raw-ethernet-frame-using-the-netbsd-stack
https://sock-raw.org/papers/sock_raw
https://gh0std3v.github.io/posts/2020/11/getting-a-mac-address-on-a-mac/
https://tuprints.ulb.tu-darmstadt.de/6243/1/TR-18.pdf
https://jnc0x24dd099bb870.github.io/Network/ARP/ARP2/index.html
https://www.freebsd.org/cgi/man.cgi?query=bpf&sektion=4&manpath=FreeBSD+4.7-RELEASE
https://circle-networks.com/resources?lang=en&doc=bl200076p
https://www.freebsd.org/cgi/man.cgi?query=bpf&sektion=4&manpath=FreeBSD+7.1-RELEASE
Debian shortcut https://nui.fr/obsolescence-de-net-tools-ifconfig-ping/
https://security.stackexchange.com/questions/133784/does-arp-spoofing-work-on-all-lans

https://security.stackexchange.com/questions/41961/arp-poisoning-between-a-wired-and-wireless-network
https://stackoverflow.com/questions/16710040/arp-request-and-reply-using-c-socket-programming    

The `ARP Cache Poisoning`section of this link is very interesting, it report some behaviors that I encountered myself. Pour ma part j'ai réussi à faire accepter des paquets et à maintenir le spoof en place en renvoyant régulierement des paquets ARP REPLY à la target:
https://www.utc.edu/sites/default/files/2021-04/course-paper-5620-attacktcpip.pdf

## Tips
`SO_BINDTODEVICE` is deprecated since 1999. Use `bind()` with a `struct sockaddr_ll` instead   

Enable IP forwarding: 
mac:	`sysctl -w net.inet.ip.forwarding=1`    
linux:	`echo 1 > /proc/sys/net/ipv4/ip_forward`    
Broadcasting udp package: `nc -ub broadcast_addr port` on linux

# usefull for OSX
https://www.vankuik.nl/2012-02-09_Writing_ethernet_packets_on_OS_X_and_BSD

# usefull for Linux
https://man7.org/linux/man-pages/man7/packet.7.html
# Very trés trés important
	https://security.stackexchange.com/questions/197453/mitm-using-arp-spoofing-with-kali-linux-running-on-virtualbox-with-bridged-wifi



bpf0 is lo0
bpf1 is en0
bpf2 is en1
etc...

srp_ip      = qui l'on veut usurpé
src_mac     = notre addresse MAC
target_ip   = ip de la personne que l'on veut spoof
target_mac  = mac de la personne que l'on veut spoof

donc depuis mon mac si je veut recevoir tout ce qu'envoie la VM au à la box:
./ft_malcolm box my_mac_addr VM_IP VM_MAC

When sending an ARP request: always fill the source_mac_address with address of the request's sender 
When sending an ARP request: always fill the source_ip_address with the IP of the sender
When sending an ARP request: always fill the target_mac_address with 00:00:00:00:00:00
When sending an ARP request: always fill the target_ip_address with the IP to guess

When sending an ARP reply: fill the source_mac_address with the MAC address that was looking for (the response of the previous request)
When sending an ARP reply: fill the source_ip_address with the IP of the sender
When sending an ARP reply: always fill the target_mac_address with the address of the emitter of the request
When sending an ARP reply: always fill the target_ip_address with the IP of the emitter of the request

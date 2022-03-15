# include "ft_malcolm.h"



uint32_t	ipToInt(const struct sockaddr *ip)
{
	return ((struct sockaddr_in *)ip)->sin_addr.s_addr;
}

void     ipToSockaddr(const uint8_t ip[IPV4_ADDR_LEN], struct sockaddr *sock)
{
# ifdef OSX
	sock->sa_len = 16;
# endif //OSX
	sock->sa_family = AF_INET;
	ft_memcpy( &((struct sockaddr_in *) sock)->sin_addr, ip, IPV4_ADDR_LEN);
} 

# ifdef OSX
int     getIfMacAddress(struct ether_addr *ethAddr, const char* ifname)
{
	int     mib[6] = { CTL_NET, PF_ROUTE, 0, AF_LINK, NET_RT_IFLIST, 0 };
	size_t  len = 0;
	mib[5] = if_nametoindex(ifname);
	if (!mib[5]) {
		dprintf(STDERR_FILENO, "Cannot get index of interface '%s': %s\n", ifname, strerror(errno));
		return EXIT_FAILURE;
	}
	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
		dprintf(STDERR_FILENO, "Sysctl failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	char *buf = calloc(1, len);
	if (!buf) {
		dprintf(STDERR_FILENO, "Malloc buf failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
		dprintf(STDERR_FILENO, "Sysctl failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	struct if_msghdr    *ifm = (struct if_msghdr *) buf;
	struct sockaddr_dl  *sdl = (struct sockaddr_dl *)(ifm + 1);
	uint8_t *macAddr = (uint8_t *) LLADDR(sdl);
	ft_memcpy(ethAddr, macAddr, ETHER_ADDR_LEN);
	return EXIT_SUCCESS;
}

# endif //OSX

// RECV an UDP packet
// int waitUDPRequest(ft_malcolm *malc)
// {
// 	int enableBroadcast = 1;
// 	// //Create a socket enabled for receiving UDP packets
// 	if ((malc->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
// 	{
// 		dprintf(STDERR_FILENO, "Failed to initialize socket: %s\n", strerror(errno));
// 		return EXIT_FAILURE;
// 	}
// 	if (setsockopt(malc->socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
// 	{
// 		dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
// 		return EXIT_FAILURE;	
// 	}
// 	// if (setsockopt(malc->socket, SOL_SOCKET, SO_BINDTODEVICE, &malc->, sizeof(enableBroadcast)))
// 	// {
// 	// 	dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
// 	// 	return EXIT_FAILURE;	
// 	// }
// 	struct sockaddr_in si_me, si_other;
// 	memset(&si_me, 0, sizeof(si_me)); //TODO ft_memset
// 	si_me.sin_family = AF_INET;
// 	si_me.sin_port = htons(6000);
// 	// si_me.sin_addr.s_addr = INADDR_BROADCAST;
// 	si_me.sin_addr.s_addr = INADDR_ANY;
// 	// si_me.sin_addr.s_addr = inet_addr("192.168.1.98");
// 	if (bind(malc->socket, (struct sockaddr *)&si_me, sizeof(struct sockaddr)))
// 	{
// 		dprintf(STDERR_FILENO, "Socket binding failed: %s\n", strerror(errno));
// 		return EXIT_FAILURE;
// 	}
// 	char buf[10000];
// 	char buf2[42];
// 	int ret = 0;
// 	unsigned slen=sizeof(struct sockaddr);
// 	dprintf(STDOUT_FILENO, "wait incoming data\n");
// 	while(1)
// 	{
// 	    ret = recvfrom(malc->socket, buf, sizeof(buf)-1, 0, (struct sockaddr *)&si_other, &slen);
// 		buf[ret] = '\0';
// 	    dprintf(STDOUT_FILENO, "recv: %s from %s\n", buf, ipToStr(((struct sockaddr*)&si_other), buf2, 41));
// 	}
// 	return EXIT_SUCCESS;
// }

// LINUX
// int waitArpRequest(ft_malcolm *malc)
// {
// 	struct sockaddr_ll socket_address;
// 	int enableBroadcast = 1;
// 	// //Create a socket enabled for receiving ARP packets
// 	if ((malc->socket = socket(AF_INET, SOCK_RAW, htons(ETHERTYPE_ARP))) < 0)
// 	{
// 		dprintf(STDERR_FILENO, "Failed to initialize socket: %s\n", strerror(errno));
// 		return EXIT_FAILURE;
// 	}
// 	if (setsockopt(malc->socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
// 	{
// 		dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
// 		return EXIT_FAILURE;	
// 	}
// 	// if (setsockopt(malc->socket, SOL_SOCKET, SO_BINDTODEVICE, &malc->, sizeof(enableBroadcast)))
// 	// {
// 	// 	dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
// 	// 	return EXIT_FAILURE;	
// 	// }
// 	memset(&socket_address, 0, sizeof(socket_address)); //TODO ft_memset
// 	socket_address. = AF_INET;
// 	socket_address.sin_port = htons(6000);
// 	// socket_address.sin_addr.s_addr = INADDR_BROADCAST;
// 	socket_address.sin_addr.s_addr = INADDR_ANY;
// 	// socket_address.sin_addr.s_addr = inet_addr("192.168.1.98");
// 	if (bind(malc->socket, (struct sockaddr *)&socket_address, sizeof(struct sockaddr)))
// 	{
// 		dprintf(STDERR_FILENO, "Socket binding failed: %s\n", strerror(errno));
// 		return EXIT_FAILURE;
// 	}
// 	char buf[10000];
// 	char buf2[42];
// 	int ret = 0;
// 	unsigned slen=sizeof(struct sockaddr);
// 	dprintf(STDOUT_FILENO, "wait incoming data\n");
// 	while(1)
// 	{
// 	    ret = recvfrom(malc->socket, buf, sizeof(buf)-1, 0, (struct sockaddr *)&si_other, &slen);
// 		buf[ret] = '\0';
// 	    dprintf(STDOUT_FILENO, "recv: %s from %s\n", buf, ipToStr(((struct sockaddr*)&si_other), buf2, 41));
// 	}
// 	return EXIT_SUCCESS;
// }
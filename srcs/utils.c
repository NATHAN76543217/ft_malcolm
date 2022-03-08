# include "ft_malcolm.h"

// Convert a struct sockaddr address to a string, IPv4 and IPv6:
char *get_ip_str(const struct sockaddr *sa, char *dest, size_t maxlen)
{
	switch(sa->sa_family) {
		case AF_INET:
			inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
					dest, maxlen);
			break;

		case AF_INET6:
			inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
					dest, maxlen);
			break;
		default:
			strncpy(dest, "Unknown AF", maxlen); //TODO ft_strncpy
			return NULL;
	}

	return dest;
}

char    *macToStr(const struct ether_addr *mac, char dst[ETHER_ADDRSTRLEN])
{
	dst[sprintf(dst, "%02x:%02x:%02x:%02x:%02x:%02x",
		mac->octet[0],
		mac->octet[1],
		mac->octet[2],
		mac->octet[3],
		mac->octet[4],
		mac->octet[5]
		)] = '\0';
	return dst;
}

uint		ipToInt(const struct sockaddr *ip)
{
	return ((struct sockaddr_in *)ip)->sin_addr.s_addr;
}

void     ipToSockaddr(const u_char ip[IPV4_ADDR_LEN], struct sockaddr *sock)
{
	sock->sa_len = 16;
	sock->sa_family = AF_INET;
	ft_memcpy( &((struct sockaddr_in *) sock)->sin_addr, ip, IPV4_ADDR_LEN);
}

int     strToMac(struct ether_addr *dst, const char *str)
{
	int i = 0;
	int x = 0;
	int number = 0;
	int digit = 0;

	dprintf(STDOUT_FILENO, "str = `%s`\n", str);
	while (str[i] && x < 6)
	{
		if (str[i] == ':')
		{
			i++;
			continue ;
		}
		
		number = ft_atoi_base(str + i, "0123456789ABCDEF");
		digit = ft_digit_base(number, "0123456789ABCDEF");
		dst->octet[x] = number;
		i += digit;
		x++;
	}
	dprintf(1, "MAC addr = %02x:%02x:%02x:%02x:%02x:%02x\n", 
		dst->octet[0],
		dst->octet[1],
		dst->octet[2],
		dst->octet[3],
		dst->octet[4],
		dst->octet[5]);
	return EXIT_SUCCESS;
}

int     getMacAddress(struct ether_addr *ethAddr, const char* ifname)
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
	u_char *macAddr = (u_char *) LLADDR(sdl);
	ft_memcpy(ethAddr, macAddr, ETHER_ADDR_LEN);
	return EXIT_SUCCESS;
}

// RECV an UDP packet
// int waitUDPRequest(ft_malcolm *malc)
// {
// 	int enableBroadcast = 1;
// 	// //Create a socket enabled for receiving UDP packets
// 	if ((malc->recv_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
// 	{
// 		dprintf(STDERR_FILENO, "Failed to initialize socket: %s\n", strerror(errno));
// 		return EXIT_FAILURE;
// 	}
// 	if (setsockopt(malc->recv_socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
// 	{
// 		dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
// 		return EXIT_FAILURE;	
// 	}
// 	// if (setsockopt(malc->recv_socket, SOL_SOCKET, SO_BINDTODEVICE, &malc->, sizeof(enableBroadcast)))
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
// 	if (bind(malc->recv_socket, (struct sockaddr *)&si_me, sizeof(struct sockaddr)))
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
// 	    ret = recvfrom(malc->recv_socket, buf, sizeof(buf)-1, 0, (struct sockaddr *)&si_other, &slen);
// 		buf[ret] = '\0';
// 	    dprintf(STDOUT_FILENO, "recv: %s from %s\n", buf, get_ip_str(((struct sockaddr*)&si_other), buf2, 41));
// 	}
// 	return EXIT_SUCCESS;
// }

// LINUX
// int waitArpRequest(ft_malcolm *malc)
// {
// 	struct sockaddr_ll socket_address;
// 	int enableBroadcast = 1;
// 	// //Create a socket enabled for receiving ARP packets
// 	if ((malc->recv_socket = socket(AF_INET, SOCK_RAW, htons(ETHERTYPE_ARP))) < 0)
// 	{
// 		dprintf(STDERR_FILENO, "Failed to initialize socket: %s\n", strerror(errno));
// 		return EXIT_FAILURE;
// 	}
// 	if (setsockopt(malc->recv_socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
// 	{
// 		dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
// 		return EXIT_FAILURE;	
// 	}
// 	// if (setsockopt(malc->recv_socket, SOL_SOCKET, SO_BINDTODEVICE, &malc->, sizeof(enableBroadcast)))
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
// 	if (bind(malc->recv_socket, (struct sockaddr *)&socket_address, sizeof(struct sockaddr)))
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
// 	    ret = recvfrom(malc->recv_socket, buf, sizeof(buf)-1, 0, (struct sockaddr *)&si_other, &slen);
// 		buf[ret] = '\0';
// 	    dprintf(STDOUT_FILENO, "recv: %s from %s\n", buf, get_ip_str(((struct sockaddr*)&si_other), buf2, 41));
// 	}
// 	return EXIT_SUCCESS;
// }
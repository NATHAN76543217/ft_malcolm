# include "ft_malcolm.h"

#ifdef OSX

int bpfSetOption(ft_malcolm *malc)
{
	struct ifreq	ifr;
	u_int32_t		enable = 1;

	if (malc->opt.verbose)
		dprintf(STDOUT_FILENO, "Setting bpf options.\n");

	/* Associate the bpf device with an interface */
	ft_strlcpy(ifr.ifr_name, malc->ifName, sizeof(ifr.ifr_name)-1);
	if(ioctl(malc->socket, BIOCSETIF, &ifr) < 0)
	{
		dprintf(STDERR_FILENO, "ioct (BIOCSETIF) failed cause: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	/* Set header complete mode */
	if(ioctl(malc->socket, BIOCSHDRCMPLT, &enable) < 0)
	{
		dprintf(STDERR_FILENO, "ioct (BIOCSHDRCMPLT) failed cause: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	/* Monitor packets sent from our interface */
	if(ioctl(malc->socket, BIOCSSEESENT, &enable) < 0)
	{
		dprintf(STDERR_FILENO, "ioct (BIOCSSEESENT) failed cause: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	/* Return immediately when a packet received */
	if(ioctl(malc->socket, BIOCIMMEDIATE, &enable) < 0)
	{
		dprintf(STDERR_FILENO, "ioct (BIOCIMMEDIATE) failed cause: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	return 0;
}

int bpfCheckDlt(ft_malcolm *malc)
{
	u_int32_t dlt = 0;

	/* Ensure we are dumping the datalink we expect DLT described in <net/bpf.h> */
	if(ioctl(malc->socket, BIOCGDLT, &dlt) < 0)
	{
		dprintf(STDERR_FILENO, "ioct failed cause: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	switch (dlt) {
		case DLT_EN10MB:
			dprintf(STDOUT_FILENO, "datalink type: %u (DLT_EN10MB)\n", dlt);
			return EXIT_SUCCESS;
		default:
			dprintf(STDERR_FILENO, "Unsupported datalink type: %u", dlt);
			errno = EINVAL;
			return EXIT_FAILURE;
	}
}


int	bpfSetFilter(ft_malcolm *malc)
{
	struct bpf_program fcode = {0};

	/* Accept only arp request */
	struct bpf_insn insns[] = {
		// Load word at octet 12
		BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 12),
		// If not ETHERTYPE_ARP, skip next 3 (and return nothing)
		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ETHERTYPE_ARP, 0, 3),
		// Load word at octet 20
		BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 20),
		// If not ARPOP_REPLY, skip next 1 (and return nothing)
		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ARPOP_REQUEST, 0, 1),
		// Valid ARP reply received, return message
		BPF_STMT(BPF_RET | BPF_K, sizeof(struct ether_arp) + sizeof(struct ether_header)),
		// Return nothing
		BPF_STMT(BPF_RET | BPF_K, 0),
	};

	/* Set the filter */
	fcode.bf_len = sizeof(insns) / sizeof(struct bpf_insn);
	fcode.bf_insns = &insns[0];

	if(ioctl(malc->socket, BIOCSETF, &fcode) < 0)
	{
		dprintf(STDERR_FILENO, "ioct failed cause: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int read_packets(ft_malcolm *malc)
{
	char    *p = NULL;
	char	ipBuf[INET_ADDRSTRLEN];
	// char	ipBuf2[INET_ADDRSTRLEN];
	ssize_t n = 0;
	size_t  blen = 0;
	size_t	packet_len = sizeof(struct ether_header) + sizeof(struct ether_arp);
	struct  bpf_hdr *bh = NULL;
	struct  ether_arp 		*ah = NULL;
	struct  ether_header	*eh = NULL;

	/* get internal bpf's buffer size */
	if(ioctl(malc->socket, BIOCGBLEN, &blen) < 0)
		return EXIT_FAILURE;

	if ( (malc->buffer = malloc(blen)) == NULL)
	{
		dprintf(STDERR_FILENO, "malloc buffer failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	// dprintf(STDOUT_FILENO, "Waiting a msg from (%s)\n", ipToStr(&malc->sockSrcIp, ipBuf, INET_ADDRSTRLEN));
	dprintf(STDOUT_FILENO, "Waiting a request for (%s)\n", ipToStr(&malc->sockTargetIp, ipBuf, INET_ADDRSTRLEN));

	while (1)
	{
		ft_memset(malc->buffer, '\0', blen);

		n = read(malc->socket, malc->buffer, blen);

		if (n < 0)
		{
			dprintf(STDERR_FILENO, "Failed to read from bpf: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		else if (n == 0)
		{
			dprintf(STDERR_FILENO, "EOF reached\n");
			return EXIT_FAILURE;
		}
		// dprintf(STDOUT_FILENO, "An ARP request as been recieved:\n");
		
		p = malc->buffer;
		while (p + packet_len < malc->buffer + n) {
			bh = (struct bpf_hdr *)p;

			/* Start of ethernet frame */
			eh = (struct ether_header *)(p + bh->bh_hdrlen);
			ah = (struct ether_arp *)(eh + 1);
				

			// uint SPA = (*(uint*)ah->arp_spa);
			// u_char *cSPA = (u_char *)&SPA;

			// uint TPA = (*(uint*)ah->arp_tpa);
			// u_char *cTPA = (u_char *)&TPA;
				// dprintf(STDOUT_FILENO, "Receive an ARP request:\n%02x:%02x:%02x:%02x:%02x:%02x "
				// 	"%02x:%02x:%02x:%02x:%02x:%02x REQ Who has (%u.%u.%u.%u)? Tell (%u.%u.%u.%u)\n",
				// 	eh->ether_shost[0], eh->ether_shost[1], eh->ether_shost[2],
				// 	eh->ether_shost[3], eh->ether_shost[4], eh->ether_shost[5],
				// 	eh->ether_dhost[0], eh->ether_dhost[1], eh->ether_dhost[2],
				// 	eh->ether_dhost[3], eh->ether_dhost[4], eh->ether_dhost[5],
				// 	cTPA[0], cTPA[1], cTPA[2], cTPA[3],
				// 	cSPA[0], cSPA[1], cSPA[2], cSPA[3]);
			// uint intIp = (ipToInt(&malc->sockTargetIp));
			// u_char *charIp = (u_char *)&intIp;


			
			// dprintf(STDOUT_FILENO,"1 - %d\n"
			// "2- %d | sha == %u (%u.%u.%u.%u) | srcMac == %u (%u.%u.%u.%u)\n"
			// "3- %d\n"
			// "4- %d\n",
			// 		(ntohs(ah->ea_hdr.ar_op) == ARPOP_REQUEST),
			// 		!ft_memcmp(ah->arp_sha, &malc->srcMac, ETHER_ADDR_LEN),
			// 		TPA, 
			// 		cTPA[0],
			// 		cTPA[1],
			// 		cTPA[2],
			// 		cTPA[3],
					// intIp,
			// 		charIp[0],
			// 		charIp[1],
			// 		charIp[2],
			// 		charIp[3],
			// 		(*(uint*)ah->arp_spa == ipToInt(&malc->sockSrcIp)),
			// 		(*(uint*)ah->arp_tpa == ipToInt(&malc->sockTargetIp))
			// 		);
			if (ntohs(ah->ea_hdr.ar_op) == ARPOP_REQUEST &&
				// !ft_memcmp(ah->arp_sha, &malc->srcMac, ETHER_ADDR_LEN) && 
				// (*(uint32_t*)ah->arp_spa == ipToInt(&malc->sockSrcIp)) &&
				(*(uint32_t*)ah->arp_tpa == ipToInt(&malc->sockTargetIp))
				)
			{
				// dprintf(STDOUT_FILENO, "\nAccept request:\n%02x:%02x:%02x:%02x:%02x:%02x "
				// 	"%02x:%02x:%02x:%02x:%02x:%02x REQ Who has %s? Tell %s\n",
				// 	eh->ether_shost[0], eh->ether_shost[1], eh->ether_shost[2],
				// 	eh->ether_shost[3], eh->ether_shost[4], eh->ether_shost[5],
				// 	eh->ether_dhost[0], eh->ether_dhost[1], eh->ether_dhost[2],
				// 	eh->ether_dhost[3], eh->ether_dhost[4], eh->ether_dhost[5],
				// 	ipToStr(&malc->sockTargetIp, ipBuf2, INET_ADDRSTRLEN),
					// ipToStr(&malc->sockSrcIp, ipBuf, INET_ADDRSTRLEN ));
					/* Tell source that we are target */
					dprintf(STDOUT_FILENO, "Now get ready to send an ARP reply\n");
					spoofArp(malc);

					int i = 15;
					while (i > 0 || 1)
					{
						spoofArp(malc);
						usleep(300);
						i--;
					}
			}
			p += BPF_WORDALIGN(bh->bh_hdrlen + bh->bh_caplen);
		}
	}
	return EXIT_SUCCESS;
}
# endif     //OSX
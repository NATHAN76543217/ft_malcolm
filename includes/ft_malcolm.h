#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H


# include <stdlib.h>	// EXIT_FAILURE EXIT_SUCCESS
# include <stdio.h>		// dprintf
# include <unistd.h>	// STDOUT_FILENO STDERR_FILENO
# include <string.h>	// memset strerror

# include <ifaddrs.h>	// getifaddrs freeifaddrs
# include <netdb.h>		// gethostbyname
# include <sys/socket.h>// socket AF_INET/AF_INET6 sento recvfrom setsockopt
# include <sys/types.h>
# include <netinet/in.h>// sockaddr_in
# include <net/if_arp.h>//struct arp_hdr
# ifdef LINUX
#  include <net/if.h>
#  include <linux/if_ether.h> //sockaddr_ll
#  include <linux/if_packet.h> //sockaddr_ll
# elif defined OSX
#  include <sys/ioctl.h> // ioctl
#  include <sys/sysctl.h> //sysctl
#  include <net/if.h>
#  include <net/if_types.h>
#  include <net/if_dl.h> // sockaddr_dl
#  include <net/bpf.h>

# endif

# include "libft.h"
# include "argparse.h"

struct ft_malcolm_options
{
	int		verbose;
	int		reverse;
	char	*ifName;
};

struct ft_malcolm_options
{
	int				verbose;
	int				reverse;
	char    		*ifName;
};


typedef struct ft_malcolm
{
	int					socket;
	struct ifaddrs*     all_ifs;
	struct ifaddrs*     used_ifs;
	struct sockaddr_in	conn;
	char				*msg;
	char				*buffer;
	uint8_t				ownIP[4];
	struct sockaddr		sockSrcIp;
	struct sockaddr		sockTargetIp;
	uint8_t				srcIp[4];
	uint8_t				targetIp[ 4];
	char				ifName[IF_NAMESIZE + 1];
	uint32_t			ifIndex;
	int					verbose;
	struct ft_malcolm_options opt;
	macAddr_t	ownMac;
	macAddr_t	srcMac;
	macAddr_t	targetMac;

}				ft_malcolm;


/* BPF */
int		bpfSetOption(ft_malcolm *malc);
int		bpfCheckDlt(ft_malcolm *malc);
int		bpfSetFilter(ft_malcolm *malc);
int		read_packets(ft_malcolm *malc);
# ifdef OSX
int		getIfMacAddress(macAddr_t *ethAddr, const char* ifname);
# elif defined LINUX
int		getIfMacAddress(ft_malcolm *malc, macAddr_t *ethAddr);
# endif //LINUX
void	ipToSockaddr(const uint8_t ip[IPV4_ADDR_LEN], struct sockaddr *sock);
uint32_t	ipToInt(const struct sockaddr *ip);

/* Packet */
int		spoofArp(ft_malcolm *malc);
int		fillArpPacketOsx(arp_t *arphdr, macAddr_t *localMac,
						macAddr_t *targetMac, uint8_t *srcIp, uint8_t *targetIp, uint16_t arpType);


#endif  //FT_MALCOLM_H
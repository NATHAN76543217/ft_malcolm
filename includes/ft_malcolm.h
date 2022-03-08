#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

//TODO debug
# ifndef OSX
# define OSX
# endif

# include <stdlib.h>	// EXIT_FAILURE EXIT_SUCCESS
# include <stdio.h>		// dprintf
# include <unistd.h>	// STDOUT_FILENO STDERR_FILENO
# include <string.h>	// memset strerror
# include <ifaddrs.h>	// getifaddrs freeifaddrs
# include <netdb.h>		// gethostbyname
# include <sys/socket.h>// socket AF_INET/AF_INET6 sento recvfrom setsockopt
# include <sys/types.h>
# include <arpa/inet.h>
# include <netinet/in.h>// sockaddr_in
# include <errno.h>		// errno
# include <net/if_arp.h>//struct arp_hdr
# include <net/if_types.h>
# include <net/ethernet.h> //ETH_P_ARP
# ifdef LINUX
#  include <netinet/if_ether.h> //sockaddr_ll
# elif defined OSX
#  include <sys/ioctl.h> // ioctl
#  include <sys/sysctl.h> //sysctl
#  include <net/if.h>
#  include <net/if_dl.h> // sockaddr_dl
#  include <netinet/if_ether.h>
#  include <net/bpf.h>
# endif

# include "libft.h"
# include "argparse.h"

# define IPV4_ADDR_LEN 4
# define ETHER_ADDRSTRLEN 18

typedef struct ft_malcolm
{
	struct ifaddrs*     all_ifs;
	struct ifaddrs*     used_ifs;
	struct sockaddr_in	conn;
	char				*msg;
	char				*buffer;
	u_char				ownIP[4];
	struct sockaddr		sockSrcIp;
	struct sockaddr		sockTargetIp;
	u_char				srcIp[4];
	struct ether_addr	ownMac;
	struct ether_addr	srcMac;
	struct ether_addr	targetMac;
	u_char				targetIp[4];

	// struct ether_arp		arph;
	int		recv_socket;
	int		verbose;
	char    if_name[IF_NAMESIZE];

}				ft_malcolm;

char	*get_ip_str(const struct sockaddr *sa, char *dest, size_t maxlen); //TODO move it into libft

/* BPF */
int		bpfSetOption(int fd, char *ifname);
int		bpfCheckDlt(int fd);
int		bpfSetFilter(int fd);
int		read_packets(ft_malcolm *malc);
int		getMacAddress(struct ether_addr *ethAddr, const char* ifname);
int		strToMac(struct ether_addr *dst, const char *str);
char	*macToStr(const struct ether_addr *mac, char dst[ETHER_ADDRSTRLEN]);
void	ipToSockaddr(const u_char ip[IPV4_ADDR_LEN], struct sockaddr *sock);
uint	ipToInt(const struct sockaddr *ip);

/* Packet */
int		spoofArp(ft_malcolm *malc);
int		fillEtherPacket(struct ether_header *eth, struct ether_addr *src, struct ether_addr *dst, u_short type);
int		fillArpPacketOsx(struct ether_arp *arphdr, struct ether_addr *localMac,
						struct ether_addr *targetMac, u_char *srcIp, u_char *targetIp, u_short arpType);


#endif  //FT_MALCOLM_H

/*
 * Copyright (c) 1993 Daniel Boulet
 *
 * Redistribution and use in source forms, with and without modification,
 * are permitted provided that this entire comment appears intact.
 *
 * Redistribution in binary form may occur without any restrictions.
 * Obviously, it would be nice if you gave credit where credit is due
 * but requiring it would be too onerous.
 *
 * This software is provided ``AS IS'' without any warranties of any kind.
 *
 */

/*
 *  Linux port (c) 1994 Bob Beck
 *
 * Redistribution and use in source forms, with and without modification,
 * are permitted provided that this entire comment appears intact.
 *
 * This software is provided ``AS IS'' without any warranties of any kind.
 *
 */

/*
 *	Drastically cleaned up: Alan Cox Dec 1st 1994.
 */

/*
 * Command line interface for IP firewall facility
 */

/* $Header: /usr/brule/dept/beck/ipfirewall/RCS/ipfirewall.c,v 1.1 1994/07/28 00:06:11 beck Exp $ */
/* $Log: ipfirewall.c,v $
 * Revision 1.1  1994/07/28  00:06:11  beck
 * Initial revision
 * */


#include <sys/types.h>
#ifdef _LINUX_TYPES_H
/* Yep. it's Linux */
#ifndef LINUX
#define LINUX
#endif
#endif
#include <sys/socket.h>
#define IPFIREWALL
#include <netinet/in.h>
#include <netinet/in_system.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#ifndef LINUX
#include <netinet/ip_fw.h>
#else
#include <linux/ip_fw.h>	/* Until it gets into stdinc */
#endif
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef LINUX
#include <ctype.h>
#define IPVERSION 4
#endif

typedef enum
{
	IPF_BLOCKING = 0,
	IPF_FORWARDING = 1,
	IPF_ACCOUNTING = 2
} ipf_kind;

static char *ipf_names[3] =
	{"blocking", "forwarding", "accounting"};
static long ipf_addfunc[3] =
	{IP_FW_ADD_BLK, IP_FW_ADD_FWD, IP_ACCT_ADD};
static long ipf_delfunc[3] =
	{IP_FW_DEL_BLK, IP_FW_DEL_FWD, IP_ACCT_DEL};


void
show_usage ()
{
	fprintf (stderr, "ipfirewall:  {\n");
	fprintf (stderr, "               list ...\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               flush\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               checkb[locking] ...\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               checkf[orwarding] ...\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               addb[lock] ...\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               addf[orwarding] ...\n");
	fprintf (stderr, "             }\n");
	fprintf (stderr, "               adda[ccounting] ...\n");
	fprintf (stderr, "             }\n");
}

/*
 * I'm not sure that this is practical ...
 */

void
show_help ()
{
	abort ();
	fprintf (stderr, "ipfirewall:  {\n");
	fprintf (stderr, "               list\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               flush\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               checkb[locking] <type> <src> <dest>\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               checkf[orwarding] <type> <src> <dest>\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               addb[lock] <type> <src> <dest>\n");
	fprintf (stderr, "             |\n");
	fprintf (stderr, "               addf[orwarding] <type> <src> <dest>\n");
	fprintf (stderr, "             }\n");
	fprintf (stderr, "               adda[ccounting] <type> <src> <dest>\n");
	fprintf (stderr, "             }\n");
	fprintf (stderr, "where\n");
	fprintf (stderr, "       <src> ::= <host>:<port> /* for TCP or UDP */\n");
	fprintf (stderr, "       <src> ::= <host>        /* for ICMP */\n");
	fprintf (stderr, "      <host> ::= <byte>.<byte>.<byte>.<byte> | <hostname>\n");
	fprintf (stderr, "      <port> ::= <short> | <servicename>\n");
	fprintf (stderr, "     <short> ::= an integer in the range 1-65535\n");
	fprintf (stderr, "      <byte> ::= an integer in the range 0-255\n");
}

static
char *
fmtip (u_long uaddr)
{
	static char tbuf[100];

	sprintf (tbuf, "%d.%d.%d.%d",
		 ((char *) &uaddr)[0] & 0xff,
		 ((char *) &uaddr)[1] & 0xff,
		 ((char *) &uaddr)[2] & 0xff,
		 ((char *) &uaddr)[3] & 0xff);

	return (&tbuf[0]);
}

static
void
print_ports (int cnt, int range, u_short * ports)
{
	int ix;
	char *pad;

	if (range)
	{
		if (cnt < 2)
		{
			fprintf (stderr, "ipfirewall:  range flag set but only %d ports\n", cnt);
			abort ();
		}
		printf ("%d:%d", ports[0], ports[1]);
		ix = 2;
		pad = " ";
	}
	else
	{
		ix = 0;
		pad = "";
	}

	while (ix < cnt)
	{
		printf ("%s%d", pad, ports[ix]);
		pad = " ";
		ix += 1;
	}
}

int
do_setsockopt (char *what, int fd, int proto, int cmd, void *data, int datalen, int ok_errno)
{
	char *cmdname;

	switch (cmd)
	{
	case IP_FW_FLUSH:
		cmdname = "IP_FW_FLUSH";
		break;
	case IP_FW_CHK_BLK:
		cmdname = "IP_FW_CHK_BLK";
		break;
	case IP_FW_CHK_FWD:
		cmdname = "IP_FW_CHK_FWD";
		break;
	case IP_FW_ADD_BLK:
		cmdname = "IP_FW_ADD_BLK";
		break;
	case IP_FW_ADD_FWD:
		cmdname = "IP_FW_ADD_FWD";
		break;
	case IP_FW_DEL_BLK:
		cmdname = "IP_FW_DEL_BLK";
		break;
	case IP_FW_DEL_FWD:
		cmdname = "IP_FW_DEL_FWD";
		break;
	case IP_ACCT_ADD:
		cmdname = "IP_ACCT_ADD";
		break;
	case IP_ACCT_DEL:
		cmdname = "IP_ACCT_DEL";
		break;
	default:
		fprintf (stderr, "ipfirewall:  unknown command (%d) passed to do_setsockopt - bye!\n", cmd);
		abort ();
	}

	if (fd < 0)
	{
		printf ("setsockopt(%d, %d, %s, 0x%x, 0x%x)\n", fd, proto, cmdname, (int) data, datalen);
		if (cmd == IP_FW_CHK_BLK || cmd == IP_FW_CHK_FWD)
		{
			struct iphdr *ip = (struct iphdr *) data;
			struct tcphdr *tcp = (struct tcphdr *) &(((int *) ip)[ip->ihl]);
			if (ip->ihl != sizeof (struct iphdr) / sizeof (int))
			{
				fprintf (stderr, "ip header length %d, should be %d\n", ip->ihl, sizeof (struct iphdr) / sizeof (int));
			}
			if (ip->protocol != IPPROTO_TCP && ip->protocol != IPPROTO_UDP)
				abort ();
			printf ("data = struct iphdr : struct %shdr {\n", ip->protocol == IPPROTO_TCP ? "tcp" : "udp");
			printf ("\tsrc=%s ", fmtip (ip->saddr));
			printf ("%d\n", ntohs (tcp->th_sport));
			printf ("\tdst=%s  ", fmtip (ip->daddr));
			printf ("%d\n", ntohs (tcp->th_dport));
			printf ("}\n");
		}
		else if (cmd == IP_FW_ADD_BLK || cmd == IP_FW_ADD_FWD || cmd == IP_ACCT_ADD)
		{
			struct ip_fw *fp = (struct ip_fw *) data;
			int fmt_ports;
			printf ("data = struct ip_fw {\n");
			if (fp->flags & IP_FW_F_ACCEPT)
			{
				printf ("\taccept ");
			}
			else
			{
				printf ("\tdeny ");
			}
			switch (fp->flags & IP_FW_F_KIND)
			{
			case IP_FW_F_ALL:
				printf ("\tuniversal\n");
				fmt_ports = 0;
				break;
			case IP_FW_F_TCP:
				printf ("tcp\n");
				fmt_ports = 1;
				break;
			case IP_FW_F_UDP:
				printf ("udp\n");
				fmt_ports = 1;
				break;
			case IP_FW_F_ICMP:
				printf ("icmp\n");
				fmt_ports = 0;
				break;
			}
			printf ("\tsrc=%s:", fmtip (fp->src.s_addr));
			printf ("%s ", fmtip (fp->src_mask.s_addr));
			if (fmt_ports)
			{
				print_ports (fp->n_src_p, fp->flags & IP_FW_F_SRNG, &fp->ports[0]);
			}
			else if (fp->flags & (IP_FW_F_SRNG | IP_FW_F_DRNG))
			{
				abort ();
			}
			else if (fp->n_src_p > 0 || fp->n_dst_p > 0)
			{
				abort ();
			}
			printf ("\n");
			printf ("\tdst=%s:", fmtip (fp->dst.s_addr));
			printf ("%s ", fmtip (fp->dst_mask.s_addr));
			if (fmt_ports)
			{
				print_ports (fp->n_dst_p, fp->flags & IP_FW_F_DRNG, &fp->ports[fp->n_src_p]);
			}
			printf ("\n");
			printf ("}\n");
		}
	}
	else
	{
		if (setsockopt (fd, proto, cmd, data, datalen) < 0)
		{
			if (errno == ok_errno)
			{
				return (errno);
			}
			perror ("ipfirewall:  setsockopt");
			exit (1);
		}
	}
	return (0);
}

void
show_parms (char **argv)
{
	while (*argv)
	{
		printf ("%s ", *argv++);
	}
}

int
get_protocol (char *arg, void (*cmd_usage) (ipf_kind), ipf_kind kind)
{
	if (arg == NULL)
	{
		fprintf (stderr, "ipfirewall:  missing protocol name\n");
	}
	else if (strcmp (arg, "tcp") == 0)
	{
		return (IP_FW_F_TCP);
	}
	else if (strcmp (arg, "udp") == 0)
	{
		return (IP_FW_F_UDP);
	}
	else if (strcmp (arg, "icmp") == 0)
	{
		return (IP_FW_F_ICMP);
	}
	else if (strcmp (arg, "all") == 0)
	{
		return (IP_FW_F_ALL);
	}
	else
	{
		fprintf (stderr, "illegal protocol name \"%s\"\n", arg);
	}
	(*cmd_usage) (kind);
	exit (1);
	return (0);
}

void
get_ipaddr (char *arg, struct in_addr *addr, struct in_addr *mask, void (*usage) (ipf_kind), ipf_kind kind)
{
	char *p, *tbuf;
	int period_cnt, non_digit;
	struct hostent *hptr;

	if (arg == NULL)
	{
		fprintf (stderr, "ipfirewall:  missing ip address\n");
		exit (1);
	}

	period_cnt = 0;
	non_digit = 0;
	for (p = arg; *p != '\0' && *p != '/' && *p != ':'; p += 1)
	{
		if (*p == '.')
		{
			if (p > arg && *(p - 1) == '.')
			{
				fprintf (stderr, "ipfirewall:  two periods in a row in ip address (%s)\n", arg);
				exit (1);
			}
			period_cnt += 1;
		}
		else if (!isdigit (*p))
		{
			non_digit = 1;
		}
	}

	tbuf = malloc (p - arg + 1);
	strncpy (tbuf, arg, p - arg);
	tbuf[p - arg] = '\0';

	if (non_digit)
	{

		hptr = gethostbyname (tbuf);
		if (hptr == NULL)
		{
			fprintf (stderr, "ipfirewall:  unknown host \"%s\"\n", tbuf);
			exit (1);
		}
		if (hptr->h_length != sizeof (struct in_addr))
		{
			fprintf (stderr, "ipfirewall:  hostentry addr length = %d, expected %d (i.e. sizeof(struct in_addr))\n",
				 hptr->h_length, sizeof (struct in_addr));
			exit (1);
		}

		bcopy (hptr->h_addr, addr, sizeof (struct in_addr));

	}
	else
	{

		if (period_cnt == 3)
		{

			int a1, a2, a3, a4, matched;

			if ((matched = sscanf (tbuf, "%d.%d.%d.%d", &a1, &a2, &a3, &a4))
			    != 4)
			{
				fprintf (stderr,
					 "ipfirewall: Only %d fields matched in IP address!\n",
					 matched);
				/* should this exit here? or catch it later? -BB */
			}

			if (a1 > 255 || a2 > 255 || a3 > 255 || a4 > 255)
			{
				fprintf (stderr, "ipfirewall:  number too large in ip address (%s)\n", arg);
				exit (1);
			}

			((char *) addr)[0] = a1;
			((char *) addr)[1] = a2;
			((char *) addr)[2] = a3;
			((char *) addr)[3] = a4;

		}
		else if (strcmp (tbuf, "0") == 0)
		{

			((char *) addr)[0] = 0;
			((char *) addr)[1] = 0;
			((char *) addr)[2] = 0;
			((char *) addr)[3] = 0;

		}
		else
		{

			fprintf (stderr, "ipfirewall:  incorrect ip address format \"%s\" (expected 3 periods)\n", tbuf);
			exit (1);

		}

	}

	free (tbuf);

	if (mask == NULL)
	{

		if (*p != '\0')
		{
			fprintf (stderr, "ipfirewall:  ip netmask not allowed here (%s)\n", (char *) addr);
			exit (1);
		}

	}
	else
	{

		if (*p == ':')
		{

			get_ipaddr (p + 1, mask, NULL, usage, kind);

		}
		else if (*p == '/')
		{

			int bits;
			char *end;

			p += 1;
			if (*p == '\0')
			{
				fprintf (stderr, "ipfirewall:  missing mask value (%s)\n", arg);
				exit (1);
			}
			else if (!isdigit (*p))
			{
				fprintf (stderr, "ipfirewall:  non-numeric mask value (%s)\n", arg);
				exit (1);
			}

			bits = strtol (p, &end, 10);
			if (*end != '\0')
			{
				fprintf (stderr, "ipfirewall:  junk after mask (%s)\n", arg);
				exit (1);
			}

			if (bits < 0 || bits > sizeof (u_long) * 8)
			{
				fprintf (stderr, "ipfirewall:  mask length value out of range (%s)\n", arg);
				exit (1);
			}

			if (bits == 0)
			{	/* left shifts of 32 aren't defined */
				mask->s_addr = 0;
			}
			else
			{
				((char *) mask)[0] = (-1 << (32 - bits)) >> 24;
				((char *) mask)[1] = (-1 << (32 - bits)) >> 16;
				((char *) mask)[2] = (-1 << (32 - bits)) >> 8;
				((char *) mask)[3] = (-1 << (32 - bits)) >> 0;
			}

		}
		else if (*p == '\0')
		{
			mask->s_addr = 0xffffffff;
		}
		else
		{

			fprintf (stderr, "ipfirewall:  junk after ip address (%s)\n", arg);
			exit (1);

		}

		/*
	 * Mask off any bits in the address that are zero in the mask.
	 * This allows the user to describe a network by specifying
	 * any host on the network masked with the network's netmask.
	 */
		addr->s_addr &= mask->s_addr;

	}

}

u_short
get_one_port (char *arg, void (*usage) (ipf_kind), ipf_kind kind, const char *proto_name)
{
	int slen = strlen (arg);

	if (slen > 0 && strspn (arg, "0123456789") == slen)
	{
		int port;
		char *end;

		port = strtol (arg, &end, 10);
		if (*end != '\0')
		{
			fprintf (stderr, "ipfirewall:  illegal port number (%s)\n", arg);
			exit (1);
		}

		if (port <= 0 || port > 65535)
		{
			fprintf (stderr, "ipfirewall:  port number out of range (%d)\n", port);
			exit (1);
		}

		return (port);

	}
	else
	{

		struct servent *sptr;

		sptr = getservbyname (arg, proto_name);

		if (sptr == NULL)
		{
			fprintf (stderr, "ipfirewall:  unknown %s service \"%s\"\n", proto_name, arg);
			exit (1);
		}

		return (ntohs (sptr->s_port));

	}

}

int
get_ports (char ***argv_ptr, u_short * ports, int min_ports, int max_ports, void (*usage) (ipf_kind), ipf_kind kind, const char *proto_name)
{
	int ix;
	char *arg;
	int sign;

	ix = 0;
	sign = 1;
	while ((arg = **argv_ptr) != NULL && strcmp (arg, "from") != 0 && strcmp (arg, "to") != 0)
	{

		char *p;

	/*
	 * Check that we havn't found too many port numbers.
	 * We do this here instead of with another condition on the while loop
	 * so that the caller can assume that the next parameter is NOT a port number.
	 */

		if (ix >= max_ports)
		{
			fprintf (stderr, "ipfirewall:  too many port numbers (max %d, got at least %d, next parm=\"%s\")\n", max_ports, max_ports + 1, arg);
			exit (1);
		}

		if ((p = strchr (arg, ':')) == NULL)
		{

			ports[ix++] = get_one_port (arg, usage, kind, proto_name);

		}
		else
		{

			if (ix > 0)
			{

				fprintf (stderr, "ipfirewall:  port ranges are only allowed for the first port value pair (%s)\n", arg);
				exit (1);

			}

			if (max_ports > 1)
			{

				char *tbuf;

				tbuf = malloc ((p - arg) + 1);
				strncpy (tbuf, arg, p - arg);
				tbuf[p - arg] = '\0';

				ports[ix++] = get_one_port (tbuf, usage, kind, proto_name);
				ports[ix++] = get_one_port (p + 1, usage, kind, proto_name);
				sign = -1;

			}
			else
			{

				fprintf (stderr, "ipfirewall:  port range not allowed here (%s)\n", arg);
				exit (1);

			}
		}

		*argv_ptr += 1;
	}

	if (ix < min_ports)
	{
		if (min_ports == 1)
		{
			fprintf (stderr, "ipfirewall:  missing port number%s\n", max_ports == 1 ? "" : "(s)");
		}
		else
		{
			fprintf (stderr, "ipfirewall:  not enough port numbers (expected %d, got %d)\n", min_ports, ix);
		}
		exit (1);
	}

	return (sign * ix);

}

void
check_usage (ipf_kind kind)
{
	fprintf (stderr, "usage:  ipfirewall check%s ...\n", ipf_names[kind]);
}

void
check (ipf_kind kind, int socket_fd, char **argv)
{
	int protocol;
	struct iphdr *packet;
	char *proto_name;

	packet = (struct iphdr *) malloc (sizeof (struct iphdr) + sizeof (struct tcphdr));
	packet->version = IPVERSION;
	packet->ihl = sizeof (struct iphdr) / sizeof (int);
	printf ("check%s ", kind == IPF_BLOCKING ? "blocking" : "forwarding");
	show_parms (argv);
	printf ("\n");

	proto_name = *argv++;
	protocol = get_protocol (proto_name, check_usage, kind);
	switch (protocol)
	{
	case IP_FW_F_TCP:
		packet->protocol = IPPROTO_TCP;
		break;
	case IP_FW_F_UDP:
		packet->protocol = IPPROTO_UDP;
		break;
	default:
		fprintf (stderr, "ipfirewall:  can only check TCP or UDP packets\n");
		break;
	}

	if (*argv == NULL)
	{
		fprintf (stderr, "ipfirewall:  missing \"from\" from keyword\n");
		exit (1);
	}
	if (strcmp (*argv, "from") == 0)
	{
		argv += 1;
		get_ipaddr (*argv++, (struct in_addr *) &packet->saddr, NULL, check_usage, kind);
		if (protocol == IP_FW_F_TCP || protocol == IP_FW_F_UDP)
		{
			get_ports (&argv, &((struct tcphdr *) (&packet[1]))->th_sport, 1, 1, check_usage, kind, proto_name);
			((struct tcphdr *) (&packet[1]))->th_sport = htons (
				  ((struct tcphdr *) (&packet[1]))->th_sport
				);
		}
	}
	else
	{
		fprintf (stderr, "ipfirewall:  expected \"from\" keyword, got \"%s\"\n", *argv);
		exit (1);
	}

	if (*argv == NULL)
	{
		fprintf (stderr, "ipfirewall:  missing \"to\" from keyword\n");
		exit (1);
	}
	if (strcmp (*argv, "to") == 0)
	{
		argv += 1;
		get_ipaddr (*argv++, (struct in_addr *) &packet->daddr, NULL, check_usage, kind);
		if (protocol == IP_FW_F_TCP || protocol == IP_FW_F_UDP)
		{
			get_ports (&argv, &((struct tcphdr *) (&packet[1]))->th_dport, 1, 1, check_usage, kind, proto_name);
			((struct tcphdr *) (&packet[1]))->th_dport = htons (
				  ((struct tcphdr *) (&packet[1]))->th_dport
				);
		}
	}
	else
	{
		fprintf (stderr, "ipfirewall:  expected \"to\" keyword, got \"%s\"\n", *argv);
		exit (1);
	}

	if (*argv == NULL)
	{

		if (do_setsockopt (kind == IPF_BLOCKING ? "checkblocking" : "checkforwarding",
				   socket_fd, IPPROTO_IP,
		       kind == IPF_BLOCKING ? IP_FW_CHK_BLK : IP_FW_CHK_FWD,
				   packet,
			     sizeof (struct iphdr) + sizeof (struct tcphdr),
				   EACCES
		    ) == 0
		)
		{
			printf ("packet accepted by %s firewall\n",
			  kind == IPF_BLOCKING ? "blocking" : "forwarding");
		}
		else
		{
			printf ("packet rejected by %s firewall\n",
			  kind == IPF_BLOCKING ? "blocking" : "forwarding");
		}

		return;

	}
	else
	{
		fprintf (stderr, "ipfirewall:  extra parameters at end of command (");
		show_parms (argv);
		fprintf (stderr, ")\n");
		exit (1);
	}
}

void
add_usage (ipf_kind kind)
{
	fprintf (stderr, "usage:  ipfirewall add%s ...\n", ipf_names[kind]);
}

void
add (ipf_kind kind, int socket_fd, char **argv)
{
	int protocol, accept_firewall, src_range, dst_range;
	struct ip_fw firewall;
	char *proto_name;

	printf ("add%s ", ipf_names[kind]);
	show_parms (argv);
	printf ("\n");

	if (kind != IPF_ACCOUNTING)
	{

		if (*argv == NULL)
		{
			fprintf (stderr, "ipfirewall:  missing \"accept\" or \"deny\" keyword\n");
			exit (1);
		}

		if (strcmp (*argv, "deny") == 0)
		{
			accept_firewall = 0;
		}
		else if (strcmp (*argv, "accept") == 0)
		{
			accept_firewall = IP_FW_F_ACCEPT;
		}
		else
		{
			fprintf (stderr, "ipfirewall:  expected \"accept\" or \"deny\", got \"%s\"\n", *argv);
			exit (1);
		}

		argv += 1;
	}
	else
		accept_firewall = 0;
	proto_name = *argv++;
	protocol = get_protocol (proto_name, add_usage, kind);

	if (*argv == NULL)
	{
		fprintf (stderr, "ipfirewall:  missing \"from\" keyword\n");
		exit (1);
	}
	if (strcmp (*argv, "from") == 0)
	{
		argv++;
		get_ipaddr (*argv++, &firewall.src, &firewall.src_mask, add_usage, kind);
		if (protocol == IP_FW_F_TCP || protocol == IP_FW_F_UDP)
		{
			int cnt;
			cnt = get_ports (&argv, &firewall.ports[0], 0, IP_FW_MAX_PORTS, add_usage, kind, proto_name);
			if (cnt < 0)
			{
				src_range = IP_FW_F_SRNG;
				cnt = -cnt;
			}
			else
			{
				src_range = 0;
			}
			firewall.n_src_p = cnt;
		}
		else
		{
			firewall.n_src_p = 0;
			src_range = 0;
		}
	}
	else
	{
		fprintf (stderr, "ipfirewall:  expected \"from\", got \"%s\"\n", *argv);
		exit (1);
	}

	if (*argv == NULL)
	{
		fprintf (stderr, "ipfirewall:  missing \"to\" keyword\n");
		exit (1);
	}
	if (strcmp (*argv, "to") == 0)
	{
		argv++;
		get_ipaddr (*argv++, &firewall.dst, &firewall.dst_mask, add_usage, kind);
		if (protocol == IP_FW_F_TCP || protocol == IP_FW_F_UDP)
		{
			int cnt;
			cnt = get_ports (&argv, &firewall.ports[firewall.n_src_p], 0, IP_FW_MAX_PORTS - firewall.n_src_p, add_usage, kind, proto_name);
			if (cnt < 0)
			{
				dst_range = IP_FW_F_DRNG;
				cnt = -cnt;
			}
			else
			{
				dst_range = 0;
			}
			firewall.n_dst_p = cnt;
		}
		else
		{
			firewall.n_dst_p = 0;
			dst_range = 0;
		}
	}
	else
	{
		fprintf (stderr, "ipfirewall:  expected \"to\", got \"%s\"\n", *argv);
		exit (1);
	}

	if (*argv == NULL)
	{

		firewall.flags = protocol | accept_firewall | src_range | dst_range;
		(void) do_setsockopt (ipf_names[kind],
				      socket_fd, IPPROTO_IP,
				      ipf_addfunc[kind],
				      &firewall,
				      sizeof (firewall),
				      0
			);

	}
	else
	{
		fprintf (stderr, "ipfirewall:  extra parameters at end of command (");
		show_parms (argv);
		fprintf (stderr, ")\n");
		exit (1);
	}
}

static int count_mask(unsigned long mask)
{
	int ct;
	for(ct=0;(mask&0x80000000);ct++)
		mask<<=1;
	return ct;
}

void list_file(char *path, int acct)
{
	FILE * f=fopen(path,"r");
	char buf[256];
	unsigned long sa,da,sm,dm;
	unsigned short nsp, ndp;
	unsigned long npkt, nbyt;
	unsigned int ports[10];
	int flags;
	if(f==NULL)
	{
		perror(path);
		exit(1);
	}
	fgets(buf,255,f);	/* Title */
	while(fgets(buf,255,f))
	{
		unsigned int *pp=&ports[0];
		sscanf(buf,"%lX/%lX->%lX/%lX %X %u %u %lu %lu %u %u %u %u %u %u %u %u %u %u",
			&sa,&sm,&da,&dm,&flags, &nsp, &ndp, &npkt, &nbyt,
			&ports[0],&ports[1],&ports[2],&ports[3],&ports[4],
			&ports[5],&ports[6],&ports[7],&ports[8],&ports[9]);
		
		if(!acct)
		{
			if(flags&IP_FW_F_ACCEPT)
				printf("Accept ");
			else
				printf("Reject ");
		}
		else
			printf("%lu packets (%lu bytes) matching: ",
				npkt,nbyt);
				
		if(flags&IP_FW_F_ALL)
			printf("all  from ");
		else if(flags&IP_FW_F_TCP)
			printf("TCP  from ");
		else if(flags&IP_FW_F_UDP)
			printf("UDP  from ");
		else
			printf("ICMP from ");
		if(sm==0)
			printf("anywhere to ");
		else
			printf("%s/%d to ",fmtip(htonl(sa)),count_mask(sm));
		if(dm==0)
			printf("anywhere");
		else
			printf("%s/%d", fmtip(htonl(da)),count_mask(dm));
		
		if(flags&(IP_FW_F_TCP|IP_FW_F_UDP))
		{
			printf("  Ports: ");
			if(nsp==0)
				printf("any ");
			else
			{
				if(flags&IP_FW_F_SRNG)
				{	
					printf("%u-%u ",pp[0],pp[1]);
					pp+=2;
					nsp-=2;
				}
				while(nsp-->0)
					printf("%u ",*pp++);
			}
			printf("to");
		
			if(ndp==0)
				printf(" any");
			else
			{
				if(flags&IP_FW_F_DRNG)
				{
					printf(" %u-%u",pp[0],pp[1]);
					pp+=2;
				}
				while(ndp-->0)
					printf(" %u",*pp++);		
			}
		}
		printf("\n");
	}
	fclose(f);
}

void list(int socket_fd, char **argv)
{
	if(*argv==NULL && **argv)
	{
		fprintf(stderr,"blocking, forwarding or accounting keyword expected.\n");
		exit(1);
	}
	if(strncmp(*argv,"blocking",strlen(*argv))==0)
	{
		list_file("/proc/net/ip_block",0);
		return;
	}
	if(strncmp(*argv,"forwarding",strlen(*argv))==0)
	{
		list_file("/proc/net/ip_forward",0);
		return;
	}
	if(strncmp(*argv,"accounting",strlen(*argv))==0)
	{
		list_file("/proc/net/ip_acct",1);
		return;
	}
	fprintf(stderr,"Found '%s': 'blocking', 'forwarding' or 'accounting' keyword expected.\n",*argv);
	exit(1);
}
		
void
main (argc, argv)
     int argc;
     char **argv;
{
	int socket_fd;

	socket_fd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);

	if (socket_fd < 0)
	{
		perror ("ipfirewall:");
		exit(1);
	}

	if (argc == 1)
	{
		show_usage ();
		exit (1);
	}

	if (strcmp (argv[1], "list") == 0)
	{
		list(socket_fd, &argv[2]);
	}
	else if (strcmp (argv[1], "flush") == 0)
	{
#ifdef LINUX
		/* Same kludge as above, see above ranting and griping -BB */
		unsigned long fred = 1;
		(void) do_setsockopt (argv[1], socket_fd, IPPROTO_IP, IP_FW_FLUSH, &fred, sizeof (unsigned long), 0);
#else
		(void) do_setsockopt (argv[1], socket_fd, IPPROTO_IP, IP_FLUSH_FIREWALLS, NULL, 0, 0);
#endif

	}
	else if (strlen (argv[1]) >= strlen ("checkb")
	       && strncmp (argv[1], "checkblocking", strlen (argv[1])) == 0)
	{

		check (IPF_BLOCKING, socket_fd, &argv[2]);

	}
	else if (strlen (argv[1]) >= strlen ("checkf")
	     && strncmp (argv[1], "checkforwarding", strlen (argv[1])) == 0)
	{

		check (IPF_FORWARDING, socket_fd, &argv[2]);

	}
	else if (strlen (argv[1]) >= strlen ("addb")
		 && strncmp (argv[1], "addblocking", strlen (argv[1])) == 0)
	{

		add (IPF_BLOCKING, socket_fd, &argv[2]);

	}
	else if (strlen (argv[1]) >= strlen ("addf")
	       && strncmp (argv[1], "addforwarding", strlen (argv[1])) == 0)
	{

		add (IPF_FORWARDING, socket_fd, &argv[2]);

	}
	else if (strlen (argv[1]) >= strlen ("adda")
	       && strncmp (argv[1], "addaccounting", strlen (argv[1])) == 0)
	{

		add (IPF_ACCOUNTING, socket_fd, &argv[2]);

	}
	else
	{

		fprintf (stderr, "ipfirewall:  unknown command \"%s\"\n", argv[1]);
		show_usage ();
		exit (1);
	}

	exit (0);
}

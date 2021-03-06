/* This is the userspace/kernel interface for Generic IP Chains,
   required for libc6. */
#ifndef _FWCHAINS_KERNEL_HEADERS_H
#define _FWCHAINS_KERNEL_HEADERS_H

#include <limits.h>

#if defined(__GLIBC__) && __GLIBC__ == 2
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/types.h>
#else /* libc5 */
#include <linux/icmp.h>
#include <linux/if.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/types.h>
#include <linux/udp.h>
#include <sys/socket.h>
#endif
#endif

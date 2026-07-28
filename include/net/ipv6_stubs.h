/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _IPV6_STUBS_H
#define _IPV6_STUBS_H

/*
 * 5.x split struct ipv6_stub out of net/addrconf.h. This tree still keeps it
 * there (and in its 4.14 shape, without the fib6_info-era members), so this
 * header only forwards to addrconf.h for the benefit of backported callers.
 */
#include <net/addrconf.h>

#endif

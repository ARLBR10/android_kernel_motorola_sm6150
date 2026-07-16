/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FPS_NOTIFIER_H
#define _LINUX_FPS_NOTIFIER_H

#include <linux/notifier.h>

int FPS_register_notifier(struct notifier_block *nb, unsigned long stype,
			  bool report);
int FPS_unregister_notifier(struct notifier_block *nb, unsigned long stype);
void FPS_notify(unsigned long stype, int state);

#endif /* _LINUX_FPS_NOTIFIER_H */

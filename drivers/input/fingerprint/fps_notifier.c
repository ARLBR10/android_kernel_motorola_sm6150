// SPDX-License-Identifier: GPL-2.0
#include <linux/atomic.h>
#include <linux/fps_notifier.h>
#include <linux/module.h>

static BLOCKING_NOTIFIER_HEAD(fps_notifier_chain);
static atomic_t fps_notifier_users = ATOMIC_INIT(0);
static int fps_state;

int FPS_register_notifier(struct notifier_block *nb, unsigned long stype,
			  bool report)
{
	int error;

	error = blocking_notifier_chain_register(&fps_notifier_chain, nb);
	if (error)
		return error;

	atomic_inc(&fps_notifier_users);
	pr_info("%s: FPS sensor %lu notifier enabled\n", __func__, stype);
	if (report) {
		int state = READ_ONCE(fps_state);

		blocking_notifier_call_chain(&fps_notifier_chain, stype,
					     &state);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(FPS_register_notifier);

int FPS_unregister_notifier(struct notifier_block *nb, unsigned long stype)
{
	int error;

	error = blocking_notifier_chain_unregister(&fps_notifier_chain, nb);
	if (!error)
		atomic_dec(&fps_notifier_users);

	pr_debug("%s: FPS sensor %lu notifier unregister\n", __func__, stype);
	return error;
}
EXPORT_SYMBOL_GPL(FPS_unregister_notifier);

void FPS_notify(unsigned long stype, int state)
{
	int old_state;

	if (!atomic_read(&fps_notifier_users))
		return;

	old_state = xchg(&fps_state, state);
	if (old_state != state)
		blocking_notifier_call_chain(&fps_notifier_chain, stype,
					     &state);
}
EXPORT_SYMBOL_GPL(FPS_notify);

MODULE_DESCRIPTION("Fingerprint sensor state notifier");
MODULE_LICENSE("GPL v2");

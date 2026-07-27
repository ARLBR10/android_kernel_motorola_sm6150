// SPDX-License-Identifier: GPL-2.0
#include <linux/uaccess.h>
#include <linux/nospec.h>

/* out-of-line parts */

#ifndef INLINE_COPY_FROM_USER
unsigned long _copy_from_user(void *to, const void __user *from, unsigned long n)
{
	unsigned long res = n;
	might_fault();
	if (likely(access_ok(VERIFY_READ, from, n))) {
		/*
		 * Ensure that bad access_ok() speculation will not
		 * lead to nasty side effects *after* the copy is
		 * finished:
		 */
		barrier_nospec();
		kasan_check_write(to, n);
		res = raw_copy_from_user(to, from, n);
	}
	if (unlikely(res))
		memset(to + (n - res), 0, res);
	return res;
}
EXPORT_SYMBOL(_copy_from_user);
#endif

#ifndef INLINE_COPY_TO_USER
unsigned long _copy_to_user(void __user *to, const void *from, unsigned long n)
{
	might_fault();
	if (likely(access_ok(VERIFY_WRITE, to, n))) {
		kasan_check_read(from, n);
		n = raw_copy_to_user(to, from, n);
	}
	return n;
}
EXPORT_SYMBOL(_copy_to_user);
#endif

/**
 * check_zeroed_user: check if a userspace buffer only contains zero bytes
 * @from: Source address, in userspace.
 * @size: Size of buffer.
 *
 * This is effectively shorthand for "memchr_inv(from, 0, size) == NULL" for
 * userspace addresses (and is more efficient because we don't care where the
 * first non-zero byte is).
 *
 * Returns:
 *  * 0: There were non-zero bytes present in the buffer.
 *  * 1: The buffer was full of zero bytes.
 *  * -EFAULT: access to userspace failed.
 *
 * Upstream (f5a1a536fa14 "lib: introduce copy_struct_from_user() helper") uses
 * the unsafe_get_user()/user_access_begin() fast path.  This tree's
 * user_access_begin() is only an access_ok() alias and it has no
 * aligned_byte_mask(), so the straightforward __get_user() loop is used here
 * instead.  Same semantics, no fast path.
 */
int check_zeroed_user(const void __user *from, size_t size)
{
	unsigned long val;

	if (unlikely(size == 0))
		return 1;

	if (!access_ok(VERIFY_READ, from, size))
		return -EFAULT;

	while (size >= sizeof(unsigned long)) {
		if (__get_user(val, (unsigned long __user *)from))
			return -EFAULT;
		if (val)
			return 0;
		from += sizeof(unsigned long);
		size -= sizeof(unsigned long);
	}

	while (size--) {
		unsigned char c;

		if (__get_user(c, (unsigned char __user *)from))
			return -EFAULT;
		if (c)
			return 0;
		from++;
	}

	return 1;
}
EXPORT_SYMBOL(check_zeroed_user);

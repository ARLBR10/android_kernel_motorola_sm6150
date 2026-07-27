/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __CRYPTOHASH_H
#define __CRYPTOHASH_H

#include <uapi/linux/types.h>

#define SHA_DIGEST_WORDS 5
#define SHA_MESSAGE_BYTES (512 /*bits*/ / 8)
#define SHA_WORKSPACE_WORDS 16

void sha_init(__u32 *buf);
void sha_transform(__u32 *digest, const char *data, __u32 *W);

/*
 * Compatibility aliases for the upstream sha_* -> sha1_* rename.  The BPF code
 * backported from 5.10 uses the new spelling; taking the rename treewide would
 * touch every SHA-1 caller in the tree (random.c, syncookies, ...) for no
 * benefit, so only the names are bridged here.
 */
#define SHA1_DIGEST_WORDS	SHA_DIGEST_WORDS
#define SHA1_BLOCK_SIZE		SHA_MESSAGE_BYTES
#define SHA1_WORKSPACE_WORDS	SHA_WORKSPACE_WORDS

static inline void sha1_init(__u32 *buf)
{
	sha_init(buf);
}

static inline void sha1_transform(__u32 *digest, const char *data, __u32 *W)
{
	sha_transform(digest, data, W);
}

#endif

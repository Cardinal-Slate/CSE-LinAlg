/* kernel/tag.c — the Vector type identity: a psda whose potential is the tag 'V'. Below the seam, because
   a tag is raw reading bytes. Every Vector value orders level with this node. cse_type_has checks it.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/linalg.h"
#include "slate/psda.h"

static const unsigned char TAG_V[] = { 'V', 0 };
static slate_psda VTYPE = { TAG_V, 0, 0 };
slate_psda *cse_vec_type(void) { return &VTYPE; }

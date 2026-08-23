/// cse/linalg.h — the LinAlg domain: vectors over ℚ, type-first. A value is a psda carrying the Vector
/// type (tag 'V'): payload = x, rest = y, each a ℚ scalar. It stacks concretely on Arith — the scalar
/// arithmetic is cse_arith_* — and names its type: cse_vec_dot takes Vector carriers, checked
/// (cse_type_has). No field is supplied; the method is the type, and the type restricts the carrier.
/// Stacks on the spine + CSE-DSA + CSE-Types + CSE-Arith. SPDX-License-Identifier: MIT OR Apache-2.0
#pragma once
#include "slate/psda.h"

/// the Vector type — the tag a value must order level with to be accepted.
slate_psda *cse_vec_type(void);

slate_psda *cse_vec(slate_psda **pool, slate_psda *x, slate_psda *y);
slate_psda *cse_vec_x(slate_psda *v);
slate_psda *cse_vec_y(slate_psda *v);

/// u + v. Operands must be Vectors, or nothing.
slate_psda *cse_vec_add(slate_psda **pool, slate_psda *u, slate_psda *v);
/// s · v — a ℚ scalar times a Vector.
slate_psda *cse_vec_smul(slate_psda **pool, slate_psda *s, slate_psda *v);
/// u · v — dot product (a ℚ scalar); the kernel of matrix multiply.
slate_psda *cse_vec_dot(slate_psda **pool, slate_psda *u, slate_psda *v);

/// cse/linalg.h — the LinAlg domain: vectors over a component field, generic over cse_field. It never
/// names its scalars' field — supply Arith and the scalars are rationals. Vector add, scalar multiply,
/// and dot (the kernel of matmul), composed from the field's operations. Stacks on spine + CSE-DSA + the
/// field interface. SPDX-License-Identifier: MIT OR Apache-2.0
#pragma once
#include "slate/psda.h"
#include "cse/field.h"

slate_psda *cse_vec(slate_psda **pool, slate_psda *x, slate_psda *y);
slate_psda *cse_vec_x(slate_psda *v);
slate_psda *cse_vec_y(slate_psda *v);

/// u + v over the field F.
slate_psda *cse_vec_add(const cse_field *F, slate_psda **pool, slate_psda *u, slate_psda *v);
/// s · v — scalar times vector.
slate_psda *cse_vec_smul(const cse_field *F, slate_psda **pool, slate_psda *s, slate_psda *v);
/// u · v — dot product (a scalar); the kernel of matrix multiply.
slate_psda *cse_vec_dot(const cse_field *F, slate_psda **pool, slate_psda *u, slate_psda *v);

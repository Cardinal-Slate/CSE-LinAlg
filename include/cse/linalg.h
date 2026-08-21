/// cse/linalg.h — the LinAlg domain: vectors and matrices over a ring. Stacks on Arith, not the tower.
/// SPDX-License-Identifier: MIT OR Apache-2.0
///
/// LinAlg is the sideways rung: modules/vector spaces and a matrix algebra whose scalars come from a ring
/// (Arith). Its operations — add, scalar-multiply, matrix-multiply, transpose, and the identity — build
/// structure; their meaning is the invariants below. Note one asymmetry from Arith: matrix multiply is
/// associative but **not** commutative, so there is no matmul_comm law — A·B ≢ B·A in general. Reduction
/// (the actual arithmetic, and the fast path — a matrix unit or a GPU) is a provider's job.
#pragma once
#include "slate/psda.h"
#include "cse/arith.h"   /* scalars form a ring */

/// The operation atoms and the identity element.
extern slate_reading cse_linalg_add_op;    /* + (vector/matrix add) */
extern slate_reading cse_linalg_smul_op;   /* · (scalar × vector)   */
extern slate_reading cse_linalg_matmul_op; /* × (matrix product)    */
extern slate_reading cse_linalg_tr_op;     /* ᵀ (transpose)         */
extern slate_reading cse_linalg_eye;       /* I (identity matrix)   */

/// The operations — each builds a composition drawn from `pool`; none computes.
slate_psda *cse_linalg_add      (slate_psda **pool, slate_psda *a, slate_psda *b);  /* a + b   */
slate_psda *cse_linalg_smul     (slate_psda **pool, slate_psda *s, slate_psda *v);  /* s · v   */
slate_psda *cse_linalg_matmul   (slate_psda **pool, slate_psda *a, slate_psda *b);  /* A × B   */
slate_psda *cse_linalg_transpose(slate_psda **pool, slate_psda *a);                 /* Aᵀ      */
slate_psda *cse_linalg_eye_val  (slate_psda **pool);                               /* I       */

/// Equivalence — the spine's order at level (reduction supplied by a provider).
slate_psda *cse_linalg_eq(slate_psda *x, slate_psda *y);

/// ─── the invariants of the LinAlg domain ────────────────────────────────────────────────────────────
///   vector space / module           matrix algebra
///   ─────────────────────           ──────────────
///   add_comm   a+b ≡ b+a            matmul_assoc    (A·B)·C ≡ A·(B·C)      [not commutative]
///   smul_dist  s·(a+b) ≡ s·a+s·b    matmul_dist     A·(B+C) ≡ A·B + A·C
///   smul_id    1·a ≡ a             matmul_id       I·A ≡ A
///                                    transpose_prod  (A·B)ᵀ ≡ Bᵀ·Aᵀ
///                                    transpose_inv   (Aᵀ)ᵀ ≡ A
slate_psda *cse_linalg_add_comm      (slate_psda **s, slate_psda *a, slate_psda *b);
slate_psda *cse_linalg_smul_dist     (slate_psda **s, slate_psda *sc, slate_psda *a, slate_psda *b);
slate_psda *cse_linalg_smul_id       (slate_psda **s, slate_psda *a);
slate_psda *cse_linalg_matmul_assoc  (slate_psda **s, slate_psda *a, slate_psda *b, slate_psda *c);
slate_psda *cse_linalg_matmul_dist   (slate_psda **s, slate_psda *a, slate_psda *b, slate_psda *c);
slate_psda *cse_linalg_matmul_id     (slate_psda **s, slate_psda *a);
slate_psda *cse_linalg_transpose_prod(slate_psda **s, slate_psda *a, slate_psda *b);
slate_psda *cse_linalg_transpose_inv (slate_psda **s, slate_psda *a);

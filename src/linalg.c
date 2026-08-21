/* src/linalg.c — the LinAlg domain: vector-space and matrix-algebra laws. Slate-only.
   SPDX-License-Identifier: MIT OR Apache-2.0

   Operations build compositions; scalars are Arith values. The invariants are the module and matrix-ring
   laws — with no commutativity law for matmul, deliberately. Equivalence is the spine's order at level;
   the arithmetic (and its fast path — a matrix unit, a GPU) is a provider's job. */
#include "cse/linalg.h"
#include "cse/dsa/cell.h"

static slate_psda *bin(slate_psda **p, slate_reading op, slate_psda *a, slate_psda *b) {
  return cse_cell(cse_cell_take(p), a, b, op);
}
slate_psda *cse_linalg_add   (slate_psda **p, slate_psda *a, slate_psda *b) { return bin(p, cse_linalg_add_op, a, b); }
slate_psda *cse_linalg_smul  (slate_psda **p, slate_psda *s, slate_psda *v) { return bin(p, cse_linalg_smul_op, s, v); }
slate_psda *cse_linalg_matmul(slate_psda **p, slate_psda *a, slate_psda *b) { return bin(p, cse_linalg_matmul_op, a, b); }
slate_psda *cse_linalg_transpose(slate_psda **p, slate_psda *a) { return cse_cell(cse_cell_take(p), a, 0, cse_linalg_tr_op); }
slate_psda *cse_linalg_eye_val(slate_psda **p) {
  slate_psda *e = cse_cell_take(p); e->potential = cse_linalg_eye; e->prev = 0; e->next = 0; return e;
}

slate_psda *cse_linalg_eq(slate_psda *x, slate_psda *y) {
  return (slate_order(x, y).potential == slate_level()) ? x : 0;
}

/* a + b ≡ b + a */
slate_psda *cse_linalg_add_comm(slate_psda **s, slate_psda *a, slate_psda *b) {
  return cse_linalg_eq(cse_linalg_add(s, a, b), cse_linalg_add(s, b, a));
}
/* s·(a+b) ≡ s·a + s·b */
slate_psda *cse_linalg_smul_dist(slate_psda **s, slate_psda *sc, slate_psda *a, slate_psda *b) {
  return cse_linalg_eq(cse_linalg_smul(s, sc, cse_linalg_add(s, a, b)),
                       cse_linalg_add(s, cse_linalg_smul(s, sc, a), cse_linalg_smul(s, sc, b)));
}
/* 1·a ≡ a */
slate_psda *cse_linalg_smul_id(slate_psda **s, slate_psda *a) {
  return cse_linalg_eq(cse_linalg_smul(s, cse_arith_one_val(s), a), a);
}
/* (A·B)·C ≡ A·(B·C) */
slate_psda *cse_linalg_matmul_assoc(slate_psda **s, slate_psda *a, slate_psda *b, slate_psda *c) {
  return cse_linalg_eq(cse_linalg_matmul(s, cse_linalg_matmul(s, a, b), c),
                       cse_linalg_matmul(s, a, cse_linalg_matmul(s, b, c)));
}
/* A·(B+C) ≡ A·B + A·C */
slate_psda *cse_linalg_matmul_dist(slate_psda **s, slate_psda *a, slate_psda *b, slate_psda *c) {
  return cse_linalg_eq(cse_linalg_matmul(s, a, cse_linalg_add(s, b, c)),
                       cse_linalg_add(s, cse_linalg_matmul(s, a, b), cse_linalg_matmul(s, a, c)));
}
/* I·A ≡ A */
slate_psda *cse_linalg_matmul_id(slate_psda **s, slate_psda *a) {
  return cse_linalg_eq(cse_linalg_matmul(s, cse_linalg_eye_val(s), a), a);
}
/* (A·B)ᵀ ≡ Bᵀ·Aᵀ */
slate_psda *cse_linalg_transpose_prod(slate_psda **s, slate_psda *a, slate_psda *b) {
  return cse_linalg_eq(cse_linalg_transpose(s, cse_linalg_matmul(s, a, b)),
                       cse_linalg_matmul(s, cse_linalg_transpose(s, b), cse_linalg_transpose(s, a)));
}
/* (Aᵀ)ᵀ ≡ A */
slate_psda *cse_linalg_transpose_inv(slate_psda **s, slate_psda *a) {
  return cse_linalg_eq(cse_linalg_transpose(s, cse_linalg_transpose(s, a)), a);
}

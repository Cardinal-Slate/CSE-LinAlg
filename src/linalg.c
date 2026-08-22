/* src/linalg.c — vectors over an opaque scalar field. Composes F's operations on CSE-DSA cells; names no
   concrete field. Slate-only. SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/linalg.h"
#include "cse/dsa/cell.h"

slate_psda *cse_vec(slate_psda **pool, slate_psda *x, slate_psda *y) {
  return cse_cell(cse_cell_take(pool), x, y, 0);
}
slate_psda *cse_vec_x(slate_psda *v) { return cse_cell_payload(v); }
slate_psda *cse_vec_y(slate_psda *v) { return cse_cell_rest(v); }

slate_psda *cse_vec_add(const cse_field *F, slate_psda **p, slate_psda *u, slate_psda *v) {
  return cse_vec(p, F->add(p, cse_vec_x(u), cse_vec_x(v)), F->add(p, cse_vec_y(u), cse_vec_y(v)));
}
slate_psda *cse_vec_smul(const cse_field *F, slate_psda **p, slate_psda *s, slate_psda *v) {
  return cse_vec(p, F->mul(p, s, cse_vec_x(v)), F->mul(p, s, cse_vec_y(v)));
}
slate_psda *cse_vec_dot(const cse_field *F, slate_psda **p, slate_psda *u, slate_psda *v) {
  return F->add(p, F->mul(p, cse_vec_x(u), cse_vec_x(v)), F->mul(p, cse_vec_y(u), cse_vec_y(v)));
}

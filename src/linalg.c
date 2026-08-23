/* src/linalg.c — vectors over ℚ, type-first. A value carries the tag 'V' (kernel/tag.c); its components
   are ℚ scalars and its arithmetic is Arith's (cse_arith_*). Every op checks its carriers have type Vector
   (cse_type_has); a foreign carrier is refused. Slate-only. SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/linalg.h"
#include "cse/arith.h"
#include "cse/types.h"
#include "cse/dsa/cell.h"

slate_psda *cse_vec(slate_psda **pool, slate_psda *x, slate_psda *y) {
  return cse_cell(cse_cell_take(pool), x, y, cse_vec_type()->potential);   /* tag 'V', x/y = payload/rest */
}
slate_psda *cse_vec_x(slate_psda *v) { return cse_cell_payload(v); }
slate_psda *cse_vec_y(slate_psda *v) { return cse_cell_rest(v); }

slate_psda *cse_vec_add(slate_psda **p, slate_psda *u, slate_psda *v) {
  if (!cse_type_has(u, cse_vec_type()) || !cse_type_has(v, cse_vec_type())) return 0;
  return cse_vec(p, cse_arith_add(p, cse_vec_x(u), cse_vec_x(v)),
                    cse_arith_add(p, cse_vec_y(u), cse_vec_y(v)));
}
slate_psda *cse_vec_smul(slate_psda **p, slate_psda *s, slate_psda *v) {
  if (!cse_type_has(v, cse_vec_type())) return 0;
  return cse_vec(p, cse_arith_mul(p, s, cse_vec_x(v)),
                    cse_arith_mul(p, s, cse_vec_y(v)));
}
slate_psda *cse_vec_dot(slate_psda **p, slate_psda *u, slate_psda *v) {
  if (!cse_type_has(u, cse_vec_type()) || !cse_type_has(v, cse_vec_type())) return 0;
  return cse_arith_add(p, cse_arith_mul(p, cse_vec_x(u), cse_vec_x(v)),
                          cse_arith_mul(p, cse_vec_y(u), cse_vec_y(v)));
}

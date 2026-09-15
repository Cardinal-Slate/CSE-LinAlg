/* src/linalg.c — vectors over ℚ, type-first over opaque typing. A value is a ℚ pair (x, y) tagged with the
   Vector type via cse_typed: its content is a cell(x, y), its type is an identity handle. The scalar
   arithmetic is Arith's (cse_arith_*). Every op checks its carriers have type Vector by identity
   (cse_type_has), never reading their content. Slate-only. SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/linalg.h"
#include "cse/arith.h"
#include "cse/types.h"
#include "cse/dsa/cell.h"

/* the Vector type: an object with identity (its fields are never inspected) */
static slate_psda VTYPE;
slate_psda *cse_vec_type(void) { return &VTYPE; }

slate_psda *cse_vec(slate_psda **pool, slate_psda *x, slate_psda *y) {
  slate_psda *content = cse_cell(cse_cell_take(pool), x, y, 0);   /* x = payload, y = rest */
  return cse_typed(cse_cell_take(pool), content, &VTYPE);
}
slate_psda *cse_vec_x(slate_psda *v) { return cse_cell_payload(cse_content(v)); }
slate_psda *cse_vec_y(slate_psda *v) { return cse_cell_rest(cse_content(v)); }

slate_psda *cse_vec_add(slate_psda **p, slate_psda *u, slate_psda *v) {
  if (!cse_type_has(u, &VTYPE) || !cse_type_has(v, &VTYPE)) return 0;
  return cse_vec(p, cse_arith_add(p, cse_vec_x(u), cse_vec_x(v)),
                    cse_arith_add(p, cse_vec_y(u), cse_vec_y(v)));
}
slate_psda *cse_vec_smul(slate_psda **p, slate_psda *s, slate_psda *v) {
  if (!cse_type_has(v, &VTYPE)) return 0;
  return cse_vec(p, cse_arith_mul(p, s, cse_vec_x(v)),
                    cse_arith_mul(p, s, cse_vec_y(v)));
}
slate_psda *cse_vec_dot(slate_psda **p, slate_psda *u, slate_psda *v) {
  if (!cse_type_has(u, &VTYPE) || !cse_type_has(v, &VTYPE)) return 0;
  return cse_arith_add(p, cse_arith_mul(p, cse_vec_x(u), cse_vec_x(v)),
                          cse_arith_mul(p, cse_vec_y(u), cse_vec_y(v)));
}

/* tests/linalg.c — vectors over ℚ, type-first. No field is wired; LinAlg stacks on Arith directly, and a
   value carries the Vector tag. The op refuses a foreign carrier.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include <stdio.h>
#include "cse/linalg.h"
#include "cse/arith.h"
#include "cse/types.h"
#include "prime.h"
#include "slate/psda.h"

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("  FAIL %s\n", (m)); fails++; } } while (0)

static slate_psda arena[1024];
static slate_psda *pool;
static void pool_init(void) { int i; for (i = 0; i < 1023; i++) arena[i].next = &arena[i + 1]; arena[1023].next = 0; pool = &arena[0]; }

#define S(n) cse_arith_val(&pool, (n), 1)
static slate_psda *vec(long x, long y) { return cse_vec(&pool, S(x), S(y)); }
static int scal(slate_psda *s, long num, long den) { long n, d; cse_arith_read(s, &n, &d); return n == num && d == den; }
static int isv(slate_psda *v, long x, long y) { return scal(cse_vec_x(v), x, 1) && scal(cse_vec_y(v), y, 1); }

int main(void) {
  pool_init();
  slate_psda *u = vec(1, 2), *v = vec(3, 4);

  CHECK(isv(cse_vec_add(&pool, u, v), 4, 6), "(1,2) + (3,4) = (4,6)");
  CHECK(isv(cse_vec_smul(&pool, S(2), u), 2, 4), "2·(1,2) = (2,4)");
  CHECK(scal(cse_vec_dot(&pool, u, v), 11, 1), "(1,2)·(3,4) = 11");
  CHECK(scal(cse_vec_dot(&pool, u, vec(2, 1)), 4, 1), "matmul entry (1,2)·(2,1) = 4");

  /* type-first: a value has the Vector tag, and dot restricts its carrier to Vectors */
  CHECK(cse_type_has(u, cse_vec_type()) != 0, "a value has type Vector");
  CHECK(cse_vec_dot(&pool, S(2), v) == 0, "dot refuses a ℚ scalar where a Vector is required");

  printf(fails ? "linalg: FAIL\n" : "linalg: ok\n");
  return fails ? 1 : 0;
}

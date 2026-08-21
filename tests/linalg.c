/* tests/linalg.c — the vector-space and matrix-algebra laws: the LinAlg domain contract.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include <stdio.h>
#include "cse/linalg.h"
#include "cse/dsa/cell.h"
#include "slate/psda.h"
#include "slate/encode.h"

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("  FAIL %s\n", (m)); fails++; } } while (0)

static slate_psda arena[512];
static slate_psda *pool;
static void pool_init(void) { int i; for (i = 0; i < 511; i++) arena[i].next = &arena[i + 1]; arena[511].next = 0; pool = &arena[0]; }

static unsigned char vbuf[8][4];
static slate_psda vnodes[8];
static int vn = 0;
static slate_psda *value(unsigned id) {
  unsigned char b = (unsigned char)id;
  slate_encode(&b, 1, vbuf[vn]);
  vnodes[vn].potential = vbuf[vn]; vnodes[vn].prev = 0; vnodes[vn].next = 0;
  return &vnodes[vn++];
}

int main(void) {
  pool_init();
  slate_psda *A = value(1), *B = value(2), *C = value(3), *s = value(9);

  /* operations build the right structure */
  slate_psda *m = cse_linalg_matmul(&pool, A, B);
  CHECK(cse_cell_payload(m) == A && cse_cell_rest(m) == B && cse_cell_key(m) == cse_linalg_matmul_op, "matmul builds A×B");
  CHECK(cse_cell_key(cse_linalg_transpose(&pool, A)) == cse_linalg_tr_op, "transpose is a distinct operation");

  /* the invariants — the domain contract */
  struct { const char *law; slate_psda *held; } inv[] = {
    { "add_comm       a+b = b+a",              cse_linalg_add_comm(&pool, A, B) },
    { "smul_dist      s(a+b) = sa + sb",       cse_linalg_smul_dist(&pool, s, A, B) },
    { "smul_id        1a = a",                 cse_linalg_smul_id(&pool, A) },
    { "matmul_assoc   (AB)C = A(BC)",          cse_linalg_matmul_assoc(&pool, A, B, C) },
    { "matmul_dist    A(B+C) = AB + AC",       cse_linalg_matmul_dist(&pool, A, B, C) },
    { "matmul_id      IA = A",                 cse_linalg_matmul_id(&pool, A) },
    { "transpose_prod (AB)^T = B^T A^T",       cse_linalg_transpose_prod(&pool, A, B) },
    { "transpose_inv  (A^T)^T = A",            cse_linalg_transpose_inv(&pool, A) },
  };
  int i;
  printf("  the LinAlg domain — module + matrix-algebra laws (matmul is NOT commutative):\n");
  for (i = 0; i < 8; i++)
    printf("    %-38s %s\n", inv[i].law, inv[i].held ? "hold" : "awaits reduction");

  /* the same-shape laws must already hold under the spine's order */
  CHECK(cse_linalg_add_comm(&pool, A, B) != 0, "add_comm holds structurally");
  CHECK(cse_linalg_matmul_assoc(&pool, A, B, C) != 0, "matmul_assoc holds structurally");

  printf(fails ? "linalg: FAIL\n" : "linalg: ok\n");
  return fails ? 1 : 0;
}

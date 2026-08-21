/* kernel/atoms.c — LinAlg's operation atoms and the identity, spelled in bytes below the seam.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "slate/psda.h"

static const unsigned char A_ADD[]  = { 'A', 0 };   /* vector/matrix add */
static const unsigned char A_SMUL[] = { 'S', 0 };   /* scalar × vector   */
static const unsigned char A_MMUL[] = { '@', 0 };   /* matrix product    */
static const unsigned char A_TR[]   = { 'T', 0 };   /* transpose         */
static const unsigned char A_EYE[]  = { 'I', 0 };   /* identity matrix   */

slate_reading cse_linalg_add_op    = A_ADD;
slate_reading cse_linalg_smul_op   = A_SMUL;
slate_reading cse_linalg_matmul_op = A_MMUL;
slate_reading cse_linalg_tr_op     = A_TR;
slate_reading cse_linalg_eye       = A_EYE;

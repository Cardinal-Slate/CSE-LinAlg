# CSE-LinAlg

The **linear-algebra domain** for Cardinal-Slate — vectors and matrices over a ring. This is the
*sideways* rung: it doesn't extend the number tower, it stacks on the ring
([CSE-Arith](https://github.com/Cardinal-Slate/CSE-Arith)) and builds a module / vector space plus a
matrix algebra on top of it.

## The invariants

```
vector space / module            matrix algebra
  add_comm   a+b ≡ b+a            matmul_assoc    (A·B)·C ≡ A·(B·C)     ← associative...
  smul_dist  s(a+b) ≡ sa + sb     matmul_dist     A·(B+C) ≡ A·B + A·C
  smul_id    1·a ≡ a              matmul_id       I·A ≡ A
                                   transpose_prod  (A·B)ᵀ ≡ Bᵀ·Aᵀ
                                   transpose_inv   (Aᵀ)ᵀ ≡ A
```

One deliberate asymmetry from Arith: **matrix multiply is not commutative** — `A·B ≢ B·A` in general —
so there is no `matmul_comm` law. That single missing axiom is the whole difference between scalar and
matrix algebra.

The operations (`add`, `smul`, `matmul`, `transpose`, `eye`) build structure; `≡` is the spine's `order`
at `level`; the arithmetic — and the fast path, a CPU matrix unit or a GPU — is a **provider's** job
(CSE-GEMM, CSE-GPU).

## Role

Depends on the spine, CSE-DSA, and CSE-Arith. Sits beside the number tower, not inside it.

## Build

```
make          gate + standalone header + the law report, then libcse-linalg.a
make check    the gate and the test only
```

## License

MIT OR Apache-2.0.

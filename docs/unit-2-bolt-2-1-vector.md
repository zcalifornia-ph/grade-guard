# Unit 2 Bolt 2.1: Shared Utility and Vector Extraction

Objective: move the reusable `Vector` implementation out of `grade-guard/main.c`, define one stable API contract for downstream modules, and prove the extracted behavior with focused regression coverage.

## Executable Contract

Goal / deliverable

- Make `grade-guard/header/vector.h` and `grade-guard/source/vector.c` the single implementation of the shared vector API.
- Keep current call sites in `grade-guard/main.c` source-compatible while documenting the ownership and failure contract.
- Add repeatable verification for push, pop, remove, resize, and indexed access.

Bolt checklist

- Design: define vector API expectations, resize policy, bounds behavior, and ownership semantics.
- Implement: move the working vector behavior out of `grade-guard/main.c` and into the shared module.
- Test: add focused regression coverage for empty and non-empty vector operations.
- Docs: record supported element-storage patterns, failure behaviors, and build/test commands.
- Review: confirm the API remains stable enough for `models`, `ui_console`, persistence work, and future extraction.

Acceptance criteria interpreted for this Bolt

- `grade-guard/header/vector.h` exposes the shared API and ownership contract.
- `grade-guard/source/vector.c` owns the live implementation instead of a placeholder.
- `grade-guard/main.c` consumes the shared vector module instead of duplicating vector code.
- Regression coverage proves create, push, pop, remove, resize, and indexed access on empty and non-empty vectors.

Dependencies / interfaces touched

- Source of truth before extraction: `grade-guard/main.c`
- Public API: `grade-guard/header/vector.h`
- Shared implementation: `grade-guard/source/vector.c`
- Downstream domain dependency: `grade-guard/header/models.h`
- Verification: `grade-guard/tests/vector_test.c`
- Upstream contract: `REQUIREMENTS.md`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because the vector module is the lowest shared dependency in the extraction graph.
- NFR-05 matters because invalid ownership rules here would leak directly into profile, course, parameter, and activity cleanup logic.
- NFR-09 matters because extracting `Vector` is the first real cut that shrinks `main.c` toward orchestration-only code.
- The main current risk is confusing vector-owned element bytes with caller-owned nested heap allocations.

## Context Summary

- The repo already had a public `vector.h` and `vector.c`, but the source file was a stub that returned `NULL` or `0`.
- `grade-guard/main.c` still contained the only working vector implementation and duplicate vector declarations.
- `models.h` already depended on `vector.h`, so downstream modules were waiting on a real vector contract.
- Current delete flows rely on `vector_remove()` returning a heap copy of the removed value.
- Existing call sites store domain structs by value and sometimes store raw pointers by value for string-selection workflows.
- The repo did not yet have automated coverage for vector behavior.

## Design Decisions

- `Vector` stores elements by value as raw bytes.
- If those bytes contain pointers, the vector owns only the pointer-sized slot, not the pointee.
- `vector_destroy()` frees only the vector allocation and backing storage.
- `vector_remove()` returns a heap-allocated copy of the removed element; the caller owns and frees that copy.
- `vector_at()` returns an interior pointer into vector storage; callers must treat it as invalid after mutation, resize, or destroy.
- `vector_push_back()` is a no-op on invalid arguments or allocation failure.
- `vector_resize()` ignores zero-capacity requests, requests that would shrink below the current element count, and failed reallocations.
- Capacity growth doubles from the current capacity, starting from an initial capacity of four elements.

### Supported Element-Storage Patterns

- Plain values such as `int`, `float`, and small structs.
- Domain structs stored by value, with nested heap fields still owned by higher-level lifecycle helpers.
- Pointer values stored by value, where the pointer target remains owned outside the vector.

### Failure Behavior

- `vector_create(0)` returns `NULL`.
- `vector_at()` and `vector_remove()` return `NULL` for out-of-bounds access or invalid inputs.
- `vector_pop_back()` on an empty vector is a no-op.
- `vector_push_back()` and `vector_resize()` preserve the previous vector state if allocation fails.

## Validation Evidence

- Acceptance build (2026-03-12): `gcc -I grade-guard/header grade-guard/main.c grade-guard/source/vector.c -o grade-guard/bolt-2-1-main.exe` succeeded.
- Regression compile (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe` succeeded.
- Regression run (2026-03-12): `.\grade-guard/tests/vector_test.exe` exited `0` and printed `vector tests passed`.

## Review Status

- Reviewed for current downstream consumers: `models.h`, `ui_console.h`, and the remaining `main.c` call sites only depend on by-value storage, heap-copy removal, indexed interior access, and no-op failure handling documented above.
- Unit 2 human validation for broader data ownership and lifecycle policy remains open until Bolt 2.2 is complete.

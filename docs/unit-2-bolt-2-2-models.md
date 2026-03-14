# Unit 2 Bolt 2.2: Domain Types and Lifecycle Helpers

Objective: extract the live domain ownership rules out of `grade-guard/main.c`, make the cleanup model explicit in `models.h` and `models.c`, and prove nested create/add/delete flows without relying on undefined frees.

## Executable Contract

Goal / deliverable

- Make `grade-guard/header/models.h` and `grade-guard/source/models.c` the source of truth for domain types and lifecycle helpers.
- Replace the monolith's embedded-element cleanup bugs with one consistent model for stack, vector-embedded, and heap-owned domain structs.
- Add focused regression coverage for nested create/add/delete/reset flows under the repository's `grade-guard/unit-tests/` harness.

Bolt checklist

- Design: define lifecycle helpers for profile, course, parameter, activity creation, reset, and destruction.
- Implement: move live domain declarations and cleanup behavior into the models module and update affected `main.c` flows.
- Test: exercise nested add/remove flows and whole-profile cleanup with repeatable unit tests.
- Docs: record the ownership rules and invariants that downstream persistence/UI work must follow.
- Review: verify the ownership model matches current vector-by-value storage and `vector_remove()` semantics.

Acceptance criteria interpreted for this Bolt

- Public domain structs live in `grade-guard/header/models.h`, not in `grade-guard/main.c`.
- Lifecycle helpers distinguish between embedded instances that must be reset and heap copies that may be destroyed.
- Course, parameter, activity, and profile removal paths use cleanup helpers that match vector storage semantics.
- Evidence covers nested add/remove behavior plus safe full-profile cleanup.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Shared container contract: `grade-guard/header/vector.h` and `grade-guard/source/vector.c`
- Domain API: `grade-guard/header/models.h`
- Domain implementation: `grade-guard/source/models.c`
- Updated callers: `grade-guard/main.c`
- Verification: `grade-guard/unit-tests/test_framework.h` and `grade-guard/unit-tests/models_lifecycle_test.c`

Relevant NFRs and risks for this Bolt

- NFR-05 is the primary target because the old code freed vector-embedded structs as if they were heap allocations.
- NFR-04 matters because `main.c` now has to link against the extracted models module without symbol duplication.
- The main brownfield risk was mixing three storage modes in one code path: stack values before `vector_push_back()`, embedded values returned by `vector_at()`, and heap copies returned by `vector_remove()`.

## Context Summary

- `Vector` stores domain structs by value, so `vector_at()` returns borrowed pointers into vector storage.
- `vector_remove()` returns a heap copy of the removed bytes, which is the only place where freeing the wrapper pointer is correct.
- The original `main.c` destructors freed `Activities`, `Course_Parameter`, and `Course` wrapper pointers even when those objects lived inside vectors.
- `input_courses()` reused one initialized `Course` across loop iterations, which risked reusing component vectors between courses.
- The parameter-add flow allocated a heap `Course_Parameter`, copied it into a vector, and leaked the original allocation.
- The parameter-delete flow manually freed an embedded vector element and then called `vector_remove()` afterward.
- Downstream modules already depend on `models.h`, so Bolt 2.2 needed to publish the real lifecycle contract before UI/persistence extraction continues.

## Ownership Model

- `*_init()` prepares a stack or embedded struct for first use. For types with nested collections, it allocates the owned child vector.
- `*_reset()` releases owned strings and child vectors but does not free the wrapper struct itself.
- `*_destroy()` calls `*_reset()` and then frees the wrapper pointer. Use it only for heap-owned instances such as copies returned by `vector_remove()`.
- `Course` and `Student_Profile` own child vectors of embedded structs. Cleanup therefore walks the vector and calls `*_reset()` on each embedded element before destroying the vector container.
- `course_init()` allocates both lecture and lab parameter vectors so a `Course` can always be reset safely regardless of the current `lab_flag`.

## Invariants for Nested Relationships

- `Student_Profile.courses` stores `Course` by value.
- `Course_Component.parameters` stores `Course_Parameter` by value.
- `Course_Parameter.activities` stores `Activities` by value.
- Once a stack-local domain struct is pushed into a vector, ownership of its nested allocations transfers to the vector element copy. The temporary stack copy must not be reset afterward.
- UI code may use pointers returned by `vector_at()` only as borrowed views. Those pointers must never be passed to `*_destroy()`.

## Validation Evidence

- Acceptance build (2026-03-12): `gcc -I grade-guard/header grade-guard/main.c grade-guard/source/vector.c grade-guard/source/models.c -o grade-guard/bolt-2-2-main.exe` succeeded.
- Lifecycle regression compile (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe` succeeded.
- Lifecycle regression run (2026-03-12): `.\grade-guard/unit-tests/models_lifecycle_test.exe` exited `0` and printed `models lifecycle tests passed`.
- Vector regression compile (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe` succeeded.
- Vector regression run (2026-03-12): `.\grade-guard/tests/vector_test.exe` exited `0` and printed `vector tests passed`.

## Review Status

- Reviewed against current downstream needs: persistence and UI callers can now distinguish between borrowed vector elements and heap-owned removed copies without inventing their own cleanup rules.
- Unit 2 human validation is still required before the wider ownership policy is considered fully approved.

# Version 0.1.5 Notes

## Title
Unit 2 / Bolt 2.2 Domain Lifecycle Extraction and Ownership-Contract Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.5` repository state if it now contains:

- the extracted vector module at `grade-guard/header/vector.h` and `grade-guard/source/vector.c`,
- the extracted domain lifecycle module at `grade-guard/header/models.h` and `grade-guard/source/models.c`,
- the focused vector regression harness at `grade-guard/tests/vector_test.c`,
- the new lifecycle test harness under `grade-guard/unit-tests/`,
- the Bolt 2.2 design/evidence record at `docs/unit-2-bolt-2-2-models.md`,
- a root `README.md` updated to `v0.1.5`,
- a new top entry in `CHANGELOG.md`,
- and this file at `docs/version-0-1-5-docs.md`.

The governance baseline for this version changed only where the repository context demanded it:

- `CONTRIBUTING.md` was updated because the standard build now links `models.c` and the recommended verification flow now includes lifecycle coverage.
- `SECURITY.md` was updated because the supported-version table was stale and still pointed at `0.0.x`.
- `CODE_OF_CONDUCT.md` was not changed because this version does not change community policy.

## One-Sentence Objective

Finish Unit 2 / Bolt 2.2 by moving the live domain types and lifecycle rules into the shared `models` module, then prove that nested create/add/delete cleanup now matches the vector ownership model already established in Bolt 2.1.

## Why This Version Matters

`v0.1.4` made the shared `vector` layer real.
It did not yet resolve the more dangerous ownership problem in the actual domain model.

That gap mattered because the monolith still mixed incompatible cleanup assumptions:

- stack-local domain structs before `vector_push_back()`,
- borrowed vector elements returned by `vector_at()`,
- and heap copies returned by `vector_remove()`.

This version matters because it closes that specific ambiguity.

In `v0.1.5`:

- the domain structs now live behind one shared header,
- the lifecycle API now distinguishes reset-versus-destroy correctly,
- the risky add/delete flows in `main.c` now use that contract,
- and the repository has repeatable lifecycle tests instead of relying only on inspection.

That is a meaningful stabilization step before Unit 3 starts moving UI and controller code out of the monolith.

## Plan A / Plan B

### Plan A (Recommended): Review the Lifecycle Extraction in 20-30 Minutes

1. Read `docs/unit-2-bolt-2-2-models.md`.
2. Open `grade-guard/header/models.h` and `grade-guard/source/models.c`.
3. Rebuild the app with `models.c` linked in.
4. Run the lifecycle regression harness in `grade-guard/unit-tests/models_lifecycle_test.c`.

### Plan B: Review Only the Repository-Level Release Trail

1. Read the top `v0.1.5` entry in `CHANGELOG.md`.
2. Open `README.md` and verify the new build/test commands.
3. Check `CONTRIBUTING.md` and `SECURITY.md` if you only need the repo-policy deltas.

Use Plan B if you only need the release-documentation and manual-commit context.

## What Changed (Artifact Map)

- Extracted domain API and implementation:
  - `grade-guard/header/models.h`
  - `grade-guard/source/models.c`
- Updated monolith callers to consume the shared ownership API:
  - `grade-guard/main.c`
- Kept the shared vector layer as the storage contract foundation:
  - `grade-guard/header/vector.h`
  - `grade-guard/source/vector.c`
- Added lifecycle regression coverage:
  - `grade-guard/unit-tests/test_framework.h`
  - `grade-guard/unit-tests/models_lifecycle_test.c`
- Kept vector regression coverage in place:
  - `grade-guard/tests/vector_test.c`
- Added Bolt-specific design and evidence notes:
  - `docs/unit-2-bolt-2-2-models.md`
- Added detailed version notes:
  - `docs/version-0-1-5-docs.md`
- Updated release and support docs:
  - `README.md`
  - `CHANGELOG.md`
  - `CONTRIBUTING.md`
  - `SECURITY.md`
- Updated the local planning baseline:
  - `REQUIREMENTS.md`
- Governance doc intentionally left unchanged:
  - `CODE_OF_CONDUCT.md`
- Local artifacts identified for manual deletion before commit:
  - `grade-guard/build-smoke/`
  - `grade-guard/grade-guard-warning-build.exe`
  - `grade-guard/main.exe`
  - `grade-guard/bolt-2-1-main.exe`
  - `grade-guard/bolt-2-2-main.exe`
  - `grade-guard/tests/vector_test.exe`
  - `grade-guard/unit-tests/models_lifecycle_test.exe`

## Guided Walkthrough (What Landed)

## 1) The `models` Module Is Now a Real Runtime Dependency

Before `v0.1.5`, `grade-guard/header/models.h` and `grade-guard/source/models.c` existed only as scaffold placeholders.

In this version, they now own the live domain contract for:

- `Activities`
- `Course_Parameter`
- `Course_Component`
- `Course`
- `Student_Profile`

That matters because later modules now have one shared place to import domain types and ownership rules instead of re-declaring or reinterpreting them inside `main.c`.

## 2) The Ownership Contract Is Explicit Instead of Accidental

The key design move in this version is the split between:

- `*_init()`
- `*_reset()`
- `*_destroy()`

The intended use is now clear:

- `*_init()` prepares a stack-local or embedded struct for use.
- `*_reset()` frees the nested allocations owned by that struct but does not free the wrapper itself.
- `*_destroy()` is reserved for heap-owned wrapper pointers, especially the copies returned by `vector_remove()`.

That distinction is the core fix for the old bug pattern.

## 3) `main.c` No Longer Frees Embedded Vector Elements as If They Were Heap Objects

The monolith previously did unsafe things like:

- freeing wrapper pointers for vector-embedded `Activities`,
- allocating heap `Course_Parameter` structs only to copy them by value into a vector,
- and mixing reset and destroy behavior in one path.

In `v0.1.5`, the callers were updated to match the vector contract:

- stack-local domain structs are initialized before they are filled and pushed,
- borrowed vector elements from `vector_at()` are treated as embedded storage,
- and removed elements from `vector_remove()` are passed to `*_destroy()` because those are heap copies.

This version does not eliminate the monolith, but it removes one of the highest-risk memory-management ambiguities inside it.

## 4) The Repository Now Has a Real Unit-Test Entry Point for Shared Domain Behavior

`grade-guard/unit-tests/` now contains:

- `test_framework.h`, a small reusable assertion layer,
- `models_lifecycle_test.c`, a focused regression harness for nested domain ownership flows.

The lifecycle test covers the exact behavior this version claims to fix:

- profile initialization,
- course creation,
- parameter and activity insertion,
- deletion through `vector_remove()`,
- whole-profile reset,
- and reset idempotence.

That is the right scope for Bolt 2.2.
It is not a full application test suite, but it is sufficient proof for the extracted ownership contract.

## 5) Repository-Level Documentation Had to Change Because the Build and Verification Story Changed

The root docs now describe a different reality than in `v0.1.4`.

The normal build now links:

- `grade-guard/main.c`
- `grade-guard/source/vector.c`
- `grade-guard/source/models.c`

The verification story also changed:

- the vector regression harness still exists,
- but there is now a second focused lifecycle regression harness under `grade-guard/unit-tests/`.

That is why `README.md` and `CONTRIBUTING.md` were updated.

## 6) `SECURITY.md` Was Corrected Because the Version Table Was Out of Date

One repo-policy fix was warranted in this version.

The security policy still claimed `0.0.x` was the supported line even though the repository has been shipping `0.1.x` release checkpoints for multiple iterations.

`v0.1.5` corrects that table to say:

- `0.1.x` is supported,
- `0.0.x` is not.

This was not a behavioral security change.
It was a documentation-accuracy fix.

## 7) The Requirements Baseline Advanced, but the Unit 2 Human Gate Is Still Open

`REQUIREMENTS.md` now records Bolt 2.2 as complete with:

- design,
- implementation,
- test,
- docs,
- and review notes.

That does not mean Unit 2 itself is fully approved.
The human validation line for Unit 2 data ownership and lifecycle rules is still open.

That is the correct state:

- Bolt 2.1 is complete,
- Bolt 2.2 is complete,
- Unit 2 human validation still remains to be signed off.

## 8) Manual Cleanup Still Matters Before the Human Commit

Several generated artifacts are still present locally, including:

- old scaffold smoke-build output,
- prior acceptance binaries,
- and the current vector plus lifecycle regression executables.

Per repository rules, these were not deleted automatically.
They are recorded in `CHANGELOG.md` under `### For Deletion` so you can remove them manually before making your commit.

## 9) `REQUIREMENTS.md` Still Has the Same Staging Caveat

One practical note from earlier versions still applies:

- `REQUIREMENTS.md` records the live Bolt completion state,
- but the repository ignore rules may still keep it out of a default staging flow.

The release docs and version notes describe the Bolt completion clearly, but you should still verify what Git is actually staging before you commit.

## Copy-Paste Commands

### Read the Bolt 2.2 design/evidence note

```powershell
Get-Content docs/unit-2-bolt-2-2-models.md
```

### Rebuild the app with the extracted `models` module

```powershell
gcc -I grade-guard/header grade-guard/main.c grade-guard/source/vector.c grade-guard/source/models.c -o grade-guard/bolt-2-2-main.exe
```

### Re-run the lifecycle regression test

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe
.\grade-guard/unit-tests/models_lifecycle_test.exe
```

### Re-run the vector regression test

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe
.\grade-guard/tests/vector_test.exe
```

### Re-check local files before committing

```powershell
git status --short --ignored
```

### Confirm whether `REQUIREMENTS.md` is still ignored

```powershell
git check-ignore -v REQUIREMENTS.md
```

## Pitfalls + Review Notes

### 1) This is still not the end of the refactor

Do not describe `v0.1.5` as if `main.c` is already thin.
The ownership layer is improved, but the UI, persistence, and controller work still largely live in the monolith.

### 2) The lifecycle harness is focused, not broad

The repository now has targeted automated coverage for:

- vector behavior,
- domain lifecycle cleanup.

It still does not have broad end-to-end coverage for the full console workflow.

### 3) Manual cleanup is still required

Local executable artifacts and old build directories are still present.
Review and delete them manually if you do not want them in the commit.

### 4) `REQUIREMENTS.md` can still be missed during manual staging

Because the requirements file may still be ignored, the local Bolt completion record can be absent from a normal `git add .` flow.

### 5) Unit 2 still needs human approval

Bolt completion and unit approval are not the same thing.
The ownership policy is now implemented and documented, but the explicit human validation line in `REQUIREMENTS.md` remains open.

## Skill Transfer: What This Version Clarifies

After reading the `v0.1.5` materials, a contributor should be able to explain:

1. Why vector-by-value storage forces different cleanup rules for borrowed elements and removed copies.
2. Why `*_reset()` and `*_destroy()` must be separate in a nested C domain model.
3. How the lifecycle harness proves the correctness of the ownership contract without needing the full UI stack.
4. Why the normal build and contributor verification flow changed after the models extraction.
5. Why Unit 2 is still review-gated even though Bolt 2.2 is complete.

## 24-72 Hour Next Steps

1. Manually delete the generated artifacts listed in `CHANGELOG.md` if you want a clean commit.
2. Review and approve the Unit 2 ownership rules now that both Bolt 2.1 and Bolt 2.2 have evidence.
3. Start Unit 3 / Bolt 3.1 by extracting console primitives into the planned UI/platform layer.
4. Expand the `grade-guard/unit-tests/` harness to cover more shared modules as the refactor progresses.

---

This file exists to explain `v0.1.5` in more detail than the changelog while staying aligned with the actual repository state and the current Bolt 2.2 checkpoint.

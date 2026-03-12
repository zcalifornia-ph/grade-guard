# Version 0.1.4 Notes

## Title
Unit 2 / Bolt 2.1 Shared Vector Extraction and Regression Coverage Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.4` repository state if it now contains:

- the existing Windows console prototype source at `grade-guard/main.c`,
- the extracted shared vector API at `grade-guard/header/vector.h`,
- the live shared vector implementation at `grade-guard/source/vector.c`,
- the focused vector regression harness at `grade-guard/tests/vector_test.c`,
- the Bolt 2.1 design/evidence record at `docs/unit-2-bolt-2-1-vector.md`,
- a root `README.md` updated to `v0.1.4`,
- a new top entry in `CHANGELOG.md`,
- and this file at `docs/version-0-1-4-docs.md`.

The governance baseline remains mostly unchanged:

- `CONTRIBUTING.md` was updated because the build and verification workflow changed.
- `SECURITY.md` was not changed.
- `CODE_OF_CONDUCT.md` was not changed.

## One-Sentence Objective

Turn the `vector` scaffold from Unit 1 into the first real extracted runtime module, prove its behavior with repeatable checks, and update the repository documentation to match that new source-backed state.

## Why This Version Matters

`v0.1.3` proved that the planned module tree could exist and compile.
It did not yet move real behavior out of `grade-guard/main.c`.

This version matters because it is the first point where the refactor stops being scaffold-only and starts becoming behavioral:

- the vector module is now real,
- `main.c` no longer owns duplicate vector logic,
- the vector API has documented ownership and failure rules,
- and the repo now has a repeatable regression harness for the extracted module.

That is a meaningful transition.
It lowers the risk of future extraction work because later modules can now build on a real shared utility layer instead of a placeholder.

## Plan A / Plan B

### Plan A (Recommended): Review the Vector Extraction in 20-30 Minutes

1. Read `docs/unit-2-bolt-2-1-vector.md`.
2. Open `grade-guard/header/vector.h` and `grade-guard/source/vector.c`.
3. Run the focused regression test in `grade-guard/tests/vector_test.c`.
4. Check the Bolt 2.1 completion note in `REQUIREMENTS.md`.

### Plan B: Review Only the Repository-Level Release Trail

1. Read the top `v0.1.4` entry in `CHANGELOG.md`.
2. Open `README.md` and verify the new build/test commands.
3. Return to this file only if you want the fuller explanation and risk notes.

Use Plan B if you only need to confirm the release documentation and manual-commit context.

## What Changed (Artifact Map)

- Extracted shared utility API and implementation:
  - `grade-guard/header/vector.h`
  - `grade-guard/source/vector.c`
- Removed duplicate live vector logic from:
  - `grade-guard/main.c`
- Added focused regression coverage:
  - `grade-guard/tests/vector_test.c`
- Added Bolt-specific design and evidence notes:
  - `docs/unit-2-bolt-2-1-vector.md`
- Added detailed version notes:
  - `docs/version-0-1-4-docs.md`
- Updated release and contributor docs:
  - `README.md`
  - `CHANGELOG.md`
  - `CONTRIBUTING.md`
- Updated the local planning baseline:
  - `REQUIREMENTS.md`
- Governance docs intentionally left unchanged:
  - `SECURITY.md`
  - `CODE_OF_CONDUCT.md`
- Local artifacts identified for manual deletion before commit:
  - `grade-guard/build-smoke/`
  - `grade-guard/grade-guard-warning-build.exe`
  - `grade-guard/main.exe`
  - `grade-guard/bolt-2-1-main.exe`
  - `grade-guard/tests/vector_test.exe`

## Guided Walkthrough (What Landed)

## 1) The Vector Module Is Now the First Real Extracted Runtime Module

The main change in `v0.1.4` is that `grade-guard/source/vector.c` is no longer a placeholder.
It now owns the live implementation of:

- `vector_create`
- `vector_destroy`
- `vector_push_back`
- `vector_pop_back`
- `vector_remove`
- `vector_at`
- `vector_size`
- `vector_resize`

That matters because later modules now have one real shared dependency to build on.

## 2) `main.c` Stopped Duplicating Vector Behavior

Before this version, the live vector implementation still lived inside `grade-guard/main.c`, even though the scaffolded `vector.c` file existed.

In `v0.1.4`:

- `main.c` now includes `header/vector.h`,
- the duplicate vector declarations and implementation were removed from the monolith,
- and the program build now links `grade-guard/source/vector.c` explicitly.

This is the correct direction for the architecture because it turns `main.c` into a consumer of shared modules instead of a dumping ground.

## 3) Ownership and Failure Semantics Were Documented Instead of Left Implicit

The public header and the Bolt-specific documentation now make the contract explicit:

- vector elements are stored by value as raw bytes,
- storing a pointer means the vector owns only the slot, not the pointee,
- `vector_remove()` returns a heap-allocated copy that the caller must free,
- `vector_at()` returns an interior pointer that becomes invalid after mutation, resize, or destroy,
- invalid arguments and allocation failure preserve the prior vector state where possible.

That is important because Unit 2 is primarily about memory ownership and lifecycle clarity.
Even though broader domain cleanup work is still pending for Bolt 2.2, this version closes the ambiguity for the shared vector layer.

## 4) The Repository Now Has Focused Regression Coverage for the Extracted Utility

`grade-guard/tests/vector_test.c` adds repeatable checks for:

- zero-sized element rejection,
- empty-vector bounds handling,
- push and automatic growth,
- indexed access,
- removal with shifting,
- shrink/grow resize behavior,
- pop behavior,
- and pointer-value storage by value.

This does not mean the repository now has a full test framework.
It means the first extracted utility is no longer validated only by inspection.
That is the right scope for Bolt 2.1.

## 5) Build and Contributor Instructions Were Updated Because the Build Changed

The root `README.md` and `CONTRIBUTING.md` were updated because the project no longer builds correctly with `main.c` alone.

The build commands now link:

- `grade-guard/main.c`
- `grade-guard/source/vector.c`

The docs also now include a focused regression command for the vector harness.

This is why `CONTRIBUTING.md` changed in this version.
It was necessary, not optional, because contributor instructions would otherwise be wrong.

## 6) The Requirements Baseline Advanced, but the Unit 2 Review Gate Is Still Open

`REQUIREMENTS.md` now records Bolt 2.1 as complete with:

- design,
- implementation,
- test,
- docs,
- and review notes.

However, Unit 2 human validation is still open.

That is correct because Bolt 2.2 still has to resolve the broader ownership and lifecycle rules for:

- `Student_Profile`,
- `Course`,
- `Course_Component`,
- `Course_Parameter`,
- `Activities`.

This version completes the vector layer, not the entire memory-ownership problem.

## 7) The Changelog and Version Notes Now Match the Real Repository State

The root `CHANGELOG.md` now records `v0.1.4` as a real version entry instead of an `Unreleased` draft.
That matters because the repository state is already specific enough to describe as a concrete release checkpoint:

- one extracted runtime module,
- one focused regression harness,
- one Bolt-specific design/evidence note,
- and one updated contributor/build workflow.

This version also adds this detailed notes file so the changelog can stay short without losing technical context.

## 8) Manual Cleanup Still Matters Before the Human Commit

Several generated artifacts are still present locally, including:

- prior scaffold smoke-build outputs,
- prior local executable builds,
- the Bolt 2.1 acceptance executable,
- and the vector regression test executable.

Per repository rules, these were not deleted automatically.
They are recorded in `CHANGELOG.md` under `### For Deletion` so you can remove them manually before your commit if you want a clean worktree.

## 9) `REQUIREMENTS.md` Is Still Ignored by Git

One practical note matters for the manual commit:

- `REQUIREMENTS.md` contains the Bolt 2.1 completion record,
- but `.gitignore` still ignores `REQUIREMENTS.md`.

That means the file may not appear in a normal staging flow unless you deliberately override the ignore rule.
The release docs now describe the completed Bolt, but the planning record itself still has this repository-level staging constraint.

## Copy-Paste Commands

### Read the Bolt 2.1 design/evidence note

```powershell
Get-Content docs/unit-2-bolt-2-1-vector.md
```

### Rebuild the app with the extracted vector module

```powershell
gcc -I grade-guard/header grade-guard/main.c grade-guard/source/vector.c -o grade-guard/bolt-2-1-main.exe
```

### Re-run the focused vector regression test

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe
.\grade-guard/tests/vector_test.exe
```

### Re-check local files before committing

```powershell
git status --short --ignored
```

### Confirm the ignore rule for `REQUIREMENTS.md`

```powershell
git check-ignore -v REQUIREMENTS.md
```

## Pitfalls + Review Notes

### 1) This is still not the full Unit 2 completion

Do not describe `v0.1.4` as if the whole domain/lifecycle extraction is already done.
This version completes Bolt 2.1 only.

### 2) The current regression harness is focused, not comprehensive

The repo now has targeted automated coverage for the vector module.
It still does not have broad end-to-end or full-module automated coverage.

### 3) Manual cleanup is still required

Local executable artifacts and old smoke-build outputs are still present.
They should be reviewed and deleted manually if you do not want them in the commit.

### 4) `REQUIREMENTS.md` can still be missed during manual staging

Because `REQUIREMENTS.md` is ignored by `.gitignore`, the local Bolt progress record may not travel with the rest of the version docs unless you handle it deliberately.

### 5) The rest of the monolith still exists

`main.c` is smaller than before in one area, but most application logic is still monolithic.
That is expected at this stage.

## Skill Transfer: What This Version Clarifies

After reading the `v0.1.4` materials, a contributor should be able to explain:

1. Why the vector module is the first safe extraction target.
2. What ownership semantics the shared vector API does and does not promise.
3. How the regression harness proves the basic vector contract.
4. Why contributor build instructions had to change after the extraction.
5. Why Unit 2 still remains open even though Bolt 2.1 is complete.

## 24-72 Hour Next Steps

1. Manually delete the generated artifacts listed in `CHANGELOG.md` if you want a clean commit.
2. Decide whether `REQUIREMENTS.md` should remain ignored now that it contains ongoing Bolt completion records.
3. Start Unit 2 / Bolt 2.2 by moving domain structs and lifecycle helpers into `models`.
4. Decide where the broader unit-test framework should live before more extracted modules add their own harnesses.

---

This file exists to explain `v0.1.4` in more detail than the changelog while staying aligned with the actual repository state and the current Bolt 2.1 checkpoint.

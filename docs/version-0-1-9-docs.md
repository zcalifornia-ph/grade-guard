# Version 0.1.9 Notes

## Title
Repository-Wide Source Attribution and License Header Normalization for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.9` repository state if it now contains:

- the standardized attribution block at the top of every `.h` and `.c` file under `grade-guard/`,
- the embedded Apache 2.0 license text inside those source/header comment blocks,
- a root `README.md` updated to `v0.1.9`,
- a new top entry in `CHANGELOG.md`,
- and this file at `docs/version-0-1-9-docs.md`.

The planning and governance state changed only where the repository context required it:

- `README.md` and `CHANGELOG.md` were updated because the repository version and release trail changed.
- `REQUIREMENTS.md` was not changed because this version does not alter Unit status, acceptance evidence, or runtime scope.
- `SECURITY.md`, `CONTRIBUTING.md`, and `CODE_OF_CONDUCT.md` were not changed because this version does not alter security reporting, contributor workflow, or community-governance policy.

## One-Sentence Objective

Make authorship, course provenance, and repository licensing explicit at the file boundary for every Grade Guard C source/header file without changing runtime behavior.

## Why This Version Matters

`v0.1.8` stabilized the persistence contract and the repository runtime shape.
It did not yet make file-level provenance and license metadata consistent across the C codebase.

That left an avoidable documentation gap:

- some files could be viewed in isolation without immediate course/repository context,
- the required academic attribution format was not normalized across the source tree,
- and the root license file existed, but the relevant source/header files did not all carry that context directly.

This version matters because it closes that documentation gap:

- every tracked `.c` and `.h` file under `grade-guard/` now starts with the same academic attribution block,
- the Apache 2.0 license text is now visible inside each source/header file comment block,
- and each file now includes a short local description before the code begins.

What this version does not do is equally important:

- it does not change the persistence contract,
- it does not change the build graph or application behavior,
- it does not add new regression logic,
- and it does not close the still-open Unit 4 schema-approval or Bolt 4.2 hardening work.

## What Changed (Artifact Map)

- Repository-wide attribution/header normalization:
  - `grade-guard/main.c`
  - `grade-guard/header/*.h`
  - `grade-guard/source/*.c`
  - `grade-guard/tests/vector_test.c`
  - `grade-guard/unit-tests/*.h`
  - `grade-guard/unit-tests/*.c`
- Updated release metadata:
  - `README.md`
  - `CHANGELOG.md`
- New release note:
  - `docs/version-0-1-9-docs.md`

## Guided Walkthrough

## 1) One Header Format Now Applies to the Whole C Tree

Each targeted file now begins with the same high-level structure:

- university, college, and department lines,
- author and degree-program lines,
- the course line `CMSC 18 - COMPUTER PROGRAMING I`,
- the in-comment Apache 2.0 license text from the root repository license,
- the file name,
- and one short file-specific description.

That makes the file boundary self-describing instead of depending on repository-wide context.

## 2) The License Source Stayed at the Repository Root

The repository root still keeps the canonical Apache 2.0 text in `LICENSE.txt`.
This version mirrors that text into the source/header attribution blocks so the file-level comment and the root license stay aligned.

One practical note matters here:

- the repository uses `LICENSE.txt`, not a bare `LICENSE` file.

That distinction is now reflected in the release notes for this version so future attribution work uses the right source text.

## 3) File Descriptions Are Local Instead of Generic

The attribution block does not stop at author/license metadata.
Each file now also carries a short description of its role.

Examples of the described scope now include:

- entry-point orchestration in `grade-guard/main.c`,
- the persistence boundary in `grade-guard/source/persistence.c`,
- and shared test helpers in `grade-guard/unit-tests/test_framework.h`.

That makes quick file scanning easier for reviewers, classmates, and future maintainers.

## 4) This Is a Repository Hygiene Release, Not a Runtime Feature Release

The important engineering boundary for `v0.1.9` is that the changes are non-functional:

- no exported API changed,
- no persistence behavior changed,
- no calculations changed,
- and no user-visible console flow changed.

That is why the latest feature-level engineering note remains `docs/unit-4-bolt-4-1-persistence-contract.md`.
`v0.1.9` is about provenance clarity and release hygiene, not a new runtime milestone.

## 5) Manual Cleanup Still Matters Before a Human Commit

This repository currently contains generated binaries and a smoke-build directory from prior local validation runs.
Per repository rules, this task does not delete them automatically.

Instead, the new `CHANGELOG.md` entry marks them under `### For Deletion` so the human operator can remove them manually if desired before committing.

## Validation Evidence

- Attribution coverage check (2026-03-13): a repository-wide scan confirmed that every `.h` and `.c` file under `grade-guard/` contains the required `University of the Philippines - Mindanao` attribution marker.
- Representative structure check (2026-03-13): sampled files confirmed that the new comment block closes cleanly and the original code begins immediately after the attribution block in `grade-guard/main.c`, `grade-guard/source/persistence.c`, and `grade-guard/unit-tests/test_framework.h`.
- Functional build/test rerun (2026-03-13): not executed in this session because the underlying change is comment-only; the latest runtime/test validation baseline remains the `v0.1.8` evidence already recorded in `docs/version-0-1-8-docs.md` and `REQUIREMENTS.md`.

## Copy-Paste Commands

### Read this release note

```powershell
Get-Content docs/version-0-1-9-docs.md
```

### Verify attribution coverage across the C tree

```powershell
$targets = rg --files grade-guard -g "*.h" -g "*.c"
foreach ($path in $targets) {
    if (-not (Select-String -Path $path -Pattern '^University of the Philippines - Mindanao$' -Quiet)) {
        Write-Host "Missing attribution: $path"
    }
}
```

## Pitfalls + Debugging

### 1) Do not treat this as a runtime feature release

- The source files changed, but only in comment headers.
- If a later build or runtime issue appears, do not attribute it to a functional change in `v0.1.9` without separate evidence.

### 2) Do not assume the root license file is named `LICENSE`

- In this repository, the canonical file is `LICENSE.txt`.
- Future attribution or release tasks should continue sourcing the license text from that file unless the repository structure changes.

### 3) Do not forget the generated binaries before a manual commit

- This task intentionally left generated `.exe` outputs and the smoke-build directory untouched.
- Review the `CHANGELOG.md` `For Deletion` section and clean them manually if they should not remain in the working tree.

## Immediate Next Actions

1. Manually delete the generated binaries and smoke-build directory listed in `CHANGELOG.md` if they are not meant to stay in the repository.
2. Keep applying the same attribution format immediately when new `.c` or `.h` files are added under `grade-guard/`.
3. Continue the next functional milestone from the existing Unit 4 / Bolt 4.2 baseline rather than treating this release as a substitute for persistence hardening.

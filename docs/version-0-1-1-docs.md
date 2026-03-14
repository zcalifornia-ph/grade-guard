# Version 0.1.1 Notes

## Title
Requirements Baseline and Refactor-Planning Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.1` repository state if it now contains:

- the existing Windows console prototype source at `grade-guard/main.c`,
- a root `REQUIREMENTS.md` that breaks the next engineering phase into testable work,
- a root `README.md` updated to `v0.1.1`,
- a new top entry in `CHANGELOG.md` that records the planning artifact and current local files for manual deletion,
- this file at `docs/version-0-1-1-docs.md`.

The governance baseline from earlier versions remains in place and unchanged:

- `CONTRIBUTING.md`,
- `SECURITY.md`,
- `CODE_OF_CONDUCT.md`.

## One-Sentence Objective

Turn the next Grade Guard engineering phase from an informal refactor goal into a concrete, reviewable, testable requirements baseline without changing the implementation yet.

## Why This Version Matters

Before `v0.1.1`, the repository already contained the real prototype source, but the next step was still mostly implicit:

- the code was still a single large `main.c`,
- the need to separate concerns was known but not formally decomposed,
- bug-fix expectations existed, but not in a structured release artifact,
- the repository did not yet have a single planning document that could guide implementation safely.

This version matters because it converts that intent into a real engineering plan with traceable scope and acceptance boundaries.

## Plan A / Plan B

### Plan A (Recommended): Review the Engineering Baseline in 20-30 Minutes

1. Read `REQUIREMENTS.md` for the structured implementation plan.
2. Read the root `README.md` to see how the public project narrative now references that plan.
3. Skim `CHANGELOG.md` for the short release summary and manual cleanup items.
4. Return to this file if you want the fuller rationale and walkthrough.

### Plan B: Validate the Plan Against the Current Code

1. Open `grade-guard/main.c`.
2. Confirm that UI, persistence, data structures, and grade logic are still mixed in one file.
3. Compare that baseline with the Units and Bolts defined in `REQUIREMENTS.md`.

Use Plan B if you want to verify that the plan is grounded in the real implementation rather than written as abstract documentation.

## What Changed (Artifact Map)

- New planning artifact:
  - `REQUIREMENTS.md`
- Updated product overview and release pointer:
  - `README.md`
- Updated short release summary and cleanup list:
  - `CHANGELOG.md`
- Detailed notes for this version:
  - `docs/version-0-1-1-docs.md`
- Existing implementation referenced by the plan:
  - `grade-guard/main.c`
- Governance docs intentionally left unchanged:
  - `CONTRIBUTING.md`
  - `SECURITY.md`
  - `CODE_OF_CONDUCT.md`
- Local artifacts identified for manual deletion before commit:
  - `grade-guard/grade-guard-warning-build.exe`
  - `grade-guard/main.exe`
  - `grade-guard/tempCodeRunnerFile.c`

## Guided Walkthrough (What Landed)

## 1) The Repository Now Has a Real Refactor Plan

The main change in `v0.1.1` is the addition of `REQUIREMENTS.md`.

That file does not implement the refactor.
Instead, it turns the next engineering phase into an executable plan with:

- one-sentence intent,
- assumptions and open questions,
- user stories with acceptance criteria,
- non-functional requirements,
- a risk register,
- main Units and smaller Bolt checklists,
- design, testing, and deployment/operations sections.

This is the first version where the repository has a formal planning baseline for the modularization and bug-fix effort.

## 2) The Plan Is Tied to the Actual Prototype, Not an Imagined Rewrite

The requirements were written against the current codebase in `grade-guard/main.c`, not a hypothetical redesign.

The plan explicitly acknowledges the present state:

- one monolithic source file,
- Windows console dependencies through `windows.h` and `conio.h`,
- local CSV persistence,
- mixed UI, data-model, persistence, and grade-calculation responsibilities,
- bug-prone areas such as ownership, parsing, and edge-case handling.

That grounding matters because it reduces the risk of a future refactor plan drifting away from the real system.

## 3) The Work Is Broken Into Executable Units

`REQUIREMENTS.md` divides the work into six Units:

1. architecture baseline and module contracts,
2. core domain, utilities, and memory ownership,
3. UI/platform layer and workflow controllers,
4. persistence and CSV contract hardening,
5. grade engine extraction and defect remediation,
6. entry-point reduction, validation, and release readiness.

Each Unit then has smaller Bolt checklists that force the work through:

- design,
- implement,
- test,
- docs,
- review.

This is a meaningful improvement over a vague goal like "split the file and fix bugs."

## 4) The README Now Points Contributors at the Planning Baseline

The root `README.md` was updated because the repository narrative needed to reflect the new project state.

The `v0.1.1` README changes now:

- bump the version marker,
- point readers to this version-notes file,
- mention the new root `REQUIREMENTS.md`,
- add a short planning-baseline section describing the intended module split and bug-fix focus,
- align the roadmap with the requirements-led implementation path.

That keeps the top-level documentation in sync with how the repository is meant to evolve next.

## 5) Validation Notes Also Surfaced Another Local Artifact

During the planning task, the current prototype was compiled with warning flags to sanity-check the baseline:

```powershell
gcc -Wall -Wextra -pedantic grade-guard\main.c -o grade-guard\grade-guard-warning-build.exe
```

That compile succeeded, but it also produced:

- `grade-guard/grade-guard-warning-build.exe`

and surfaced an existing warning in `ui_selection_handler` about fallthrough behavior.

Per repository instruction, the binary artifact was not deleted automatically.
Instead, it was recorded under `### For Deletion` in `CHANGELOG.md` so you can remove it manually before committing.

## 6) Governance Docs Did Not Need More Changes

`CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` were reviewed in scope and left untouched.

Reason:

- this version changes planning artifacts, not contribution rules,
- it does not introduce a new security-reporting process,
- it does not change maintainer contact details or repository governance expectations.

## Copy-Paste Commands

### Read the new planning baseline

```powershell
Get-Content REQUIREMENTS.md
```

### Check the human-validation checkpoints

```powershell
rg "Human validation required" REQUIREMENTS.md
```

### Re-check local files before committing

```powershell
git status --short --untracked-files=all
```

## Pitfalls + Review Notes

### 1) `REQUIREMENTS.md` is a planning artifact, not the refactor itself

Do not describe `v0.1.1` as if the code has already been modularized.
This version defines the work; it does not complete it.

### 2) Local binaries still need manual cleanup

The repository currently contains:

- `grade-guard/grade-guard-warning-build.exe`
- `grade-guard/main.exe`

Those are not source artifacts and should not be included in a manual commit.

### 3) `tempCodeRunnerFile.c` is still editor residue

This file remains a local scratch artifact and should also be removed manually before committing.

### 4) The implementation is still Windows-only and monolithic

`v0.1.1` improves planning quality and release traceability, but it does not yet change the technical platform constraints of the prototype.

## Skill Transfer: What This Version Clarifies

After reading the `v0.1.1` materials, a contributor should be able to explain:

1. What the next engineering phase is trying to accomplish.
2. Why the current prototype needs modularization and defect remediation.
3. How the planned work is sequenced into Units and Bolts.
4. Which local artifacts must be cleaned up manually before the release is committed.

## 24-72 Hour Next Steps

1. Review and approve the human-validation checkpoints in `REQUIREMENTS.md`.
2. Delete `grade-guard/grade-guard-warning-build.exe`, `grade-guard/main.exe`, and `grade-guard/tempCodeRunnerFile.c` before creating the commit.
3. Begin with Unit 1 in `REQUIREMENTS.md` so the module boundaries are agreed before code is moved.
4. Use the defect-focused parts of the plan to reproduce and rank the current bugs before mixing fixes into the refactor.

---

This file exists to explain `v0.1.1` in more detail than the changelog while staying aligned with the actual repository state.

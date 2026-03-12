# Version 0.1.0 Notes

## Title
Initial Prototype Source Import and Release-Documentation Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.0` repository state if it now contains:

- the original Windows console prototype source at `grade-guard/main.c`,
- a root `README.md` updated to `v0.1.0`,
- a new top entry in `CHANGELOG.md` that calls out cleanup items under `### For Deletion`,
- this file at `docs/version-0-1-0-docs.md`.

The governance baseline from `v0.0.1` remains in place:

- `CONTRIBUTING.md`,
- `SECURITY.md`,
- `CODE_OF_CONDUCT.md`.

## One-Sentence Objective

Move Grade Guard from a documentation-only repository baseline to a source-backed prototype release and explain what the imported C application actually does.

## Why This Version Matters

Before `v0.1.0`, the repository described Grade Guard but did not yet track the main prototype source.
That meant readers could understand the project narrative, but they could not inspect or build the actual application from repository contents alone.

This version changes that by bringing in the Windows console implementation and updating the release docs around what is now verifiable in code.

## Plan A / Plan B

### Plan A (Recommended): Understand the Release in 20-30 Minutes

1. Read the root `README.md` for the high-level project scope and build flow.
2. Open `grade-guard/main.c` to inspect the implementation that the README now describes.
3. Skim `CHANGELOG.md` for the short release summary.
4. Return to this file if you want the fuller reasoning and artifact map.

### Plan B: Validate the Docs Against the Code First

1. Start at `grade-guard/main.c`.
2. Confirm the profile, course, parameter, activity, grade-goal, and CSV persistence flows.
3. Compare those behaviors against the updated README sections.

Use Plan B if you want proof of accuracy before reading the release narrative.

## What Changed (Artifact Map)

- Imported application source:
  - `grade-guard/main.c`
- Updated product overview and release pointer:
  - `README.md`
- Updated release summary and cleanup callouts:
  - `CHANGELOG.md`
- Detailed notes for this version:
  - `docs/version-0-1-0-docs.md`
- Governance docs intentionally left unchanged:
  - `CONTRIBUTING.md`
  - `SECURITY.md`
  - `CODE_OF_CONDUCT.md`
- Local artifacts identified for manual deletion before commit:
  - `grade-guard/main.exe`
  - `grade-guard/tempCodeRunnerFile.c`

## Guided Walkthrough (What Landed)

## 1) The Repository Now Contains the Actual Prototype

The main change in `v0.1.0` is that the Grade Guard implementation is now present in the repository under `grade-guard/main.c`.

That source file includes:

- a custom dynamic `Vector` abstraction,
- data models for student profiles, courses, course parameters, and activities,
- a Windows console UI layer built around `conio.h`, `windows.h`, cursor positioning, and color helpers,
- CSV save/load routines for persisted profiles,
- grade calculations for predicted GWA and goal progress.

This is the first version where the repository is source-backed rather than documentation-only.

## 2) The README Was Tightened Around Verifiable Behavior

The root `README.md` was updated because the repository now has real code to document, not just a project description.

The new `v0.1.0` README changes focus on:

- updating the version marker,
- pointing readers to this version-notes file,
- describing the current implementation snapshot in concrete terms,
- documenting the keyboard-driven console workflow and CSV persistence behavior,
- acknowledging that local binaries and editor scratch files still need manual cleanup.

This keeps the public-facing documentation grounded in the imported codebase.

## 3) Current Product Behavior Is Now Easier to Audit

From the imported `main.c`, the currently documented feature set includes:

- creating and selecting student profiles,
- adding courses with units and optional laboratory components,
- defining weighted course parameters,
- adding activities and scores,
- deleting activities, parameters, and courses through the console menus,
- setting a target grade goal from presets or a custom value,
- viewing a predicted GWA plus percent-of-goal progress,
- saving profiles into numbered CSV files in the working directory.

Those are implementation-backed claims now, not placeholder roadmap statements.

## 4) Cleanup Items Were Deliberately Documented Instead of Deleted

The current git context also includes two local artifacts:

- `grade-guard/main.exe`
- `grade-guard/tempCodeRunnerFile.c`

Per repository instruction, they were not deleted during task execution.
Instead, they were recorded under `### For Deletion` in `CHANGELOG.md` so you can remove them manually before committing.

## 5) Governance Docs Did Not Need More Changes

`CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` were reviewed in scope but left untouched.

Reason:

- the prototype source import does not change the maintainer contact details,
- it does not introduce a new language/runtime policy that contradicts the current contribution guide,
- it does not require a different community or security reporting process than the `v0.0.1` baseline already established.

## Copy-Paste Commands

### Inspect the imported source

```powershell
Get-Content grade-guard/main.c
```

### Build with GCC or MinGW-w64

```powershell
gcc grade-guard/main.c -o grade-guard.exe
.\grade-guard.exe
```

### Build with MSVC Developer PowerShell

```powershell
cl /TC grade-guard\main.c /Fe:grade-guard.exe
.\grade-guard.exe
```

### Re-check local files before committing

```powershell
git status --short --untracked-files=all
```

## Pitfalls + Review Notes

### 1) `main.exe` is not part of the source release

It is a local compiled binary artifact.
Do not treat it as canonical project source.

### 2) `tempCodeRunnerFile.c` is editor residue

This file is typical of local VS Code or Code Runner workflows.
It should not be part of the repository history.

### 3) CSV files are runtime outputs

The application writes numbered CSV files like `0.csv`, `1.csv`, and so on when profiles are saved.
If you generate those locally while testing, review them before future commits.

### 4) The codebase is still monolithic and Windows-only

`v0.1.0` makes the repository more real and more useful, but it does not claim architectural polish yet.
The implementation is still centered in one large C file and depends on Windows-specific headers.

## Skill Transfer: What This Version Clarifies

After reading the `v0.1.0` materials, a contributor should be able to explain:

1. Where the real Grade Guard implementation lives.
2. How the current console workflow is structured at a high level.
3. How profile persistence currently works.
4. Which files are part of the release and which local artifacts should be cleaned up manually.

## 24-72 Hour Next Steps

1. Delete `grade-guard/main.exe` and `grade-guard/tempCodeRunnerFile.c` before creating the commit.
2. Decide whether future housekeeping should include ignore rules for local binaries and editor scratch files.
3. Split the single-file implementation into smaller modules once behavior is stable enough to preserve safely.
4. Add a repeatable verification path so future README build instructions can be backed by explicit test or build evidence.

---

This file exists to explain `v0.1.0` in more detail than the changelog while staying aligned with the actual repository state.

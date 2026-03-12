# Version 0.1.2 Notes

## Title
Unit 1 / Bolt 1.1 Architecture Inventory and Partition Mapping Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.2` repository state if it now contains:

- the existing Windows console prototype source at `grade-guard/main.c`,
- the root planning baseline in `REQUIREMENTS.md`,
- the new architecture inventory at `docs/unit-1-bolt-1-1-monolith-inventory.md`,
- a root `README.md` updated to `v0.1.2`,
- a new top entry in `CHANGELOG.md`,
- this file at `docs/version-0-1-2-docs.md`.

The governance baseline from earlier versions remains unchanged:

- `CONTRIBUTING.md`,
- `SECURITY.md`,
- `CODE_OF_CONDUCT.md`.

## One-Sentence Objective

Turn Unit 1 / Bolt 1.1 from a vague "inventory the monolith" instruction into a traceable architecture artifact with a complete responsibility map and validation evidence before code extraction begins.

## Why This Version Matters

`v0.1.1` established the overall refactor plan, but it still left a critical question only partially answered:

- exactly what `grade-guard/main.c` currently owns,
- where each responsibility should move,
- how the future module boundaries avoid overlap,
- and how to prove that the inventory is complete before later Bolts start moving code.

This version matters because it creates that missing bridge between high-level planning and actual extraction work.

Instead of jumping straight into scaffolding, the repository now has:

- a module ownership charter,
- a type/function responsibility map,
- a `main()` partition summary,
- a thin-entry-point target,
- and a static coverage result showing that every top-level function in `grade-guard/main.c` has been assigned exactly once.

## Plan A / Plan B

### Plan A (Recommended): Review the Architecture Baseline in 20-30 Minutes

1. Read `docs/unit-1-bolt-1-1-monolith-inventory.md`.
2. Skim the Bolt 1.1 progress note in `REQUIREMENTS.md`.
3. Read the top of `README.md` and `CHANGELOG.md` to see how the repository now describes this checkpoint.
4. Return to this file if you want the fuller rationale and review notes.

### Plan B: Validate the Inventory Against the Source

1. Open `grade-guard/main.c`.
2. Compare its top-level functions and type declarations against the function and symbol maps in `docs/unit-1-bolt-1-1-monolith-inventory.md`.
3. Confirm that UI/platform, persistence, grade engine, models, vector utilities, and app flow are separated conceptually even though the code is still physically monolithic.

Use Plan B if you want to verify the inventory mechanically before approving the partition.

## What Changed (Artifact Map)

- New architecture inventory artifact:
  - `docs/unit-1-bolt-1-1-monolith-inventory.md`
- Updated public project overview and release pointer:
  - `README.md`
- Updated short release summary and cleanup list:
  - `CHANGELOG.md`
- Detailed notes for this version:
  - `docs/version-0-1-2-docs.md`
- Updated planning-progress record in local requirements baseline:
  - `REQUIREMENTS.md`
- Existing implementation analyzed by the inventory:
  - `grade-guard/main.c`
- Governance docs intentionally left unchanged:
  - `CONTRIBUTING.md`
  - `SECURITY.md`
  - `CODE_OF_CONDUCT.md`
- Local artifacts identified for manual deletion before commit:
  - `grade-guard/grade-guard-warning-build.exe`
  - `grade-guard/main.exe`

## Guided Walkthrough (What Landed)

## 1) Bolt 1.1 Now Has a Real Architecture Artifact

The main change in `v0.1.2` is the new file `docs/unit-1-bolt-1-1-monolith-inventory.md`.

That file records:

- the Bolt contract,
- the current-code context summary,
- architecture decision bullets,
- the module ownership charter,
- the symbol map for constants, types, and declarations,
- the function-to-module assignment table,
- the `main()` responsibility split,
- and the validation notes that justify the inventory.

This matters because later Bolts can now extract code against an explicit map instead of relying on memory or ad hoc decisions.

## 2) The Inventory Stays Grounded in the Actual Monolith

The inventory is based on the current `grade-guard/main.c`, not on an imagined future layout.

It explicitly captures that the source file currently mixes:

- vector utilities,
- domain structs and destructors,
- Windows console helpers,
- persistence routines,
- grade calculations,
- startup/shutdown logic,
- profile/course/activity workflows.

That grounding is what makes the partition useful.
If the inventory had skipped the real hotspots, Bolt 1.2 would start from an unstable baseline.

## 3) The Repository Now States the Intended Module Boundaries More Clearly

The new ownership charter defines the target roles for:

- `vector`,
- `models`,
- `grade_calc`,
- `ui_console`,
- `persistence`,
- `profile_controller`,
- `app`.

The important constraint here is not just naming modules.
It is specifying what each module must not own.

That helps prevent a false refactor where code moves to multiple files but the coupling stays intact.

## 4) Validation Was Documentation-Driven but Still Evidence-Based

This version did not build or refactor the C code.
It validated the architecture inventory through a static coverage pass instead.

Result:

- `40` top-level functions were identified in `grade-guard/main.c`,
- `40` top-level functions were mapped in the architecture inventory,
- no unmapped functions remained,
- no extra functions were documented beyond what the file defines.

That is the right level of proof for Bolt 1.1 because the deliverable is partition accuracy, not executable scaffolding yet.

## 5) The Requirements Baseline Was Advanced, But the Bolt Is Not Closed Yet

The local `REQUIREMENTS.md` now records that Bolt 1.1 has completed:

- design,
- implementation,
- test,
- docs.

The review step remains unchecked.

That is intentional.
The Bolt should not be marked complete until a human explicitly agrees that the partition supports:

- a thin future `main.c`,
- the upcoming extraction work,
- and later bug remediation.

## 6) README and Changelog Were Updated to Match the New Checkpoint

The root `README.md` was updated from `v0.1.1` to `v0.1.2` and now points readers at:

- the new version-notes file,
- the architecture inventory,
- the current Bolt 1.1 progress state.

`CHANGELOG.md` was also updated so the short release record reflects:

- the new inventory artifact,
- the local requirements progress note,
- the unchanged governance docs,
- and the current manual cleanup list.

## 7) Governance Docs Still Did Not Need More Changes

`CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` were reviewed in scope and left untouched.

Reason:

- this version is still a documentation and planning checkpoint,
- it does not change how contributors submit work,
- it does not change vulnerability reporting,
- it does not change repository conduct expectations.

## Copy-Paste Commands

### Read the architecture inventory

```powershell
Get-Content docs/unit-1-bolt-1-1-monolith-inventory.md
```

### Re-check the Bolt 1.1 progress markers

```powershell
Get-Content REQUIREMENTS.md | Select-String "Bolt 1.1" -Context 0,8
```

### Re-check local files before committing

```powershell
git status --short --ignored
```

## Pitfalls + Review Notes

### 1) This is still not the module scaffold

Do not describe `v0.1.2` as if the code has already been moved into `header/` and `source/`.
This version documents the partition and proves inventory coverage; Bolt 1.2 still has to create the actual file layout.

### 2) The Bolt review gate is still open

The architecture artifact exists, but human sign-off is still required before Bolt 1.1 can be marked complete.

### 3) `REQUIREMENTS.md` is currently ignored by `.gitignore`

That means its local progress update may not appear in a normal `git status`.
Be deliberate about that when you prepare the manual commit so the release record matches what you actually intend to include.

### 4) Local binaries still need manual cleanup

The repository currently contains:

- `grade-guard/grade-guard-warning-build.exe`
- `grade-guard/main.exe`

Those are local artifacts and should not be part of the source commit.

### 5) The implementation is still Windows-only and monolithic

`v0.1.2` improves architecture traceability and release clarity, but the actual technical constraints of the prototype remain unchanged until later Units land.

## Skill Transfer: What This Version Clarifies

After reading the `v0.1.2` materials, a contributor should be able to explain:

1. Which major responsibilities currently live inside `grade-guard/main.c`.
2. Which destination module each major function and type should move into.
3. Why the future `main.c` can become orchestration-only.
4. Why human review is still required before Bolt 1.1 is considered done.

## 24-72 Hour Next Steps

1. Review and approve the module ownership charter and function map in `docs/unit-1-bolt-1-1-monolith-inventory.md`.
2. Decide how `REQUIREMENTS.md` should be handled in Git before creating the manual commit, since it is currently ignored.
3. Delete `grade-guard/grade-guard-warning-build.exe` and `grade-guard/main.exe` before committing.
4. Start Bolt 1.2 by scaffolding `grade-guard/header/` and `grade-guard/source/` only after the architecture review is settled.

---

This file exists to explain `v0.1.2` in more detail than the changelog while staying aligned with the actual repository state and the current Bolt 1.1 checkpoint.

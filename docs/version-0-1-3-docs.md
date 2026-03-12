# Version 0.1.3 Notes

## Title
Unit 1 / Bolt 1.2 Public Interface Scaffold and File Layout Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.3` repository state if it now contains:

- the existing Windows console prototype source at `grade-guard/main.c`,
- the Unit 1 architecture inventory at `docs/unit-1-bolt-1-1-monolith-inventory.md`,
- the new Bolt 1.2 scaffold record at `docs/unit-1-bolt-1-2-module-scaffold.md`,
- the new source tree scaffold under `grade-guard/header/` and `grade-guard/source/`,
- a root `README.md` updated to `v0.1.3`,
- a new top entry in `CHANGELOG.md`,
- and this file at `docs/version-0-1-3-docs.md`.

The governance baseline from earlier versions remains unchanged:

- `CONTRIBUTING.md`,
- `SECURITY.md`,
- `CODE_OF_CONDUCT.md`.

## One-Sentence Objective

Turn Unit 1 / Bolt 1.2 from a planned module list into a concrete on-disk scaffold with public interfaces, dependency rules, and smoke-build evidence before moving real logic out of `grade-guard/main.c`.

## Why This Version Matters

`v0.1.2` documented where the monolith should be partitioned, but it still left one important gap:

- the target file layout did not exist yet,
- the public headers were not named or shaped yet,
- shared constants still had no owned destination interfaces,
- and there was no build evidence proving the split could start safely.

This version matters because it closes that gap without pretending the extraction itself is already done.

The repository now has:

- a real `header/` and `source/` directory scaffold,
- public header contracts for the seven planned modules,
- placeholder source files that compile independently,
- an explicit include-guard and dependency strategy,
- and a compile-smoke result proving the scaffold is structurally sound.

## Plan A / Plan B

### Plan A (Recommended): Review the Scaffold in 20-30 Minutes

1. Read `docs/unit-1-bolt-1-2-module-scaffold.md`.
2. Skim the Bolt 1.2 progress note in `REQUIREMENTS.md`.
3. Open `grade-guard/header/` and `grade-guard/source/` to confirm the scaffold matches the documented module map.
4. Return to this file if you want the fuller rationale and review notes.

### Plan B: Validate the Build Boundary Mechanically

1. Open the public headers under `grade-guard/header/`.
2. Confirm the dependency direction:
   - `vector.h` is lowest-level,
   - `models.h` builds on `vector.h`,
   - `grade_calc.h`, `persistence.h`, and `profile_controller.h` build on `models.h`,
   - `ui_console.h` stays separate from domain and persistence headers.
3. Re-run the compile-smoke command from `docs/unit-1-bolt-1-2-module-scaffold.md` and verify object files are produced for all seven scaffold sources.

Use Plan B if you want proof that the scaffold compiles before reviewing the naming and ownership decisions.

## What Changed (Artifact Map)

- New module scaffold files:
  - `grade-guard/header/app.h`
  - `grade-guard/header/vector.h`
  - `grade-guard/header/models.h`
  - `grade-guard/header/grade_calc.h`
  - `grade-guard/header/persistence.h`
  - `grade-guard/header/ui_console.h`
  - `grade-guard/header/profile_controller.h`
  - `grade-guard/source/app.c`
  - `grade-guard/source/vector.c`
  - `grade-guard/source/models.c`
  - `grade-guard/source/grade_calc.c`
  - `grade-guard/source/persistence.c`
  - `grade-guard/source/ui_console.c`
  - `grade-guard/source/profile_controller.c`
- New scaffold documentation artifact:
  - `docs/unit-1-bolt-1-2-module-scaffold.md`
- Updated public project overview and release pointer:
  - `README.md`
- Updated short release summary and cleanup list:
  - `CHANGELOG.md`
- Detailed notes for this version:
  - `docs/version-0-1-3-docs.md`
- Updated planning-progress record in the local requirements baseline:
  - `REQUIREMENTS.md`
- Existing implementation intentionally left as the runtime source of truth:
  - `grade-guard/main.c`
- Governance docs intentionally left unchanged:
  - `CONTRIBUTING.md`
  - `SECURITY.md`
  - `CODE_OF_CONDUCT.md`
- Local artifacts identified for manual deletion before commit:
  - `grade-guard/build-smoke/`
  - `grade-guard/grade-guard-warning-build.exe`
  - `grade-guard/main.exe`

## Guided Walkthrough (What Landed)

## 1) Bolt 1.2 Now Exists on Disk, Not Just in Planning Text

The main change in `v0.1.3` is that the target module map from Unit 1 is now represented as actual files under:

- `grade-guard/header/`
- `grade-guard/source/`

That matters because future Units no longer need to re-decide where the public API boundaries start.
The scaffold makes the intended architecture tangible.

## 2) Public Interfaces Were Named Before Heavy Extraction Begins

This version establishes the first public interfaces for:

- `app`,
- `vector`,
- `models`,
- `grade_calc`,
- `persistence`,
- `ui_console`,
- `profile_controller`.

That naming step is important because it stops later extraction work from inventing inconsistent headers on the fly.
The repository now has one agreed place for each module contract.

## 3) Shared Constants Now Have Ownership Instead of Floating in `main.c`

The scaffold assigns shared constants deliberately:

- UI-facing selection, key, and color values belong in `ui_console.h`,
- grade-goal thresholds belong in `grade_calc.h`,
- and no catch-all `common.h` was introduced.

That is a useful constraint.
It keeps constant ownership aligned with module ownership instead of rebuilding the monolith through a dumping-ground header.

## 4) Placeholder Source Files Were Added for Build Safety, Not to Fake Progress

The new `source/*.c` files are compile-safe placeholders.
They do not claim that runtime logic has already been moved out of `main.c`.

This distinction matters:

- the scaffold is real,
- the build boundary is real,
- but `grade-guard/main.c` is still the behavioral source of truth until later Bolts perform the actual extraction and bug-fix work.

That is the correct state for Bolt 1.2.

## 5) Validation Was a Compile Smoke Test, Not a Runtime Refactor Test

The acceptance run for this version was:

```powershell
$buildDir = "grade-guard\build-smoke"
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
Get-ChildItem grade-guard\source\*.c | ForEach-Object {
    $output = Join-Path $buildDir ($_.BaseName + ".o")
    gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header -c $_.FullName -o $output
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}
```

Result:

- the smoke build passed with `gcc.exe (tdm64-1) 10.3.0`,
- object files were produced for all seven scaffold modules,
- and the repository now has evidence that each new source file can compile against its own public header.

That is the right level of proof for Bolt 1.2 because the deliverable is structural readiness, not behavior migration yet.

## 6) The Requirements Baseline Advanced, But Review Gates Remain Open

The local `REQUIREMENTS.md` now records that Bolt 1.2 has completed:

- design,
- implement,
- test,
- docs.

The review step remains unchecked.
Unit 1 human validation also remains unchecked.

That is intentional.
The scaffold should not be marked fully complete until a human explicitly agrees that:

- the file boundaries are correct,
- the header layering will support later extraction,
- and the placeholder contracts are stable enough for the next Units.

## 7) README and Changelog Were Updated to Match the New Checkpoint

The root `README.md` was updated from `v0.1.2` to `v0.1.3` and now points readers at:

- the new version-notes file,
- the scaffold documentation,
- the current compile-smoke checkpoint,
- and the next roadmap step after scaffold creation.

`CHANGELOG.md` was also updated so the short release record reflects:

- the new scaffold files,
- the new scaffold documentation,
- the local requirements progress note,
- the unchanged governance docs,
- and the manual cleanup list for generated artifacts.

## 8) Governance Docs Still Did Not Need More Changes

`CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` were reviewed in scope and left untouched.

Reason:

- this version changes source layout planning and release documentation,
- it does not change contributor process,
- it does not change reporting paths for vulnerabilities,
- and it does not change conduct expectations.

## Copy-Paste Commands

### Read the scaffold document

```powershell
Get-Content docs/unit-1-bolt-1-2-module-scaffold.md
```

### Re-check the Bolt 1.2 progress markers

```powershell
Get-Content REQUIREMENTS.md | Select-String "Bolt 1.2" -Context 0,10
```

### Re-run the compile smoke test

```powershell
$buildDir = "grade-guard\build-smoke"
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
Get-ChildItem grade-guard\source\*.c | ForEach-Object {
    $output = Join-Path $buildDir ($_.BaseName + ".o")
    gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header -c $_.FullName -o $output
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}
```

### Re-check local files before committing

```powershell
git status --short --ignored
```

## Pitfalls + Review Notes

### 1) This is still not the extraction itself

Do not describe `v0.1.3` as if the production logic has already been moved into the new modules.
This version creates the scaffold and proves it compiles; it does not finish the refactor.

### 2) Placeholder implementations are intentionally minimal

The stub functions exist to validate file boundaries and public interfaces.
They are not evidence that the final runtime architecture is complete.

### 3) Review gates are still open

The scaffold exists, but Bolt 1.2 review and Unit 1 human validation are still required before the Bolt should be checked off fully.

### 4) Local build artifacts still need manual cleanup

The repository currently contains:

- `grade-guard/build-smoke/`
- `grade-guard/grade-guard-warning-build.exe`
- `grade-guard/main.exe`

Those are local artifacts and should not be part of the source commit.

### 5) `main.c` remains the runtime source of truth

The codebase is still Windows-only and behaviorally monolithic today.
The scaffold improves extraction readiness, but later Units still have to move logic and resolve ownership bugs.

### 6) `REQUIREMENTS.md` is still ignored by `.gitignore`

The local Bolt 1.2 progress update may not appear in a normal `git status`.
Be deliberate about that when you prepare the manual commit so the release record matches what you actually intend to include.

## Skill Transfer: What This Version Clarifies

After reading the `v0.1.3` materials, a contributor should be able to explain:

1. Which public modules now exist on disk.
2. How the initial header dependency direction is supposed to work.
3. Why compile-safe placeholders are useful before moving behavior.
4. Why Bolt 1.2 can be evidence-backed while still keeping review gates open.

## 24-72 Hour Next Steps

1. Review and approve the public header layout and ownership rules in `docs/unit-1-bolt-1-2-module-scaffold.md`.
2. Delete `grade-guard/build-smoke/`, `grade-guard/grade-guard-warning-build.exe`, and `grade-guard/main.exe` before creating the manual commit.
3. Start moving the lowest-risk modules such as `vector` and `models` into the new scaffold while keeping `main.c` behavior intact.
4. Decide whether the local `REQUIREMENTS.md` progress record should be tracked explicitly in Git before later Bolts depend on it.

---

This file exists to explain `v0.1.3` in more detail than the changelog while staying aligned with the actual repository state and the current Bolt 1.2 checkpoint.

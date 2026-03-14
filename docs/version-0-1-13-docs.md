# Version 0.1.13 Notes

## Title
Unit 6.1 Build-and-Validation Release Trail for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.13` repository state if it now contains:

- a root `README.md` updated to `v0.1.13`,
- a new top `v0.1.13` entry in `CHANGELOG.md`,
- this file at `docs/version-0-1-13-docs.md`,
- `docs/unit-6-bolt-6-1-build-validation.md`,
- `scripts/build.ps1`,
- `scripts/validate.ps1`,
- and `grade-guard/unit-tests/app_smoke_test.c` printing `app smoke tests passed` when compiled and run through the validated acceptance workflow.

The governance documents changed only where the new workflow made that necessary:

- `CONTRIBUTING.md` was updated because the repository now has one preferred contributor build path and one preferred full-validation command.
- `SECURITY.md` was not changed because this release does not alter vulnerability-reporting policy.
- `CODE_OF_CONDUCT.md` was not changed because this release does not alter community-governance expectations.

## One-Sentence Objective

Capture the validated Unit 6.1 build, smoke-test, and release-trail workflow in the root release metadata so contributors and reviewers can reproduce the refactored repository state from a clean checkout.

## Why This Version Matters

The repository already had the Unit 6.1 implementation work in place by the end of the previous task:

- the root build path was standardized,
- the full validation pass existed,
- the app orchestration seam had smoke coverage,
- and the bolt-specific evidence note had been added.

What was still missing was the release trail around that work.
Without that trail, the repository had a mismatch:

- `README.md` still advertised `v0.1.12`,
- `CHANGELOG.md` still stopped at the Unit 5 fix release,
- contributors still had an older setup path in `CONTRIBUTING.md`,
- and the new Unit 6.1 workflow was documented as an implementation artifact but not yet as the active release baseline.

`v0.1.13` closes that gap.
It does not change application behavior beyond the already-validated Unit 6.1 work.
Instead, it publishes the correct release metadata and contributor guidance for that work:

- the root docs now point to the right version,
- the contributor setup now prefers the validated PowerShell scripts,
- the changelog now records the new smoke harness and workflow scripts,
- and the version note now explains the change in one place without overloading `CHANGELOG.md`.

## What Changed (Artifact Map)

- Root release metadata:
  - `README.md`
  - `CHANGELOG.md`
- Version-specific release note:
  - `docs/version-0-1-13-docs.md`
- Unit 6.1 bolt note:
  - `docs/unit-6-bolt-6-1-build-validation.md`
- Contributor workflow doc:
  - `CONTRIBUTING.md`
- Validated build/validation scripts:
  - `scripts/build.ps1`
  - `scripts/validate.ps1`
- App orchestration smoke harness:
  - `grade-guard/unit-tests/app_smoke_test.c`

## Guided Walkthrough

## 1) The Root Release Metadata Now Matches the Unit 6.1 Reality

The most visible repository-level fix is simple but important:

- `README.md` now advertises `v0.1.13`,
- its top release-note link now points to this file,
- and its body still points to `docs/unit-6-bolt-6-1-build-validation.md` as the deeper bolt-specific evidence source.

That means someone landing on the repository no longer sees a stale release number after the Unit 6.1 workflow update.

## 2) The Validated Contributor Path Is Now Explicit

Before this release-trail update, `CONTRIBUTING.md` still taught a manual compile flow as the default setup path.
That was no longer the best representation of the repository because the validated baseline had moved to:

- `powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1`
- `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1`

This version updates the contributor guide to match the actual accepted baseline.
That matters because build instructions are part of project quality.
If the docs teach a less-preferred path after a validated workflow exists, contributors will naturally diverge.

## 3) The App Smoke Harness Is Part of the Published Release Story

`grade-guard/unit-tests/app_smoke_test.c` is not just another test file.
It is the first focused proof that the extracted runtime path through `app.c` holds together as a real application workflow:

- startup,
- profile creation,
- profile selection,
- save/load,
- grading persistence,
- deletion persistence,
- and exit.

This release note calls that out explicitly because it changes the confidence level of the repository.
The project no longer relies only on module-level tests plus manual confidence for the app orchestrator.

## 4) The Unit 6.1 Bolt Note and This Version Note Serve Different Jobs

These two files intentionally coexist:

- `docs/unit-6-bolt-6-1-build-validation.md` is the bolt-level engineering artifact,
- `docs/version-0-1-13-docs.md` is the release-level walkthrough.

The bolt note is where you go for:

- the exact executable contract,
- ADR bullets,
- validation matrix,
- and bolt review status.

This version note is where you go for:

- the release rationale,
- the doc-and-workflow alignment story,
- and the explanation of why `CONTRIBUTING.md` changed while `SECURITY.md` and `CODE_OF_CONDUCT.md` did not.

## 5) Source Attribution Stayed Consistent

The new `grade-guard/unit-tests/app_smoke_test.c` file now carries the same UP Mindanao / BSCS / `CMSC 18 - COMPUTER PROGRAMING I` attribution block and full Apache 2.0 license comment used across the rest of `grade-guard/`.
That is a small detail, but it matters because `v0.1.9` established repository-wide source provenance as a tracked project standard.

## Validation Evidence

This release-trail update relies on the already-completed Unit 6.1 acceptance pass.
No runtime code behavior was changed after that pass; the remaining edits in this version are documentation and contributor-workflow alignment.

Validated acceptance evidence already present in the repository:

- Baseline toolchain check (2026-03-14):
  - `gcc --version`
  - Result: `gcc.exe (tdm64-1) 10.3.0`
- Application build (2026-03-14):
  - `powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1`
  - Result: built `artifacts/unit-6-bolt-6-1/grade-guard.exe`
- Full validation run (2026-03-14):
  - `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1`
  - Result:
    - `vector tests passed`
    - `models lifecycle tests passed`
    - `persistence contract tests passed`
    - `unit 5 defect regressions passed`
    - `app smoke tests passed`

For the full engineering breakdown, see `docs/unit-6-bolt-6-1-build-validation.md`.

## Copy-Paste Commands

### Read this release note

```powershell
Get-Content docs/version-0-1-13-docs.md
```

### Rebuild the validated application target

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1
```

### Re-run the full validation workflow

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1
```

## Pitfalls + Debugging

### 1) Do not mistake this release for a new runtime feature drop

- The runtime-impacting Unit 6.1 work was already implemented and validated.
- `v0.1.13` primarily publishes the correct release metadata and contributor workflow for that work.

### 2) Do not forget that live Windows console-host review is still open

- The app smoke harness covers orchestration.
- It does not replace a real console-host pass for redraw, fullscreen, and raw-key behavior.

### 3) Do not ignore generated artifacts before manual commits

- `scripts/validate.ps1` leaves binaries, runtime directories, and CSV outputs in place intentionally.
- Review the latest `CHANGELOG.md` `For Deletion` section before a manual commit.

### 4) Do not treat old manual compile instructions as the preferred baseline

- Manual compile commands are still useful for low-level debugging.
- The preferred contributor path is now the script-based GCC workflow documented in `README.md` and `CONTRIBUTING.md`.

## Immediate Next Actions

1. Manually delete the generated binaries and CSV/runtime artifacts listed in `CHANGELOG.md` if they are not meant to be committed.
2. Decide whether Unit 6.2 should now update `README.md`, `CHANGELOG.md`, and release notes again for a broader release-readiness cut, or whether the next task should return to controller-side numeric validation.
3. Run the live Windows console-host walkthrough that still remains open after Unit 6.1.
4. Keep the script-based validation path green as future runtime and documentation work lands.

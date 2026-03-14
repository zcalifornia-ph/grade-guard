# Unit 6 Bolt 6.1: Finalize Build and Validation Workflow

Objective: lock the refactored repository to one reproducible GCC baseline, prove the real application entry-point orchestration with a headless smoke harness, and leave contributor-facing build/validation steps that can be rerun from a clean checkout.

## Executable Contract

Goal / deliverable

- Define one validated Windows build baseline with explicit warning flags and one repeatable validation entry point.
- Keep `grade-guard/main.c` as a thin handoff to the extracted application orchestrator while proving the linked module set is complete.
- Add smoke coverage for startup, profile creation, profile selection, persistence, grading, deletion, and exit without depending on live raw-key console input.
- Record the exact commands, toolchain, residual manual-review scope, and evidence under a Unit 6 artifact.

Bolt checklist

- Design: choose the baseline compiler, warning flags, output layout, and validation matrix for the refactored repository.
- Implement: add project-level build and validation scripts plus a headless `app.c` smoke harness.
- Test: run the full application build and the focused vector, lifecycle, persistence, Unit 5, and app-orchestration smoke suites.
- Docs: update contributor-facing build/validation instructions and capture the Unit 6.1 proof trail.
- Review: confirm the workflow is reproducible from a clean checkout on the validated toolchain and leave non-automated console-host risk explicit.

Acceptance criteria interpreted for this Bolt

- `grade-guard/main.c` stays orchestration-only and the validated app build links the extracted modules through `scripts/build.ps1`.
- The repository builds with `gcc` using `-std=c17 -Wall -Wextra -pedantic`.
- A regression checklist exists and passes for profile creation, selection, save/load, grading, deletion, and exit via focused automated suites plus the recorded residual live-host note.
- `README.md` describes the new layout, exact build command, validation command, baseline toolchain, and remaining limitations.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Thin entry point: `grade-guard/main.c`
- Application orchestration: `grade-guard/source/app.c`
- Build automation: `scripts/build.ps1` and `scripts/validate.ps1`
- Automated workflow proof: `grade-guard/unit-tests/app_smoke_test.c`
- Contributor-facing documentation: `README.md`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because the baseline has to compile cleanly under one explicit warning set instead of ad hoc local commands.
- NFR-05 matters because the app smoke test exercises real profile allocation, persistence, reload, and deletion-side teardown paths.
- NFR-06 matters because the project still depends on Windows console host behavior for fullscreen, redraw, and raw-key input even after orchestration is covered headlessly.
- The main brownfield risk was false validation: the repository already had focused module suites, but it lacked one top-level proof that `app.c` could create, load, select, mutate, and save profiles with the extracted module boundaries wired together.

## Context Summary

- `grade-guard/main.c` already delegates directly to `app_run()`, so the entry-point objective was satisfied before Bolt 6.1 started.
- `README.md` documented direct compile commands, but there was no single validated contributor workflow that rebuilt the app and every focused regression suite together.
- The repo already had strong module-level coverage for vectors, model lifecycle, persistence, and Unit 5 grade defects.
- `app.c` was still missing dedicated smoke coverage for startup, menu orchestration, profile creation, selection, persistence save/load, and exit.
- The repository is Windows-only, so the build baseline should reflect the actual available toolchain instead of claiming broad compiler parity without evidence.
- Existing persistence tests already generate CSV/runtime artifacts under `grade-guard/unit-tests`, which means the validation workflow must leave artifact cleanup explicit instead of silently deleting files.

## ADR Bullets

- Standardize on the repo's available `gcc` baseline (`tdm64-1` / GCC 10.3.0 in the acceptance environment) and keep the warning set at `-std=c17 -Wall -Wextra -pedantic`.
- Add `scripts/build.ps1` for the application and `scripts/validate.ps1` for the full acceptance run so contributors can rebuild the repository with one command path.
- Cover `app.c` through a headless test seam that stubs the UI/controller boundary while linking the real application, persistence, model, vector, and grade modules.
- Keep live Windows console-host behavior as an explicit residual manual-review risk instead of pretending fullscreen and raw-key flows are fully automated.

## Baseline Build and Validation Commands

Validated baseline toolchain

- Windows PowerShell with `gcc` available on `PATH`
- Acceptance environment toolchain: `gcc.exe (tdm64-1) 10.3.0`

Validated warning flags

- `-std=c17 -Wall -Wextra -pedantic`

Contributor commands

- Application build: `powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1`
- Full validation: `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1`
- Generated application binary: `artifacts/unit-6-bolt-6-1/grade-guard.exe`

## Validation Matrix

| Workflow slice | Proof source | Result |
| --- | --- | --- |
| Entry-point thinness and full app link | `scripts/build.ps1` build of `grade-guard/main.c` + extracted modules | Passed |
| Shared vector behavior | `grade-guard/tests/vector_test.c` | Passed |
| Model lifecycle and deletion ownership | `grade-guard/unit-tests/models_lifecycle_test.c` | Passed |
| Persistence save/load/list contract | `grade-guard/unit-tests/persistence_contract_test.c` | Passed |
| Grading defect regressions | `grade-guard/unit-tests/unit5_defect_baseline_test.c` | Passed |
| Startup, profile creation, selection, save/load, grading, deletion, and exit orchestration | `grade-guard/unit-tests/app_smoke_test.c` | Passed |
| Live Windows fullscreen/raw-key/redraw host behavior | Manual follow-up still required | Open residual risk |

## Validation Evidence

- Baseline toolchain check (2026-03-14): `gcc --version` reported `gcc.exe (tdm64-1) 10.3.0`.
- Acceptance build (2026-03-14): `powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1` succeeded and emitted `artifacts/unit-6-bolt-6-1/grade-guard.exe`.
- Acceptance validation run (2026-03-14): `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1` succeeded.
- Validation suites executed by the acceptance run (2026-03-14):
  - `vector_test.exe` printed `vector tests passed`
  - `models_lifecycle_test.exe` printed `models lifecycle tests passed`
  - `persistence_contract_test.exe` printed `persistence contract tests passed`
  - `unit5_defect_baseline_test.exe` printed `unit 5 defect regressions passed`
  - `app_smoke_test.exe` printed `app smoke tests passed`

## Regression Checklist

- [x] Startup initializes the app orchestrator and UI setup path once per run.
- [x] New profile flow persists a valid profile to the numbered CSV contract.
- [x] Existing profile selection loads the saved profile set and targets the expected student record.
- [x] Save/load round-trip retains grading inputs and predicted-GWA calculability.
- [x] Deletion-side state mutation survives exit/save and reload.
- [x] Exit returns cleanly after persistence succeeds.
- [ ] Live Windows host review of fullscreen/raw-key/redraw behavior remains manual.

## Review Status

- Bolt 6.1 is complete on the validated GCC baseline: the repository now has one reproducible build path, one acceptance validation command, and one headless smoke harness covering the real `app.c` orchestration seam.
- The Unit 6 human review gate is still open overall because Bolt 6.2 documentation/release artifacts and live Windows console-host acceptance remain pending.
- Generated `.csv` and `.exe` artifacts remain in the workspace for manual cleanup, consistent with the repository task rules.

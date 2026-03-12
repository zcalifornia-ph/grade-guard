# Changelog

Status: source-backed Windows console grade-tracking prototype with extracted utility, domain lifecycle, and UI/platform modules.

## v0.1.6 - 2026-03-12

### Added or Changed
- Updated `README.md` from `v0.1.5` to `v0.1.6`, refreshed the project status/progress snapshot, documented the extracted `ui_console` layer, corrected the standard build commands to link `grade-guard/source/ui_console.c`, and pointed the release trail at the new Bolt 3.1 notes.
- Implemented the shared UI/platform module in `grade-guard/header/ui_console.h` and `grade-guard/source/ui_console.c`, including screen setup, cursor and color helpers, selection widgets, field input, failure display, and the new `ui_clear_screen()` / `ui_read_key()` wrappers that isolate direct console calls.
- Updated `grade-guard/main.c` to consume `header/ui_console.h` instead of defining raw Windows console helpers inline, and switched UI call sites to the typed `UI_LIST_*` and `UI_KEY_*` interface exported by the shared console module.
- Corrected a few inherited console-layer defects during extraction, including the `UI_SELECTION_BOTH` fallthrough path, selection-label alphabet wraparound, and a stray variadic initialization misuse inside the menu widget.
- Added `docs/unit-3-bolt-3-1-ui-console.md` to document the Bolt 3.1 contract, Windows-specific dependencies, UI-boundary review notes, and current validation evidence.
- Added `docs/version-0-1-6-docs.md` with the fuller explanation of the UI extraction, release impact, remaining manual-acceptance gap, and updated verification trail.
- Updated `CONTRIBUTING.md` because the standard build now links `ui_console.c`, and UI-touching changes now explicitly require a live Windows console verification pass in addition to compile/test checks.
- Kept `REQUIREMENTS.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because Bolt 3.1 manual acceptance has not yet been executed and this version does not alter project governance or security-reporting policy.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory from earlier scaffold validation; still present locally and should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check build artifact; still present locally and should not be committed).
- `grade-guard/bolt-2-1-main.exe` (local acceptance-build artifact generated while validating Bolt 2.1).
- `grade-guard/bolt-2-2-main.exe` (local acceptance-build artifact generated while validating Bolt 2.2).
- `grade-guard/bolt-3-1-main.exe` (local acceptance-build artifact generated while validating Bolt 3.1).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary generated while validating Bolts 2.1 through 3.1).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary generated while validating Bolts 2.2 and 3.1).

## v0.1.5 - 2026-03-12

### Added or Changed
- Updated `README.md` from `v0.1.4` to `v0.1.5`, refreshed the project status/progress snapshot, and documented the new models-module build/test flow plus the Bolt 2.2 release notes link.
- Implemented the domain lifecycle module in `grade-guard/header/models.h` and `grade-guard/source/models.c`, including `*_init()`, `*_reset()`, and `*_destroy()` helpers for `Activities`, `Course_Parameter`, `Course_Component`, `Course`, and `Student_Profile`.
- Updated `grade-guard/main.c` to consume `header/models.h`, removed duplicate in-file domain declarations/destructors, and aligned create/add/delete flows with vector by-value ownership and `vector_remove()` cleanup semantics.
- Added the first shared lifecycle test harness under `grade-guard/unit-tests/`, including `test_framework.h` and `models_lifecycle_test.c`, then validated it with a local executable acceptance run.
- Added `docs/unit-2-bolt-2-2-models.md` to document the Bolt 2.2 ownership contract, nested invariants, review notes, and acceptance evidence.
- Added `docs/version-0-1-5-docs.md` with the fuller explanation of the Bolt 2.2 extraction, repository-level release trail, validation commands, and remaining review gates.
- Updated `REQUIREMENTS.md` so Unit 2 / Bolt 2.2 is checked off with dated implementation, validation, and review evidence; the Unit 2 human validation line remains open.
- Updated `CONTRIBUTING.md` because contributors now need the `models.c` link step in normal builds and a second focused regression check when touching shared domain ownership flows.
- Updated `SECURITY.md` to correct the supported-version table for the active `0.1.x` line; kept `CODE_OF_CONDUCT.md` unchanged because this version does not alter community-governance policy.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory from earlier scaffold validation; still present locally and should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check build artifact; still present locally and should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; still present locally and should not be committed).
- `grade-guard/bolt-2-1-main.exe` (local acceptance-build artifact generated while validating Bolt 2.1).
- `grade-guard/bolt-2-2-main.exe` (local acceptance-build artifact generated while validating Bolt 2.2).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary generated while validating Bolts 2.1 and 2.2).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary generated while validating Bolt 2.2).

## v0.1.4 - 2026-03-12

### Added or Changed
- Updated `README.md` from `v0.1.3` to `v0.1.4`, refreshed the project status/progress snapshot, and pointed the root documentation at the extracted vector module plus its regression harness.
- Implemented the shared vector module in `grade-guard/header/vector.h` and `grade-guard/source/vector.c`, and removed the duplicate live vector implementation from `grade-guard/main.c`.
- Added `grade-guard/tests/vector_test.c` with repeatable regression coverage for create, push, pop, remove, resize, and indexed access across value and pointer storage cases.
- Added `docs/unit-2-bolt-2-1-vector.md` to document the Bolt 2.1 contract, vector ownership rules, failure behavior, review notes, and acceptance evidence.
- Added `docs/version-0-1-4-docs.md` with the fuller explanation of the Bolt 2.1 vector extraction, validation evidence, build/test workflow updates, and remaining review gates.
- Updated `CONTRIBUTING.md` and `REQUIREMENTS.md` to reflect the new vector-linked build flow, targeted regression check, and Bolt 2.1 completion state.
- Kept `SECURITY.md` and `CODE_OF_CONDUCT.md` unchanged because this version changes implementation structure and contributor verification guidance, not security reporting or community-governance policy.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory from earlier scaffold validation; still present locally and should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check build artifact; still present locally and should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; still present locally and should not be committed).
- `grade-guard/bolt-2-1-main.exe` (local acceptance-build artifact generated while validating Bolt 2.1).
- `grade-guard/tests/vector_test.exe` (local regression-test binary generated while validating Bolt 2.1).

## v0.1.3 - 2026-03-12

### Added or Changed
- Added the first module scaffold under `grade-guard/header/` and `grade-guard/source/`, including public headers and compile-safe placeholder source files for `app`, `vector`, `models`, `grade_calc`, `persistence`, `ui_console`, and `profile_controller`.
- Added `docs/unit-1-bolt-1-2-module-scaffold.md` to document the Bolt 1.2 contract, module inventory, include-guard convention, dependency layering rules, shared-constants strategy, ownership rules, and compile-smoke evidence.
- Updated `REQUIREMENTS.md` so Unit 1 / Bolt 1.2 now records completed design, implement, test, and docs subtasks plus dated smoke-build evidence; review and human validation remain open.
- Updated `README.md` from `v0.1.2` to `v0.1.3` and refreshed the project status, progress checkpoint, and roadmap to reflect that the interface scaffold now exists and has been compile-checked.
- Added `docs/version-0-1-3-docs.md` with the fuller explanation of the Bolt 1.2 scaffold, why the source tree remains placeholder-only, and what still requires human approval before extraction accelerates.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version changes repository structure and release documentation, not contributor workflow, governance policy, or vulnerability reporting flow.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory generated while validating the new scaffold; should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check binary artifact; should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).

## v0.1.2 - 2026-03-12

### Added or Changed
- Added `docs/unit-1-bolt-1-1-monolith-inventory.md` to inventory `grade-guard/main.c`, define the module ownership charter, map top-level functions to destination modules, and document the thin-entry-point target for the future refactor.
- Updated the local planning baseline in `REQUIREMENTS.md` so Unit 1 / Bolt 1.1 now records completed design, implementation, test, and docs subtasks plus evidence from the static coverage check; human review remains open.
- Updated `README.md` from `v0.1.1` to `v0.1.2` and pointed the project overview at the new architecture inventory and Bolt 1.1 progress state.
- Added `docs/version-0-1-2-docs.md` with the fuller explanation of the Unit 1 / Bolt 1.1 inventory, validation method, remaining review gate, and manual cleanup notes.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version extends architecture-planning documentation rather than contributor policy, governance, or security reporting flow.

### For Deletion
- `grade-guard/grade-guard-warning-build.exe` (local warning-check binary generated during compile validation; should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).

## v0.1.1 - 2026-03-12

### Added or Changed
- Added root `REQUIREMENTS.md` as the planning baseline for the next engineering phase, including intent, assumptions, user stories, NFRs, risk register, Units, Bolts, architecture, testing, and operations coverage for the Grade Guard refactor.
- Updated `README.md` from `v0.1.0` to `v0.1.1` and documented that the repository now includes a requirements-driven plan for modularizing `grade-guard/main.c` into `header/` and `source/` directories.
- Added `docs/version-0-1-1-docs.md` with a fuller explanation of the new requirements artifact, the intended refactor path, and the validation notes that informed the planning scope.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version adds planning and release documentation rather than changing contributor policy or security reporting expectations.

### For Deletion
- `grade-guard/grade-guard-warning-build.exe` (local warning-check binary generated during compile validation; should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).
- `grade-guard/tempCodeRunnerFile.c` (editor-generated scratch file; should not be committed).

## v0.1.0 - 2026-03-12

### Added or Changed
- Added the initial Grade Guard prototype source tree under `grade-guard/`, anchored by the Windows console implementation in `grade-guard/main.c`.
- Updated `README.md` from `v0.0.1` to `v0.1.0` so the repository now documents the concrete profile, course, activity, persistence, and grade-goal workflows present in code.
- Added `docs/version-0-1-0-docs.md` with a fuller walkthrough of the imported prototype, current limitations, and cleanup notes for local artifacts.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because the prototype source import does not invalidate the governance baseline introduced in `v0.0.1`.

### For Deletion
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).
- `grade-guard/tempCodeRunnerFile.c` (editor-generated scratch file; should not be committed).

## v0.0.1 - 2026-03-12

### Added or Changed
- Initialized root repository documentation for Grade Guard.
- Replaced generic or template root docs with project-specific `README.md`, `CODE_OF_CONDUCT.md`, `CONTRIBUTING.md`, and `SECURITY.md`.
- Documented the current Apache 2.0 licensing, maintainer contact details, and project metadata from `agent/init.cfg`.
- Added `docs/version-0-0-1-docs.md` to capture detailed version notes for the initial repository baseline.
- Updated `.gitignore` to ignore `.gitignore` itself and hide the agent workflow files from future commits, as required by the initialization task.

### For Deletion
- None from this task context (documentation-only initialization; no generated build artifacts were created).

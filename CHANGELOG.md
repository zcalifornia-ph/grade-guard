# Changelog

Status: source-backed Windows console grade-tracking prototype.

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

# Changelog

Status: source-backed Windows console grade-tracking prototype.

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

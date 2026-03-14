# Version 1.0.0 Notes

## Title
Grade Guard 1.0.0 Pull Request Baseline

## Quick Diagnostic Read

You are looking at the correct `v1.0.0` repository state if it now contains:

- a root `README.md` updated to `v1.0.0`,
- a new top `v1.0.0` entry in `CHANGELOG.md`,
- this file at `docs/version-1-0-0-docs.md`,
- `scripts/build.ps1` defaulting to `artifacts/build/grade-guard.exe`,
- and `scripts/validate.ps1` defaulting to `artifacts/validation/`.

## One-Sentence Objective

Prepare the repository for a `1.0.0` pull request by promoting the root release metadata to a stable baseline and rewriting the main project documentation around the shipped Grade Guard application.

## Why This Version Matters

The `0.1.x` series captured the transition from an imported classroom prototype to a modular Windows console application with repeatable validation coverage.
`v1.0.0` turns that prerelease trail into a cleaner public baseline:

- the README now explains the application instead of the internal milestone tracker,
- the changelog now reflects a release-facing history,
- contributor and security guidance now reference the `1.0.x` line,
- and the build scripts now publish to generic artifact locations by default.

This release does not redefine the product.
It packages the current application state more clearly for reviewers, contributors, and future users.

## What Changed

- Root release metadata:
  - `README.md`
  - `CHANGELOG.md`
- Contributor and security guidance:
  - `CONTRIBUTING.md`
  - `SECURITY.md`
- Build and validation defaults:
  - `scripts/build.ps1`
  - `scripts/validate.ps1`
- Version-specific release note:
  - `docs/version-1-0-0-docs.md`

## Current Application Snapshot

Grade Guard currently provides:

- local student-profile creation and selection,
- course tracking with lecture-only or lecture-plus-laboratory setups,
- weighted assessment categories and activity scores,
- predicted GWA and grade-goal progress calculations,
- local CSV persistence for new saves plus compatibility for older saved profiles,
- automated regression coverage for vectors, model lifecycle handling, persistence, grading edge cases, and application smoke flows.

## Known Limitations

- The runtime is still Windows-only.
- Live console-host behavior should still be verified manually when UI behavior changes.
- Numeric input validation can still be tightened at the controller boundary.

## Validation

Recommended release verification command:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1
```

Validated acceptance evidence (2026-03-14):

- `vector tests passed`
- `models lifecycle tests passed`
- `persistence contract tests passed`
- `unit 5 defect regressions passed`
- `app smoke tests passed`
- `Validation pipeline passed.`

Expected coverage:

- vector regression
- model lifecycle regression
- persistence contract regression
- grading defect regression
- application smoke testing

## Cleanup Notes

This documentation pass intentionally does not delete generated artifacts automatically.
At the time of this `v1.0.0` update, the workspace still contains:

- older prerelease validation directories under `artifacts/unit-6-bolt-6-1/` and `artifacts/unit-6-bolt-6-2/`,
- the current `artifacts/validation/` output from the updated validation script,
- local executable artifacts in the repository root and under `grade-guard/`, `grade-guard/tests/`, and `grade-guard/unit-tests/`,
- generated CSV fixtures and runtime directories under `grade-guard/unit-tests/`,
- and the root `0.csv` working-directory profile file.

Use the `v1.0.0` `CHANGELOG.md` entry as the manual cleanup checklist.

## Immediate Next Actions

1. Run the validation workflow and confirm the generated artifacts remain local-only.
2. Perform a live Windows console walkthrough if the pull request includes UI behavior changes.
3. Review the open issues list for follow-up work after the `1.0.0` merge.

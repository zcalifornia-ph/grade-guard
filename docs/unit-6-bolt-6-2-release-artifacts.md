# Unit 6 Bolt 6.2: Update Documentation and Release Artifacts

Objective: close the Unit 6 documentation and release-artifact gap by aligning the root release metadata with the validated modular repository state, while leaving manual sign-off blockers explicit instead of implying they are solved.

## Executable Contract

Goal / deliverable

- Update the root release metadata so it reflects the modular layout, the validated GCC workflow, and the current limitations of the repository.
- Publish one bolt-specific note and one release-level note for the release-artifact pass.
- Summarize completed units, deferred work, and operator guidance for generated artifacts and release review.
- Re-run the accepted validation workflow so the documentation points to fresh evidence rather than inherited assumptions.

Bolt checklist

- Design: identify the root docs and release artifacts that must change for the post-modular release state.
- Implement: update `README.md`, `CHANGELOG.md`, the version notes, and `REQUIREMENTS.md`.
- Test: verify the published commands, file paths, and known limitations against the real repository plus a fresh acceptance run.
- Docs: summarize completed work, deferred follow-ups, and manual cleanup guidance.
- Review: record whether the repository is releasable now or which blockers still remain.

Acceptance criteria interpreted for this Bolt

- `README.md` advertises the current version, modular file layout, validated build/validation commands, and known limitations without leaving stale Unit 6.2 language behind.
- `CHANGELOG.md` records the new release-artifact pass and lists the currently visible generated artifacts that still need manual cleanup.
- A version note explains how the release-artifact pass relates to the already-validated Unit 6.1 workflow and what still blocks final sign-off.
- `REQUIREMENTS.md` records Bolt 6.2 as complete while leaving the separate Unit 6 human-validation line explicit until a human approves it.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Root release metadata: `README.md` and `CHANGELOG.md`
- Prior validation proof: `docs/unit-6-bolt-6-1-build-validation.md`
- New bolt note: `docs/unit-6-bolt-6-2-release-artifacts.md`
- New release note: `docs/version-0-1-14-docs.md`
- Acceptance command: `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because published build and validation guidance must stay aligned with the validated warning flags and actual script entry points.
- NFR-05 matters because the release trail should remain traceable to concrete evidence, not just narrative status text.
- NFR-06 still matters because the application depends on Windows console-host behavior that is not fully covered by the automated acceptance run.
- The brownfield risk for this bolt was stale metadata: after Unit 6.1, the workflow was validated, but the release trail still said the release-artifact pass was pending.

## Context Summary

- `README.md` still advertised `v0.1.13` and still described Unit 6.2 as pending work.
- `CHANGELOG.md` still said the repo was waiting on Unit 6.2 release artifacts, even though the remaining gap was primarily documentation alignment and sign-off.
- The repository already had a validated build path in `scripts/build.ps1` and a validated acceptance path in `scripts/validate.ps1`.
- `docs/unit-6-bolt-6-1-build-validation.md` already held the engineering proof for the GCC baseline and smoke coverage.
- The current workspace still contains generated `.exe` and `.csv` artifacts that must be reported for manual cleanup instead of deleted automatically.
- The remaining true blockers are manual: a live Windows console-host walkthrough and explicit Unit 6 human approval.

## ADR Bullets

- Publish this pass as `v0.1.14` so the release trail distinguishes workflow formalization (Unit 6.1) from release-artifact closure (Unit 6.2).
- Keep `docs/unit-6-bolt-6-1-build-validation.md` as the source of truth for the build/validation matrix and use this bolt note for release-readiness tracking.
- Re-run `scripts/validate.ps1` unchanged instead of inventing a new validation path for documentation-only work.
- Keep generated artifacts documented for manual cleanup instead of removing them automatically.
- Leave the Unit 6 human-validation line open until a human explicitly approves the release-readiness criteria and documentation scope.

## Release-Readiness Checklist

- [x] Root version metadata points to `v0.1.14`.
- [x] `README.md` describes the modular layout, validated commands, and current limitations without stale Unit 6.2 wording.
- [x] `CHANGELOG.md` records the release-artifact pass and current cleanup targets.
- [x] A new release note exists at `docs/version-0-1-14-docs.md`.
- [x] A bolt-specific release-artifact note exists at `docs/unit-6-bolt-6-2-release-artifacts.md`.
- [x] The accepted validation workflow was re-run after the documentation changes.
- [ ] Live Windows console-host walkthrough is still required.
- [ ] Unit 6 human validation approval is still required.

## Validation Evidence

- Acceptance validation run (2026-03-14): `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1 -OutputDir artifacts\unit-6-bolt-6-2` succeeded.
- Validation suites executed by the acceptance run (2026-03-14):
  - `vector_test.exe` printed `vector tests passed`
  - `models_lifecycle_test.exe` printed `models lifecycle tests passed`
  - `persistence_contract_test.exe` printed `persistence contract tests passed`
  - `unit5_defect_baseline_test.exe` printed `unit 5 defect regressions passed`
  - `app_smoke_test.exe` printed `app smoke tests passed`
- Local rerun note (2026-03-14): the default `artifacts/unit-6-bolt-6-1/grade-guard.exe` output could not be overwritten in this workspace, so the same validation script was rerun against a fresh `artifacts/unit-6-bolt-6-2/` directory without changing the validated build logic.
- Documentation consistency check (2026-03-14): the published version note, bolt note, changelog entry, and `README.md` paths now match the real files under `docs/`, `scripts/`, `artifacts/`, and `grade-guard/`.
- Manual-cleanup inventory check (2026-03-14): generated artifacts remain under `artifacts/unit-6-bolt-6-1/`, `artifacts/unit-6-bolt-6-2/`, `grade-guard/build-smoke/`, `grade-guard/tests/`, `grade-guard/unit-tests/`, and the root `grade-guard/` working directory, and they are documented rather than deleted.

## Review Status

- Bolt 6.2 is complete as a release-artifact and documentation-alignment pass.
- Unit 6 is not fully closed yet because the separate human-validation gate still requires explicit approval.
- The remaining release blocker is a live Windows console-host walkthrough for redraw, fullscreen, and raw-key behavior under a real host.
- Controller-side numeric validation remains a documented follow-up gap, but it is tracked as deferred product work rather than hidden by the release notes.
- Generated `.exe` and `.csv` artifacts remain in the workspace for manual cleanup, consistent with the repository task rules.

# Version 0.1.14 Notes

## Title
Unit 6.2 Release-Artifact Closure for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.14` repository state if it now contains:

- a root `README.md` updated to `v0.1.14`,
- a new top `v0.1.14` entry in `CHANGELOG.md`,
- this file at `docs/version-0-1-14-docs.md`,
- `docs/unit-6-bolt-6-2-release-artifacts.md`,
- an updated `REQUIREMENTS.md` entry showing Bolt 6.2 complete,
- and a still-green `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1` acceptance path.

The governance and contributor-workflow documents changed only where this pass required it:

- `CONTRIBUTING.md` was not changed because the preferred build and validation workflow did not change after `v0.1.13`.
- `SECURITY.md` was not changed because this release does not alter vulnerability-reporting policy.
- `CODE_OF_CONDUCT.md` was not changed because this release does not alter community-governance expectations.

## One-Sentence Objective

Publish the final Unit 6.2 release-artifact trail for the modular repository state while keeping the remaining manual sign-off blockers visible and reusing the validated Unit 6.1 acceptance workflow as evidence.

## Why This Version Matters

`v0.1.13` solved one important problem: it made the validated GCC build and smoke-check path explicit.
What it did not finish was the broader release trail around that work.

That left the repository in an awkward state:

- the build workflow was validated,
- the smoke harness was present,
- the root docs already pointed to the Unit 6.1 proof,
- but the top-level status text still said the release-artifact pass itself was pending.

`v0.1.14` closes that release-trail gap.
It does not add new runtime behavior.
Instead, it publishes the release-artifact closure around the existing modular repository state:

- the root docs now advertise the next release note,
- the changelog now records the release-artifact pass itself,
- the requirements file now shows Bolt 6.2 complete,
- and the new bolt note records what still blocks final release sign-off.

This distinction matters.
If the repository claims Unit 6.2 is still pending after the documentation has already been aligned, future tasks lose traceability and reviewers cannot tell whether the remaining work is technical, documentary, or simply approval-related.

## What Changed (Artifact Map)

- Root release metadata:
  - `README.md`
  - `CHANGELOG.md`
- Planning / execution tracker:
  - `REQUIREMENTS.md`
- Bolt-specific release note:
  - `docs/unit-6-bolt-6-2-release-artifacts.md`
- Version-specific release note:
  - `docs/version-0-1-14-docs.md`
- Validation source of truth reused by this release:
  - `docs/unit-6-bolt-6-1-build-validation.md`
  - `scripts/validate.ps1`

## Guided Walkthrough

## 1) The Root Status Text No Longer Pretends Unit 6.2 Is Still Undone

Before this pass, the repository homepage still said the remaining open work included Unit 6.2 release artifacts.
That stopped being precise once the remaining gap was really about manual sign-off and live-host review.

`README.md` now reflects the better state description:

- the Unit 6 documentation pass is published,
- the build and validation path is still the same one established in Unit 6.1,
- and the remaining blockers are called out as manual review and approval work.

## 2) Unit 6.1 and Unit 6.2 Now Have Separate, Clear Jobs

These two Unit 6 artifacts now divide responsibility cleanly:

- `docs/unit-6-bolt-6-1-build-validation.md` remains the engineering proof for the GCC baseline and automated acceptance workflow.
- `docs/unit-6-bolt-6-2-release-artifacts.md` now handles release metadata alignment, cleanup guidance, and final blocker tracking.

That separation keeps the build-validation evidence stable while still giving the release trail its own explicit home.

## 3) `REQUIREMENTS.md` Now Matches the Real Task State

The root requirements file is not just a wishlist; it is the repository's execution memory.
After this pass, it now records that Bolt 6.2 design, implementation, validation, documentation, and review work are complete.

Importantly, it does not hide the remaining human gate.
The separate Unit 6 approval line stays open until a human explicitly approves the release-readiness criteria and documentation scope.

## 4) Manual Cleanup Guidance Remains Intentional

This repository's task rules explicitly avoid deleting generated artifacts automatically.
That means release documentation must do the opposite of pretending the tree is clean.

`CHANGELOG.md` and the new Unit 6.2 note now keep the cleanup story explicit:

- build outputs remain under `artifacts/unit-6-bolt-6-1/`,
- older acceptance binaries remain in `grade-guard/`,
- regression outputs remain in `grade-guard/tests/` and `grade-guard/unit-tests/`,
- and runtime CSV fixtures remain in the working directories used by the persistence and app-smoke suites.

## 5) What Still Blocks Final Release Sign-Off

`v0.1.14` is a release-artifact closure, not a claim that all release risk is gone.
The repository still has two explicit manual blockers:

- a live Windows console-host walkthrough for redraw, fullscreen, and raw-key behavior,
- and explicit Unit 6 human approval of the release-readiness criteria and documentation scope.

Controller-side numeric validation also remains an open follow-up item, but it is documented as deferred product work rather than silently erased from the roadmap.

## Validation Evidence

This release-artifact pass reuses the established Unit 6.1 validation path and reruns it after the documentation updates.

Validated acceptance evidence for `v0.1.14`:

- Full validation run (2026-03-14):
  - `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1 -OutputDir artifacts\unit-6-bolt-6-2`
  - Result:
    - `vector tests passed`
    - `models lifecycle tests passed`
    - `persistence contract tests passed`
    - `unit 5 defect regressions passed`
    - `app smoke tests passed`
- Local rerun note (2026-03-14):
  - Result: the prior `artifacts/unit-6-bolt-6-1/grade-guard.exe` artifact could not be overwritten in this workspace, so the same validation script was rerun against `artifacts/unit-6-bolt-6-2/` instead of changing the validated workflow itself.
- Documentation alignment check (2026-03-14):
  - Result: `README.md`, `CHANGELOG.md`, `REQUIREMENTS.md`, and the new `docs/` notes now point to real files, real commands, and the current artifact inventory, including the fresh `artifacts/unit-6-bolt-6-2/` rerun output directory.

For the full build-and-smoke breakdown, see `docs/unit-6-bolt-6-1-build-validation.md`.
For the release-readiness checklist and remaining blockers, see `docs/unit-6-bolt-6-2-release-artifacts.md`.

## Copy-Paste Commands

### Read this release note

```powershell
Get-Content docs/version-0-1-14-docs.md
```

### Re-run the full validation workflow

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1
```

### Read the Unit 6.2 bolt note

```powershell
Get-Content docs/unit-6-bolt-6-2-release-artifacts.md
```

## Pitfalls + Debugging

### 1) Do not mistake `v0.1.14` for a runtime feature release

- The code behavior validated in Unit 6.1 remains the same.
- This release publishes the missing release-artifact closure and requirements alignment around that state.

### 2) Do not silently mark Unit 6 fully approved

- Bolt 6.2 is complete.
- The separate human-validation line is still intentionally open.

### 3) Do not delete generated artifacts automatically

- The repository task rules require manual cleanup by the human operator.
- Use the latest `CHANGELOG.md` `For Deletion` section as the cleanup list.

### 4) Do not skip the live Windows console-host review

- The automated acceptance run is strong enough to prove the modular orchestration path.
- It is not a substitute for validating redraw, fullscreen, and raw-key behavior under a real host.

## Immediate Next Actions

1. Manually delete the generated binaries and CSV/runtime artifacts listed in `CHANGELOG.md` if they are not meant to be committed.
2. Run the live Windows console-host walkthrough that still remains open after Unit 6.
3. Explicitly approve or reject the Unit 6 release-readiness criteria and documentation scope.
4. Return to controller-side numeric validation and broader workflow coverage after the release sign-off decision.

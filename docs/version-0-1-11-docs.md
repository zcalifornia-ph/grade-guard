# Version 0.1.11 Notes

## Title
Unit 5 Defect Baseline, Prioritization, and Grade-Engine Evidence for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.11` repository state if it now contains:

- a root `README.md` updated to `v0.1.11`,
- a new top `v0.1.11` entry in `CHANGELOG.md`,
- this file at `docs/version-0-1-11-docs.md`,
- `docs/unit-5-bolt-5-1-defect-baseline.md`,
- `grade-guard/unit-tests/unit5_defect_baseline_test.c`,
- and a `REQUIREMENTS.md` Unit 5 / Bolt 5.1 section that now records dated baseline evidence while leaving the review gate open.

The project-governance documents stayed unchanged for a reason:

- `CONTRIBUTING.md` was not changed because this release adds a new focused regression command but does not alter the contributor workflow itself.
- `SECURITY.md` was not changed because this release documents baseline defects rather than changing vulnerability-reporting policy.
- `CODE_OF_CONDUCT.md` was not changed because this release does not alter community-governance expectations.

## One-Sentence Objective

Establish a reproducible, reviewable defect baseline for the extracted grade engine and shared selection flow so Bolt 5.2 can fix confirmed issues against executable evidence instead of intuition.

## Why This Version Matters

`v0.1.10` hardened CSV persistence and made persistence failures easier to reason about.
It did not yet answer the next functional question for the repository: does the extracted grade engine produce sane academic results across common and edge-case scenarios?

That left a real gap between extraction progress and release confidence:

- `grade_calc.c` already existed as a dedicated module, but it still had no focused regression harness of its own,
- the project had not yet published a defect register that separated must-fix correctness failures from secondary cleanup work,
- and the next release step risked mixing implementation and bug discovery in the same change.

This version matters because it closes that evidence gap without pretending the bugs are already fixed:

- the repository now includes one focused Unit 5 baseline harness that reproduces the current defects directly,
- the latest Bolt note records severity, reproduction steps, root-cause hypotheses, and proposed fix scope,
- `README.md` and `CHANGELOG.md` now describe the current Unit 5 state honestly,
- and `REQUIREMENTS.md` records that Bolt 5.1 has evidence for design, implementation, test, and docs while still leaving review open.

What this version does not do is also important:

- it does not land the Bolt 5.2 fixes,
- it does not close the Unit 5 review/human-approval gate,
- it does not add controller-level automation for the full live Windows UI,
- and it does not delete any generated binaries or CSV fixtures automatically.

## What Changed (Artifact Map)

- Root release metadata:
  - `README.md`
  - `CHANGELOG.md`
- Planning / requirements evidence:
  - `REQUIREMENTS.md`
- Unit 5 Bolt documentation:
  - `docs/unit-5-bolt-5-1-defect-baseline.md`
- Version-specific release note:
  - `docs/version-0-1-11-docs.md`
- Unit 5 focused baseline harness:
  - `grade-guard/unit-tests/unit5_defect_baseline_test.c`

## Guided Walkthrough

## 1) Unit 5 Now Starts with Evidence Instead of Direct Fixes

The central design choice in this version is procedural, not just technical.
Bolt 5.1 was treated as a baseline-and-prioritization step, which means the repository now captures the current defect surface before code behavior changes in Bolt 5.2.

That matters because it gives the project a stable before-and-after contract:

- baseline state is documented,
- broken behavior is reproducible,
- and later fixes can be validated against the same scenarios.

## 2) The New Harness Proves Five Concrete Defect Classes

`grade-guard/unit-tests/unit5_defect_baseline_test.c` now reproduces the following baseline behaviors:

- zero-score activities are ignored by weighted-average aggregation,
- lab-course lecture/lab combination uses `50.0f` scaling instead of a 50 percent share,
- scores above the declared activity total are accepted and can inflate results beyond 100 percent,
- empty profiles render a displayed predicted GWA of `5.00` instead of an explicit no-data state,
- and the shared `ui_selection_handler()` ignores `Esc` instead of surfacing a cancel path.

These are intentionally baseline assertions.
They prove the defects exist on the current code, not that the repository already contains the fixes.

## 3) The Defect Register Now Separates Blockers from Recommendations

`docs/unit-5-bolt-5-1-defect-baseline.md` does more than list bugs.
It defines:

- the severity rubric,
- the required defect-register fields,
- the confirmed defect IDs,
- the root-cause hypothesis for each defect,
- the proposed Bolt 5.2 release scope,
- and the expected regression follow-up after fixes land.

The current proposed must-fix set for the first modular release is:

- zero-score handling,
- lab-course weighting,
- zero-course rendering,
- and shared selection cancel behavior.

Over-total score validation is documented as confirmed and recommended, but still slightly more design-sensitive because it crosses controller validation and grade-engine enforcement.

## 4) Root Documentation Now Matches the Real Repository State

This version also updates the top-level documentation so the repository no longer looks like persistence is the latest functional milestone.

`README.md` now:

- reports `v0.1.11`,
- points readers to the Unit 5 defect-baseline note,
- documents the new baseline regression command,
- and explains that Unit 5 has confirmed but unfixed correctness and UX issues.

`CHANGELOG.md` now:

- records the Unit 5 baseline harness and docs,
- notes the `REQUIREMENTS.md` update,
- and marks all currently visible local build artifacts and generated fixtures under `### For Deletion` instead of silently leaving them undocumented.

## 5) Manual Cleanup Still Matters Before a Human Commit

As with prior task executions in this repository, generated binaries and CSV outputs were intentionally left in place.
This version therefore continues the repository convention of documenting cleanup candidates rather than deleting them automatically.

If the current workspace is going to be committed manually, the latest `CHANGELOG.md` entry should be reviewed first so the generated `.exe` outputs and persistence fixtures do not get committed by accident.

## Validation Evidence

- Acceptance build (2026-03-14):
  - `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-5-1-main.exe`
- Unit 5 defect baseline compile (2026-03-14):
  - `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/unit5_defect_baseline_test.c grade-guard/source/grade_calc.c grade-guard/source/models.c grade-guard/source/vector.c grade-guard/source/ui_console.c -o grade-guard/unit-tests/unit5_defect_baseline_test.exe`
- Unit 5 defect baseline run (2026-03-14):
  - `.\grade-guard\unit-tests\unit5_defect_baseline_test.exe`
  - Result: `unit 5 defect baseline reproductions passed`
- Vector regression run (2026-03-14):
  - `.\grade-guard\tests\vector_test.exe`
  - Result: `vector tests passed`
- Models regression run (2026-03-14):
  - `.\grade-guard\unit-tests\models_lifecycle_test.exe`
  - Result: `models lifecycle tests passed`
- Persistence regression run (2026-03-14):
  - `.\grade-guard\unit-tests\persistence_contract_test.exe`
  - Result: `persistence contract tests passed`

## Copy-Paste Commands

### Read this release note

```powershell
Get-Content docs/version-0-1-11-docs.md
```

### Rebuild the runtime acceptance target

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-5-1-main.exe
```

### Re-run the Unit 5 defect baseline harness

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/unit5_defect_baseline_test.c grade-guard/source/grade_calc.c grade-guard/source/models.c grade-guard/source/vector.c grade-guard/source/ui_console.c -o grade-guard/unit-tests/unit5_defect_baseline_test.exe
.\grade-guard\unit-tests\unit5_defect_baseline_test.exe
```

## Pitfalls + Debugging

### 1) Do not read the Unit 5 harness as a post-fix regression yet

- The harness currently asserts the baseline broken behavior so the defect list is proven.
- After Bolt 5.2, some of these assertions should flip to the correct expected behavior.

### 2) Do not assume Bolt 5.1 is fully closed just because most subtasks are checked

- `REQUIREMENTS.md` intentionally leaves the Bolt 5.1 review checkbox open.
- The first-release fix set still needs explicit approval.

### 3) Do not treat the `Esc` defect as fully validated from the harness alone

- The harness proves the shared selection-handler contract.
- A live Windows console pass is still needed to confirm the user-facing workflow impact across real menus.

### 4) Do not forget the generated artifacts before a manual commit

- This task intentionally left `.exe` outputs, generated CSV fixtures, and the runtime output directory untouched.
- Review the latest `CHANGELOG.md` `For Deletion` section before committing.

## Immediate Next Actions

1. Approve the Bolt 5.2 must-fix set recorded in `docs/unit-5-bolt-5-1-defect-baseline.md`.
2. Implement the Unit 5.2 fixes for grade correctness, cancel behavior, and any selected input-validation changes.
3. Convert the current baseline repros into post-fix regression expectations once the fixes land.
4. Manually delete the generated binaries and persistence fixtures listed in `CHANGELOG.md` if they are not meant to be committed.

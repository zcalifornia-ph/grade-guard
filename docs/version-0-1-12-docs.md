# Version 0.1.12 Notes

## Title
Unit 5 Defect Fixes, Regression Conversion, and Release Trail for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.12` repository state if it now contains:

- a root `README.md` updated to `v0.1.12`,
- a new top `v0.1.12` entry in `CHANGELOG.md`,
- this file at `docs/version-0-1-12-docs.md`,
- `docs/unit-5-bolt-5-2-defect-fixes.md`,
- a `grade-guard/header/grade_calc.h` declaration for `calculate_predicted_gwa()`,
- a `grade-guard/header/ui_console.h` declaration for `UiSelectionStatus`,
- and `grade-guard/unit-tests/unit5_defect_baseline_test.c` printing `unit 5 defect regressions passed` instead of the old baseline-reproduction message.

The project-governance documents stayed unchanged for a reason:

- `CONTRIBUTING.md` was not changed because this release adds one more targeted regression expectation but does not alter the contributor workflow itself.
- `SECURITY.md` was not changed because this release fixes application behavior rather than changing vulnerability-reporting policy.
- `CODE_OF_CONDUCT.md` was not changed because this release does not alter community-governance expectations.

## One-Sentence Objective

Land the smallest safe code and documentation changes needed to close the confirmed Unit 5 grade-engine and selection-flow defects, then convert the old baseline harness into post-fix regression evidence.

## Why This Version Matters

`v0.1.11` proved that Unit 5 had real, reproducible correctness and UX defects.
That was the right prerequisite, but it still left the repository in an awkward state:

- the extracted grade engine existed but still produced incorrect academic results for common scenarios,
- empty profiles still looked like computed failures instead of explicit no-data cases,
- the shared selection boundary still swallowed `Esc`,
- and the Unit 5 harness was still asserting broken behavior because it was acting as a baseline register instead of a regression suite.

This version matters because it closes that loop without widening the change more than necessary:

- the grade engine now fixes the confirmed calculation defects in one place,
- the profile header can distinguish no-data from a legitimate failing grade,
- the shared selection API now exposes a named cancel status,
- the touched controller callers now treat cancel as a real back-out path,
- and the old baseline harness now acts as a focused regression suite for the fixed behavior.

What this version does not do is also important:

- it does not add full controller-side numeric input rejection yet,
- it does not replace the need for a live Windows console walkthrough,
- it does not expand into a full end-to-end controller automation layer,
- and it does not delete generated binaries or CSV fixtures automatically.

## What Changed (Artifact Map)

- Root release metadata:
  - `README.md`
  - `CHANGELOG.md`
- Version-specific release note:
  - `docs/version-0-1-12-docs.md`
- Unit 5 Bolt documentation:
  - `docs/unit-5-bolt-5-2-defect-fixes.md`
- Grade engine API and implementation:
  - `grade-guard/header/grade_calc.h`
  - `grade-guard/source/grade_calc.c`
- Shared selection API and implementation:
  - `grade-guard/header/ui_console.h`
  - `grade-guard/source/ui_console.c`
- Profile display and cancel-aware controller call sites:
  - `grade-guard/source/profile_controller.c`
- Unit 5 focused regression harness:
  - `grade-guard/unit-tests/unit5_defect_baseline_test.c`

## Guided Walkthrough

## 1) Grade Aggregation Now Fixes the Real Unit 5 Math Bugs

The biggest technical change is in `grade-guard/source/grade_calc.c`.
Instead of duplicating lecture and lab aggregation logic with the same bug in both branches, the code now uses helper functions that make the intended rules explicit.

The effective behavior changes are:

- zero-score activities still count toward the denominator,
- negative scores clamp to zero,
- scores above the declared total clamp down to the declared total,
- lab-course lecture and lab percentages combine as actual 50/50 shares,
- and weighted-average calculations still stay isolated in the grade-engine module rather than spreading validation heuristics across the controller code.

That means GG-DEF-01, GG-DEF-02, and GG-DEF-03 now close from one aggregation boundary instead of multiple ad hoc patches.

## 2) Predicted GWA Rendering Can Now Tell the Difference Between No Data and Failure

The previous profile-header path always converted the weighted average directly into GWA text.
That made empty profiles look like a computed `5.00`, which is misleading because no coursework exists yet.

This version adds `calculate_predicted_gwa()` as a display-oriented helper.
It returns a boolean success signal plus an output value, which lets the UI do the right thing:

- no positive-unit graded coursework -> `Predicted GWA: N/A`,
- valid coursework with all-zero earned scores -> `Predicted GWA: 5.00`.

That distinction matters because it avoids solving the empty-profile bug by accidentally hiding legitimate failing outcomes.

## 3) `Esc` Is Now a Real Shared Selection Contract

The shared selection code now defines named `UiSelectionStatus` values in `grade-guard/header/ui_console.h`.
That replaces the earlier magic-number-only contract and gives the shared UI layer an explicit cancel path.

`ui_selection()` and `ui_selection_array()` now return `-1` on cancel, and the touched controller call sites already structured around negative return checks now use that path consistently.
The special case that needed explicit adjustment was the component selector, which previously would have treated a cancelled selection as lecture by default.

This closes GG-DEF-05 at the shared boundary instead of trying to special-case each menu separately.

## 4) The Unit 5 Harness Is No Longer a Baseline Repro Harness

`grade-guard/unit-tests/unit5_defect_baseline_test.c` intentionally keeps the old file name for continuity, but its job is different now.
It no longer proves that the bugs exist.
It now proves that the intended fixes keep working.

The regression suite covers:

- zero-score inclusion,
- normalized lab-course weighting,
- bounded over-total scores,
- zero-course no-data behavior,
- zero-unit no-data behavior,
- valid all-zero-coursework failing-GWA behavior,
- and shared `Esc` cancellation status.

That turns the Unit 5 evidence trail into a real before/after pair:

- `docs/unit-5-bolt-5-1-defect-baseline.md` explains the broken baseline,
- `docs/unit-5-bolt-5-2-defect-fixes.md` explains the fix scope and outcomes,
- and the harness now enforces the final behavior.

## 5) The Root Documentation Now Matches the Fixed State

This release also matters at the repository level because the top-level docs were stale after the code fix landed.

`README.md` now:

- reports `v0.1.12`,
- describes Unit 5 as fixed and regression-backed rather than baseline-only,
- updates the Unit 5 test command description from baseline reproduction to defect regression,
- and points readers to the new version note plus the Bolt 5.2 fix note.

`CHANGELOG.md` now:

- records the Bolt 5.2 code changes,
- records the new version note,
- records the new Unit 5 Bolt 5.2 doc,
- and keeps the manual cleanup trail explicit under `### For Deletion`.

## Validation Evidence

- Acceptance build (2026-03-14):
  - `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-5-2-main.exe`
- Unit 5 defect regression compile (2026-03-14):
  - `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/unit5_defect_baseline_test.c grade-guard/source/grade_calc.c grade-guard/source/models.c grade-guard/source/vector.c grade-guard/source/ui_console.c -o grade-guard/unit-tests/unit5_defect_baseline_test.exe`
- Unit 5 defect regression run (2026-03-14):
  - `./grade-guard/unit-tests/unit5_defect_baseline_test.exe`
  - Result: `unit 5 defect regressions passed`
- Vector regression run (2026-03-14):
  - `./grade-guard/tests/vector_test.exe`
  - Result: `vector tests passed`
- Models regression run (2026-03-14):
  - `./grade-guard/unit-tests/models_lifecycle_test.exe`
  - Result: `models lifecycle tests passed`
- Persistence regression run (2026-03-14):
  - `./grade-guard/unit-tests/persistence_contract_test.exe`
  - Result: `persistence contract tests passed`

## Copy-Paste Commands

### Read this release note

```powershell
Get-Content docs/version-0-1-12-docs.md
```

### Rebuild the runtime acceptance target

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-5-2-main.exe
```

### Re-run the Unit 5 defect regression harness

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/unit5_defect_baseline_test.c grade-guard/source/grade_calc.c grade-guard/source/models.c grade-guard/source/vector.c grade-guard/source/ui_console.c -o grade-guard/unit-tests/unit5_defect_baseline_test.exe
.\grade-guard\unit-tests\unit5_defect_baseline_test.exe
```

## Pitfalls + Debugging

### 1) Do not confuse no-data with a real zero-grade result

- Empty or zero-unit profiles should now return no data.
- A real graded profile with all zero scores should still render `5.00`.

### 2) Do not assume controller-side validation is already solved

- The grade engine now bounds malformed over-total scores so outputs remain sane.
- The text-input controller still accepts free-form numeric input, so user-facing rejection rules are still a follow-up item.

### 3) Do not treat the `Esc` fix as a substitute for a live Windows walkthrough

- The shared selection handler and the touched call sites are now regression-backed.
- Real console-host behavior still needs a manual pass across actual menu flows.

### 4) Do not forget the generated artifacts before a manual commit

- This task intentionally left `.exe` outputs, generated CSV fixtures, and runtime output directories untouched.
- Review the latest `CHANGELOG.md` `For Deletion` section before committing.

## Immediate Next Actions

1. Manually delete the generated binaries and CSV artifacts listed in `CHANGELOG.md` if they are not meant to be committed.
2. Run a live Windows console walkthrough for the fixed Unit 5 cancel/no-data flows.
3. Decide whether the next follow-up should add controller-side numeric validation or start the Unit 6 release-readiness pass.
4. Keep the Unit 5 regression harness green while future controller and release-workflow changes land.


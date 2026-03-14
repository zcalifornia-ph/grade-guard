# Unit 5 Bolt 5.2: Fix Grading and Stability Defects

Objective: close the approved Unit 5 defect scope with the smallest safe code changes, convert the baseline evidence into fixed-behavior regressions, and leave a traceable record of which defect IDs were resolved versus deferred.

## Executable Contract

Goal / deliverable

- Fix the confirmed grade-engine correctness defects without reopening the broader refactor surface.
- Surface a no-data state for profiles that do not yet have any positive-unit graded coursework.
- Propagate `Esc` as an explicit cancel result through the shared selection boundary so controller flows can back out cleanly.
- Resolve malformed over-total score inflation safely inside the grade engine if that can be done without destabilizing controller input flows.

Bolt checklist

- Design: identify the narrowest safe fix points for grade aggregation, predicted-GWA rendering, and shared selection handling.
- Implement: patch the affected modules and keep the controller impact limited to callers that already support negative cancel returns.
- Test: replace the Unit 5 baseline repro expectations with post-fix regression checks, including zero-score, zero-course, zero-unit, malformed over-total, and `Esc` cancel coverage.
- Docs: record final defect outcomes, regression coverage, and any remaining manual-review risk.
- Review: verify the patched behavior against the approved defect IDs and confirm whether GG-DEF-03 stayed deferred or landed safely.

Acceptance criteria interpreted for this Bolt

- Zero-score activities count toward weighted averages instead of being skipped.
- Lab-course lecture and lab components stay inside a normal 0.0 to 1.0 course-percentage range.
- Over-total scores no longer inflate calculated percentages beyond 100 percent.
- Profiles with no positive-unit graded coursework display a no-data state instead of a fabricated `5.00` GWA, while valid all-zero coursework still renders as `5.00`.
- `ui_selection_handler()` returns an explicit cancel status on `Esc`, and the controller selection screens touched by this Bolt treat that as a real back-out path.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Grade engine API and implementation: `grade-guard/header/grade_calc.h` and `grade-guard/source/grade_calc.c`
- Shared selection API and implementation: `grade-guard/header/ui_console.h` and `grade-guard/source/ui_console.c`
- Profile display and controller cancel handling: `grade-guard/source/profile_controller.c`
- Regression coverage: `grade-guard/unit-tests/unit5_defect_baseline_test.c`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because the fixes still need to build cleanly under the project warning flags.
- NFR-05 matters because the regression harness continues to allocate nested course, parameter, and activity state and must tear it down cleanly.
- NFR-06 matters because the `Esc` fix changes a shared keyboard-navigation contract that multiple controller screens depend on.
- The main brownfield risk was over-fixing the controller layer: GG-DEF-03 was only accepted if it could be contained safely without widening the text-input parsing surface.

## Context Summary

- `calculate_weighted_average()` previously duplicated lecture and lab aggregation logic, which made the same score-filter bug exist in both branches.
- The prior aggregation path only counted activities when both `total_score > 0` and `score > 0`, which skipped valid zero scores and accepted over-total scores unchanged.
- The lab-course combiner multiplied lecture and lab percentages by `50.0f` each, so any mixed lab course could explode into impossible percentages before GWA clamping hid the defect.
- `ui_show_profile_header()` was the only public predicted-GWA display path, so the no-data fix needed to land behind a reusable helper instead of duplicating UI heuristics in tests.
- Shared selection already had many controller callers that treat negative selection indexes as cancel, so the safest `Esc` fix was to return a real cancel signal from the shared helper and let those callers reuse their existing abort path.
- `controller_select_component()` was the one touched caller that would have treated a negative selection as lecture by default, so it needed an explicit cancel guard.

## ADR Bullets

- Fix GG-DEF-01 and GG-DEF-03 in one place by clamping activity scores during grade aggregation instead of expanding controller-side parsing rules.
- Add `calculate_predicted_gwa()` as the public display-oriented helper so the UI can distinguish no-data from a legitimate failing grade.
- Keep the existing `unit5_defect_baseline_test.c` file path for continuity, but update its assertions to fixed-behavior regressions for Bolt 5.2.
- Preserve the generic `ui_selection_handler()` API as `int`, but formalize named cancel/confirm statuses in `ui_console.h` so callers and tests stop depending on raw magic numbers.

## Final Defect Outcomes

| ID | Outcome | Final behavior | Regression coverage |
| --- | --- | --- | --- |
| GG-DEF-01 | Resolved | Zero-score activities now contribute their total score to the denominator and clamp their earned score at zero. | `test_zero_score_activity_counts_in_weighted_average` |
| GG-DEF-02 | Resolved | Lab-course lecture and lab percentages now combine as 50/50 shares with `0.5f` weights instead of `50.0f`. | `test_lab_course_scaling_stays_normalized` |
| GG-DEF-03 | Resolved safely in Bolt 5.2 | Over-total scores are bounded inside the grade engine, so malformed stored or typed scores cannot inflate output above 100 percent. | `test_over_total_scores_are_bounded` |
| GG-DEF-04 | Resolved | Profiles with no positive-unit graded coursework now report no data, while valid all-zero coursework still renders a failing `5.00` predicted GWA. | `test_zero_course_profiles_render_as_no_data`, `test_zero_unit_profiles_render_as_no_data`, `test_valid_zero_scores_still_render_as_failing_gwa` |
| GG-DEF-05 | Resolved | `Esc` now returns `UI_SELECTION_STATUS_CANCEL`, and the touched controller menu/selection paths back out cleanly instead of swallowing the key. | `test_escape_key_returns_cancel_status` plus controller code-path review |

## Validation Evidence

- Acceptance build (2026-03-14): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-5-2-main.exe` succeeded.
- Unit 5 regression compile (2026-03-14): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/unit5_defect_baseline_test.c grade-guard/source/grade_calc.c grade-guard/source/models.c grade-guard/source/vector.c grade-guard/source/ui_console.c -o grade-guard/unit-tests/unit5_defect_baseline_test.exe` succeeded.
- Unit 5 regression run (2026-03-14): `./grade-guard/unit-tests/unit5_defect_baseline_test.exe` exited `0` and printed `unit 5 defect regressions passed`.
- Vector regression compile and run (2026-03-14): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe` succeeded, and `./grade-guard/tests/vector_test.exe` exited `0` and printed `vector tests passed`.
- Models regression compile and run (2026-03-14): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe` succeeded, and `./grade-guard/unit-tests/models_lifecycle_test.exe` exited `0` and printed `models lifecycle tests passed`.
- Persistence regression compile and run (2026-03-14): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/persistence_contract_test.c grade-guard/source/persistence.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/persistence_contract_test.exe` succeeded, and `./grade-guard/unit-tests/persistence_contract_test.exe` exited `0` and printed `persistence contract tests passed`.

## Review Status

- The approved first-release scope is fully closed: GG-DEF-01 through GG-DEF-05 now have code changes and regression evidence.
- GG-DEF-03 no longer needs deferral because the fix stayed inside the grade-engine aggregation path and did not require controller parsing changes.
- Residual manual risk remains limited to a live Windows-console walkthrough of one or more `Esc`-to-cancel controller flows, because this Bolt validated the shared handler contract and the wired callers but did not automate full console interaction.

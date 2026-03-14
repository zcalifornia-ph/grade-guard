# Unit 5 Bolt 5.1: Defect Baseline and Prioritization

Objective: establish a reproducible defect register for the extracted grade engine and adjacent selection flow before Bolt 5.2 changes behavior, so the first modular release is driven by confirmed defects instead of ad hoc fixes.

## Executable Contract

Goal / deliverable

- Define one defect triage format that records severity, reproduction, root-cause hypothesis, release-scope decision, and regression coverage.
- Reproduce the currently confirmed Unit 5 defects on the baseline code before any fixes land.
- Separate high-severity correctness failures from medium-severity UX and malformed-input failures.
- Leave a traceable register that Bolt 5.2 can update as fixes are implemented and validated.

Bolt checklist

- Design: define the defect triage format, severity levels, and reproduction-template requirements.
- Implement: reproduce and document defects found through code review and targeted baseline harnesses.
- Test: confirm each documented defect is reproducible on the baseline before changing code.
- Docs: capture root-cause hypotheses and prioritization rationale.
- Review: identify the minimum must-fix set for the first modular release and leave the human approval gate open.

Acceptance criteria interpreted for this Bolt

- A defect register exists with stable IDs, severity, baseline reproduction, fix status, and planned regression coverage.
- Confirmed high-severity defect classes are explicitly called out as release blockers for Bolt 5.2.
- Grade-engine defects are backed by executable baseline evidence instead of only narrative suspicion.
- The remaining live-console validation gap is explicit rather than silently assumed complete.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Grade engine: `grade-guard/header/grade_calc.h` and `grade-guard/source/grade_calc.c`
- Domain lifecycle and vectors used by the repro harness: `grade-guard/header/models.h`, `grade-guard/source/models.c`, `grade-guard/header/vector.h`, and `grade-guard/source/vector.c`
- Menu flow boundary: `grade-guard/header/ui_console.h` and `grade-guard/source/ui_console.c`
- Verification harness: `grade-guard/unit-tests/unit5_defect_baseline_test.c`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because the baseline evidence has to compile cleanly under the project warning flags.
- NFR-05 matters because the new baseline harness allocates nested course, parameter, and activity structures that must still tear down cleanly.
- NFR-06 matters because Unit 5 includes a confirmed selection-flow usability defect and because a full live console walkthrough is still pending.
- The main brownfield risk is false confidence: `grade_calc.c` already exists as a separate module, but there was no dedicated automated coverage proving that its current outputs are academically sane.

## Context Summary

- The grade engine is already isolated into `grade_calc.c`, so Unit 5 begins from a code-extraction baseline rather than from monolithic logic still trapped in `main.c`.
- No dedicated regression test existed for grade calculations before this Bolt, which meant calculation bugs could persist even while vector, model, and persistence suites passed.
- `calculate_weighted_average()` mixes grade aggregation, validation assumptions, and course-level weighting in one routine, making small comparison mistakes affect user-visible GWA output directly.
- `profile_controller.c` still accepts free-form numeric input for scores, totals, weights, units, and goals without a validation layer between the text UI and the domain model.
- `ui_show_profile_header()` converts the raw weighted-average output into a displayed GWA on every render, so edge-case calculation behavior immediately reaches the UI.
- `ui_selection_handler()` is the shared keyboard navigation boundary for the controller menus, so missing cancel behavior there propagates through every selection screen.
- Existing vector, model, and persistence suites still pass on this baseline, which narrows Unit 5 focus to the grade-engine and adjacent UX/validation surfaces instead of reopening previously hardened areas.

## ADR Bullets

- Record confirmed Unit 5 defects under stable IDs now, even before fixes, so Bolt 5.2 can close them against the same evidence source.
- Use one focused baseline harness to reproduce grade-engine and shared selection defects without modifying runtime behavior yet.
- Treat any defect that can silently produce an impossible or inflated GWA as a release blocker for the first modular release.
- Keep the live console review gate open because the baseline harness proves the shared selection-handler contract but does not replace a human walkthrough of the full Windows UI.

## Defect Triage Format

Severity levels

- High: silently corrupts GWA or goal-progress results for normal academic input, or makes a supported workflow unsafe to ship.
- Medium: produces incorrect output only on malformed or edge-case input, or breaks keyboard usability without data loss.
- Low: cosmetic or low-risk issues that do not materially change calculations or block a supported workflow.

Required fields per defect

- ID
- Severity
- Class
- Baseline reproduction and evidence source
- Observed behavior
- Expected behavior
- Root-cause hypothesis
- Bolt 5.2 release-scope decision
- Planned regression coverage
- Current status

## Defect Register

| ID | Severity | Class | Baseline reproduction and evidence | Observed behavior | Expected behavior | Root-cause hypothesis | Bolt 5.2 release scope | Planned regression coverage | Status |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| GG-DEF-01 | High | Calculation correctness | `unit5_defect_baseline_test.exe` case `test_zero_score_activity_is_ignored`; points at `grade_calc.c` lines 330-333 and 363-366 | Activities scored `0 / total` are excluded from both the numerator and denominator, so partial-course averages are inflated. | Zero-score activities should count toward the total attempt weight and lower the computed course percentage. | `calculate_weighted_average()` only accumulates an activity when both `total_score > 0` and `score > 0`, which filters out legitimate zero scores. | Must fix before the first modular release. | Replace the baseline repro with a regression asserting that `0/100` plus `100/100` yields `0.5`, not `1.0`. | Confirmed on baseline |
| GG-DEF-02 | High | Calculation correctness | `unit5_defect_baseline_test.exe` case `test_lab_course_scaling_explodes_course_percentage`; points at `grade_calc.c` line 376 | A lab course with 50 percent lecture and 50 percent lab performance produces a raw course percentage of `50.0`, which then clamps to a perfect `1.00` GWA. | A balanced 50 percent lecture and 50 percent lab course should stay inside the normal 0.0 to 1.0 percentage range. | The lecture and lab component percentages are multiplied by `50.0f` instead of `0.5f` when combining a lab course. | Must fix before the first modular release. | Add regression coverage for lecture-only, lab-only, and mixed lecture-plus-lab profiles with hand-checked expected percentages. | Confirmed on baseline |
| GG-DEF-03 | Medium | Malformed-input validation | `unit5_defect_baseline_test.exe` case `test_over_total_scores_are_accepted`; rooted in `profile_controller.c` lines 657-663 plus `grade_calc.c` aggregation logic | Scores greater than the declared activity total are accepted and can inflate a course above 100 percent, again collapsing the displayed GWA to `1.00`. | Activity scores should be rejected or bounded when `score > total_score`. | The controller stores free-form numeric input directly and the grade engine never validates `score <= total_score`. | Include in the first modular release if the release is intended to tolerate malformed local input safely. | Add controller-or-grade regression coverage for rejected or bounded over-total scores. | Confirmed on baseline |
| GG-DEF-04 | Medium | Edge-case calculation | `unit5_defect_baseline_test.exe` case `test_zero_course_profiles_render_as_failing_gwa`; points at `profile_controller.c` line 1162 | An empty profile displays a predicted GWA of `5.00`, which looks like a computed failing grade instead of a no-data state. | A profile with no gradable courses should display a neutral or explicit no-data state. | `ui_show_profile_header()` always pipes `calculate_weighted_average()` into `percentage_to_gwa()`, so the empty-profile `0.0` path becomes a displayed `5.0`. | Recommended for the first modular release because Bolt 5.2 already names the zero-course case as a required regression scenario. | Add explicit zero-course and zero-unit profile assertions around the public display calculation path. | Confirmed on baseline |
| GG-DEF-05 | Medium | Selection flow / UX | `unit5_defect_baseline_test.exe` case `test_escape_key_is_ignored_by_selection_handler`; points at `ui_console.c` lines 595-642 | Pressing `Esc` in shared selection handling does nothing, so menu flows cannot offer a real cancel or back-out path once selection mode starts. | Shared selection handling should expose an explicit cancel signal that controller screens can treat as back or abort. | `ui_selection_handler()` only returns a special value for `Enter`; `Esc` falls through as a no-op. | Strong candidate for the first modular release because it affects every controller selection screen and NFR-06 keyboard usability. | Add a small non-interactive regression around `ui_selection_handler()` plus a live console walkthrough for at least one course-selection path. | Confirmed on baseline |

## Prioritized First-Release Scope

Required by this baseline

- GG-DEF-01 because zero scores are common and the current behavior silently inflates grades.
- GG-DEF-02 because any lab course can currently collapse to a perfect `1.00` displayed GWA.
- GG-DEF-04 because Bolt 5.2 already calls out zero-course scenarios as mandatory regression cases.
- GG-DEF-05 because the shared selection boundary still lacks a cancel path, which violates the keyboard-usability expectation.

Recommended if time allows inside Bolt 5.2

- GG-DEF-03 because malformed local input should not be able to manufacture impossible course percentages, but this may require a controller-level validation decision instead of a grade-engine-only patch.

## Validation Evidence

- Acceptance build (2026-03-14): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-5-1-main.exe` succeeded.
- Unit 5 defect baseline compile (2026-03-14): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/unit5_defect_baseline_test.c grade-guard/source/grade_calc.c grade-guard/source/models.c grade-guard/source/vector.c grade-guard/source/ui_console.c -o grade-guard/unit-tests/unit5_defect_baseline_test.exe` succeeded.
- Unit 5 defect baseline run (2026-03-14): `./grade-guard/unit-tests/unit5_defect_baseline_test.exe` exited `0` and printed `unit 5 defect baseline reproductions passed`.
- Vector regression run (2026-03-14): `./grade-guard/tests/vector_test.exe` exited `0` and printed `vector tests passed`.
- Models regression run (2026-03-14): `./grade-guard/unit-tests/models_lifecycle_test.exe` exited `0` and printed `models lifecycle tests passed`.
- Persistence regression run (2026-03-14): `./grade-guard/unit-tests/persistence_contract_test.exe` exited `0` and printed `persistence contract tests passed`.

## Review Status

- Unit 5 now has a defect register with reproducible baseline evidence and a proposed first-release fix set.
- A live Windows-console walkthrough is still needed to confirm the full user-facing impact of GG-DEF-05 beyond the shared selection handler.
- The Unit 5 human validation gate from `REQUIREMENTS.md` remains open: the defect list and Bolt 5.2 fix scope still need approval before Unit 5 itself should be considered reviewed.

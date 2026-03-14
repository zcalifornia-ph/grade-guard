# Unit 3 Bolt 3.2: Extract Workflow Controllers

Objective: move the profile, course, parameter, and activity workflows out of the legacy monolith, make `main.c` orchestration-only, and leave a traceable controller map for the remaining Unit 3 manual acceptance work.

## Executable Contract

Goal / deliverable

- Make `grade-guard/main.c` a thin entry point that delegates to `app_run()`.
- Move top-level menu orchestration into `grade-guard/source/app.c`.
- Move profile creation, profile selection, course operations, parameter operations, activity operations, profile settings, and grade viewing into `grade-guard/source/profile_controller.c`.
- Document the resulting user-flow map and the remaining live-console validation gap before Bolt 3.2 can be checked off.

Bolt checklist

- Design: identify coherent workflow functions for profile creation, login, course management, parameter management, and activity management.
- Implement: move those flows into controller-style modules and simplify `main.c` to orchestration calls.
- Test: run end-to-end menu scenarios for create/select/update/delete flows.
- Docs: add a user-flow map that matches the refactored code paths.
- Review: verify that the controllers are smaller, comprehensible, and free from accidental feature loss.

Acceptance criteria interpreted for this Bolt

- `grade-guard/main.c` should reduce to `main() -> app_run()`.
- The startup load loop, main menu, profile lookup dispatch, and save-on-exit flow should live in `app.c`.
- The profile-level menu logic should be decomposed into smaller controller helpers rather than one long `switch`/`goto` body.
- Existing menu options should still exist in the extracted controller path.
- Non-interactive build/regression evidence should stay green, but Bolt 3.2 should remain open until a real keyboard-driven acceptance run happens.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Prior context: `docs/unit-1-bolt-1-1-monolith-inventory.md` and `docs/unit-3-bolt-3-1-ui-console.md`
- Thin entry point: `grade-guard/main.c`
- App orchestrator API: `grade-guard/header/app.h`
- Profile/workflow API: `grade-guard/header/profile_controller.h`
- Runtime orchestrator: `grade-guard/source/app.c`
- Workflow controllers: `grade-guard/source/profile_controller.c`
- Supporting runtime modules now consumed by the extracted path: `grade-guard/source/grade_calc.c`, `grade-guard/source/persistence.c`, `grade-guard/source/ui_console.c`

Relevant NFRs and risks for this Bolt

- NFR-04 and NFR-09 matter because the point of this Bolt is reducing entry-point sprawl and clarifying dependencies.
- NFR-06 matters because failure paths in the extracted menus still need visible user feedback instead of silent fallthrough or undefined behavior.
- Live Windows console behavior is still the highest remaining risk because the menus use raw key input and screen redraw APIs that are not proven by compile-only checks.
- Persistence behavior remains a downstream risk because `app.c` now consumes `persistence.c` directly, but Unit 4 contract hardening has not happened yet.

## Context Summary

- Before this Bolt, `grade-guard/main.c` still owned startup, profile bootstrap, top-level menu dispatch, profile selection, profile creation, course operations, parameter operations, activity operations, profile settings, grade viewing, persistence helpers, and grade-calculation helpers.
- `grade-guard/source/app.c` and `grade-guard/source/profile_controller.c` existed only as scaffold placeholders from Unit 1.
- `docs/unit-1-bolt-1-1-monolith-inventory.md` already mapped `main()` to `app.c` and the interactive profile/course/activity flows to `profile_controller.c`.
- Bolt 3.1 had already isolated the reusable console primitives behind `grade-guard/source/ui_console.c`, which made a controller extraction feasible without reintroducing raw WinAPI logic into new files.
- The profile/course/activity path still relied on the scaffolded `grade_calc` and `persistence` interfaces, so the extraction required wiring those modules up instead of leaving them stubbed.
- Existing automated coverage still targets shared vector and lifecycle behavior only; no controller-level automated harness existed before this extraction.

## ADR Bullets

- Keep `main.c` as a single-purpose entry point that calls `app_run()`.
- Keep `app.c` responsible for startup/shutdown orchestration and top-level profile dispatch only.
- Keep `profile_controller.c` responsible for interactive profile/course/parameter/activity workflows and profile-specific rendering helpers.
- Keep `grade_calc.c` and `persistence.c` as supporting service modules used by the extracted runtime path, even though their deeper hardening work belongs to later Units.
- Prefer small controller helpers with single menu responsibilities over one long `switch` tree with `goto` labels.

## User-Flow Map

- Process entry:
  `grade-guard/main.c` calls `app_run()`.
- App startup:
  `app_run()` initializes the console, loads numbered CSV profiles through `persistence.c`, and presents the top-level main menu.
- Main menu:
  `New Profile` calls `create_new_profile()`.
  `Select Profile` calls `profile_select_by_student_number()` and then `ui_profile_login()`.
  `Exit` saves all loaded profiles through `persistence.c` and releases the in-memory profile vector.
- Profile creation:
  `create_new_profile()` captures student identity fields, calls `input_courses()`, then collects the grade goal before returning the completed `Student_Profile`.
- Profile menu:
  `ui_profile_login()` dispatches to `controller_run_activity_operations()`, `controller_run_course_operations()`, `controller_run_profile_settings()`, and `controller_view_grades()`.
- Activity operations:
  `controller_run_activity_operations()` routes to `controller_add_activity()`, `controller_add_score()`, and `controller_delete_activity()`.
- Course operations:
  `controller_run_course_operations()` routes to `input_courses()`, `controller_delete_course()`, and `controller_manage_course_parameters()`.
- Parameter operations:
  `controller_run_parameter_operations()` routes to `controller_add_parameter()`, `controller_modify_parameter()`, `controller_print_parameters()`, and `controller_delete_parameter()`.

## Validation Evidence

- Acceptance build (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-3-2-main.exe` succeeded.
- Vector regression compile and run (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe` succeeded, and `.\grade-guard\tests\vector_test.exe` printed `vector tests passed`.
- Lifecycle regression compile and run (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe` succeeded, and `.\grade-guard\unit-tests\models_lifecycle_test.exe` printed `models lifecycle tests passed`.
- Manual end-to-end menu scenarios (2026-03-13): not executed in this non-interactive session, so the Bolt remains open pending a live Windows console walkthrough.

## Review Status

- `grade-guard/main.c` is now a thin wrapper instead of a controller hotspot.
- `grade-guard/source/app.c` now owns the top-level application workflow that the Unit 1 partition map assigned to `app`.
- `grade-guard/source/profile_controller.c` now contains smaller controller helpers for add/delete/update flows instead of one large monolithic `ui_profile_login()` implementation.
- The extracted controller path reuses `ui_console` rather than re-embedding raw console primitives.
- Human validation is still required for the Unit 3 UI boundary and workflow decomposition, and live create/select/update/delete walkthroughs are still required before Bolt 3.2 can be checked off.

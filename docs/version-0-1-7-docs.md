# Version 0.1.7 Notes

## Title
Unit 3 / Bolt 3.2 Workflow Controller Extraction and Thin Entry-Point Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.7` repository state if it now contains:

- a thin `grade-guard/main.c` that calls `app_run()`,
- the live application orchestrator in `grade-guard/source/app.c`,
- the extracted workflow controllers in `grade-guard/source/profile_controller.c`,
- the runtime `grade_calc` and `persistence` implementations wired into the build,
- the Bolt 3.2 design/evidence note at `docs/unit-3-bolt-3-2-workflow-controllers.md`,
- a root `README.md` updated to `v0.1.7`,
- a new top entry in `CHANGELOG.md`,
- and this file at `docs/version-0-1-7-docs.md`.

The planning and governance state changed only where the repository context required it:

- `REQUIREMENTS.md` now records Bolt 3.2 implementation progress and non-interactive evidence, but the Bolt remains open because the live menu walkthrough was not executed here.
- `CONTRIBUTING.md` and `README.md` were updated because the normal application build now links `app.c`, `profile_controller.c`, `grade_calc.c`, and `persistence.c` in addition to the previously extracted modules.
- `SECURITY.md` and `CODE_OF_CONDUCT.md` were not changed because this version does not alter vulnerability reporting or community-governance policy.

## One-Sentence Objective

Move the remaining profile/course/activity workflows out of the monolith, reduce `main.c` to orchestration-only startup, and document the extracted controller map without claiming manual acceptance that did not happen.

## Why This Version Matters

`v0.1.6` established the reusable UI/platform boundary.
It did not yet remove the large application workflows from the monolith.

That still left `grade-guard/main.c` acting as:

- process entry point,
- startup orchestrator,
- profile repository bootstrapper,
- profile selector,
- profile/course/activity controller,
- grade-calculation owner,
- and persistence owner.

This version matters because it cuts through that remaining hotspot:

- `main.c` now delegates immediately to `app_run()`,
- `app.c` now owns startup, main-menu dispatch, profile selection dispatch, and shutdown save logic,
- `profile_controller.c` now owns the interactive profile/course/parameter/activity flows,
- and the runtime build now uses the intended `grade_calc` and `persistence` modules instead of leaving them scaffolded.

What this version does not do is equally important:

- it does not claim Bolt 3.2 is fully accepted,
- it does not close the Unit 3 human-validation gate,
- and it does not replace the live Windows console walkthrough that is still required for create/select/update/delete verification.

## What Changed (Artifact Map)

- Thin entry point:
  - `grade-guard/main.c`
- Runtime app orchestrator:
  - `grade-guard/header/app.h`
  - `grade-guard/source/app.c`
- Workflow controller API and implementation:
  - `grade-guard/header/profile_controller.h`
  - `grade-guard/source/profile_controller.c`
- Supporting runtime services now wired into the extracted path:
  - `grade-guard/header/grade_calc.h`
  - `grade-guard/source/grade_calc.c`
  - `grade-guard/header/persistence.h`
  - `grade-guard/source/persistence.c`
- Existing shared dependencies still used by the extracted path:
  - `grade-guard/source/ui_console.c`
  - `grade-guard/source/models.c`
  - `grade-guard/source/vector.c`
- Bolt-specific design and evidence note:
  - `docs/unit-3-bolt-3-2-workflow-controllers.md`
- Updated release and contributor docs:
  - `README.md`
  - `CHANGELOG.md`
  - `CONTRIBUTING.md`
  - `REQUIREMENTS.md`

## Guided Walkthrough

## 1) `main.c` Is Finally Thin

The long-term target defined in the Unit 1 partition map was:

```c
int main(void)
{
    return app_run();
}
```

`v0.1.7` reaches that shape.

That matters because the entry point no longer owns menu flow, persistence details, or profile/course/activity branches.

## 2) `app.c` Now Owns the Top-Level Lifecycle

The extracted `app_run()` path now handles:

- console startup,
- numbered-profile bootstrap,
- top-level main-menu dispatch,
- profile-selection dispatch into the controller layer,
- and save/release behavior on exit.

That is the right boundary for the future application shell.

## 3) `profile_controller.c` Owns the Interactive Workflow Tree

The old monolith had one giant `ui_profile_login()` hotspot plus several related helper flows.

This version breaks that into smaller controller helpers such as:

- profile selection,
- profile creation,
- course add/delete/manage-parameters,
- parameter add/modify/print/delete,
- activity add/score/delete,
- profile settings,
- and grade viewing.

The code is still keyboard-driven and still uses the shared `ui_console` primitives, but the workflow responsibilities now live behind a named controller module instead of one file-wide hotspot.

## 4) The Runtime Build No Longer Depends on Stubbed Grade/Persistence Modules

The controller extraction could not stay coherent while `grade_calc.c` and `persistence.c` remained placeholders.

This version wires those modules into the runtime build so that:

- profile headers compute through `grade_calc`,
- app startup/shutdown call `persistence`,
- and the extracted controller path compiles against the intended module graph.

That does not mean Unit 4 or Unit 5 are complete.
It means the runtime path now uses the planned interfaces instead of bypassing them.

## 5) The Docs Now Match the Runtime Shape

The standard build commands in `README.md` and `CONTRIBUTING.md` now link:

- `grade-guard/main.c`
- `grade-guard/source/app.c`
- `grade-guard/source/vector.c`
- `grade-guard/source/models.c`
- `grade-guard/source/grade_calc.c`
- `grade-guard/source/persistence.c`
- `grade-guard/source/ui_console.c`
- `grade-guard/source/profile_controller.c`

Without that update, contributors would keep using stale link commands that no longer describe the application.

## 6) The Acceptance Story Is Still Deliberately Honest

This version has strong non-interactive evidence:

- the full application build succeeded,
- the shared vector regression test still passed,
- the shared lifecycle regression test still passed.

But Bolt 3.2 requires more than that.
The remaining gap is a real Windows console session that verifies:

- profile creation,
- profile selection,
- course and parameter edits,
- activity add/score/delete flows,
- and clean return paths from each menu.

Because that manual acceptance run was not executed here, the Bolt remains open in `REQUIREMENTS.md`.

## Validation Evidence

- Acceptance build (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-3-2-main.exe` succeeded.
- Vector regression compile and run (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe` succeeded, and `.\grade-guard\tests\vector_test.exe` printed `vector tests passed`.
- Lifecycle regression compile and run (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe` succeeded, and `.\grade-guard\unit-tests\models_lifecycle_test.exe` printed `models lifecycle tests passed`.
- Manual end-to-end menu run (2026-03-13): not executed in this session, so Bolt 3.2 remains open.

## Copy-Paste Commands

### Read the Bolt 3.2 design/evidence note

```powershell
Get-Content docs/unit-3-bolt-3-2-workflow-controllers.md
```

### Rebuild the app with the extracted controller path

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-3-2-main.exe
```

### Re-run the focused shared regressions

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe
.\grade-guard\tests\vector_test.exe

gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe
.\grade-guard\unit-tests\models_lifecycle_test.exe
```

### Run the app for the pending manual workflow acceptance pass

```powershell
.\grade-guard\bolt-3-2-main.exe
```

## Pitfalls + Debugging

### 1) Unresolved `app_*`, `profile_*`, `save_*`, or `calculate_*` linker errors

- Cause: the build command is still using the old module list.
- Fix: use the updated full-app build command from `README.md` or `CONTRIBUTING.md`.

### 2) The menus compile but behave oddly in a live console

- Cause: Unit 3 still depends on host-specific Windows console behavior for raw key input and redraw.
- Fix: reproduce the issue in the target Windows console host and walk the exact create/select/update/delete paths manually.

### 3) It is tempting to mark Bolt 3.2 complete immediately

- Do not do that unless the live workflow walkthrough has actually been performed.
- The compile and shared regression evidence here is necessary, but it is not the full Bolt 3.2 acceptance run.

## Immediate Next Actions

1. Run the live Windows console walkthrough for the Bolt 3.2 create/select/update/delete scenarios.
2. If that passes, update the remaining Test and Review checkboxes in `REQUIREMENTS.md`.
3. Close the Unit 3 human-validation gate once the UI/platform and workflow boundaries are approved together.
4. Then move to Unit 4 persistence hardening with the extracted `app` and `profile_controller` boundaries as the new baseline.

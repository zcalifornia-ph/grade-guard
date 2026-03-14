# Version 0.1.6 Notes

## Title
Unit 3 / Bolt 3.1 Console Primitive Extraction and UI Boundary Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.6` repository state if it now contains:

- the extracted UI/platform header at `grade-guard/header/ui_console.h`,
- the live Windows console implementation at `grade-guard/source/ui_console.c`,
- a slimmer `grade-guard/main.c` that consumes `ui_console.h` instead of owning raw console helpers,
- the Bolt 3.1 design/evidence note at `docs/unit-3-bolt-3-1-ui-console.md`,
- a root `README.md` updated to `v0.1.6`,
- a new top entry in `CHANGELOG.md`,
- and this file at `docs/version-0-1-6-docs.md`.

The governance and planning baseline changed only where the repository context required it:

- `CONTRIBUTING.md` was updated because the standard app build now links `ui_console.c`, and UI-touching changes now explicitly require live Windows console verification.
- `REQUIREMENTS.md` was intentionally left unchanged because Bolt 3.1 still has a manual acceptance step that was not executed in this session.
- `SECURITY.md` and `CODE_OF_CONDUCT.md` were not changed because this version does not alter vulnerability reporting or community-governance policy.

## One-Sentence Objective

Move the reusable Windows console primitives into the shared `ui_console` module, document that boundary, and prepare `main.c` for later workflow-controller extraction without falsely claiming Bolt 3.1 is already fully accepted.

## Why This Version Matters

`v0.1.5` stabilized the shared data model and ownership rules.
It did not yet remove the low-level Windows console code from the monolith.

That gap still mattered because `main.c` was simultaneously acting as:

- entry point,
- workflow controller,
- persistence caller,
- grade calculator caller,
- and raw console implementation file.

This version matters because it reduces that sprawl in the next safest place.

In `v0.1.6`:

- the reusable console helpers now live behind one shared module,
- `main.c` no longer owns `windows.h` / `conio.h` UI primitives directly,
- build documentation now reflects the real link steps,
- and the repository has a written release trail for Unit 3 / Bolt 3.1.

What this version does not do is equally important:

- it does not yet extract the large profile/course/activity workflows into `profile_controller.c`,
- and it does not yet mark Bolt 3.1 complete in `REQUIREMENTS.md` because the manual Windows console acceptance run is still pending.

## Plan A / Plan B

### Plan A (Recommended): Review the UI Extraction in 20-30 Minutes

1. Read `docs/unit-3-bolt-3-1-ui-console.md`.
2. Open `grade-guard/header/ui_console.h` and `grade-guard/source/ui_console.c`.
3. Rebuild the app with `ui_console.c` linked in.
4. Re-run the focused vector and lifecycle regression binaries.
5. Perform a live manual Windows console pass for menu navigation, field editing, and screen redraw behavior.

### Plan B: Review Only the Release-Trail Delta

1. Read the top `v0.1.6` entry in `CHANGELOG.md`.
2. Open `README.md` and `CONTRIBUTING.md`.
3. Confirm that the build commands now link `ui_console.c` and that manual UI verification is called out explicitly.

Use Plan B if you only need the repository-level release/documentation delta before making your own commit.

## What Changed (Artifact Map)

- Extracted UI/platform API and implementation:
  - `grade-guard/header/ui_console.h`
  - `grade-guard/source/ui_console.c`
- Updated monolith caller to consume the shared UI API:
  - `grade-guard/main.c`
- Added Bolt-specific design and evidence notes:
  - `docs/unit-3-bolt-3-1-ui-console.md`
- Added detailed version notes:
  - `docs/version-0-1-6-docs.md`
- Updated release and contributor docs:
  - `README.md`
  - `CHANGELOG.md`
  - `CONTRIBUTING.md`
- Shared modules kept as validation context:
  - `grade-guard/source/vector.c`
  - `grade-guard/source/models.c`
  - `grade-guard/tests/vector_test.c`
  - `grade-guard/unit-tests/models_lifecycle_test.c`
- Intentionally unchanged planning/governance files:
  - `REQUIREMENTS.md`
  - `SECURITY.md`
  - `CODE_OF_CONDUCT.md`
- Local artifacts identified for manual deletion before commit:
  - `grade-guard/build-smoke/`
  - `grade-guard/grade-guard-warning-build.exe`
  - `grade-guard/bolt-2-1-main.exe`
  - `grade-guard/bolt-2-2-main.exe`
  - `grade-guard/bolt-3-1-main.exe`
  - `grade-guard/tests/vector_test.exe`
  - `grade-guard/unit-tests/models_lifecycle_test.exe`

## Guided Walkthrough (What Landed)

## 1) `ui_console` Is Now a Real Runtime Module

Before this version, `grade-guard/header/ui_console.h` and `grade-guard/source/ui_console.c` existed only as scaffold placeholders.

In `v0.1.6`, they now own the live reusable UI/platform contract for:

- fullscreen and font sizing,
- screen clearing and raw key reads,
- header/banner rendering,
- generic selection widgets,
- generic field input,
- cursor positioning,
- color management,
- and failure display.

That matters because later modules now have one place to call for console behavior instead of re-embedding Windows console code in workflow files.

## 2) The UI Boundary Is Explicit Instead of Accidental

Two small API additions make the boundary clearer:

- `ui_clear_screen()`
- `ui_read_key()`

Those wrappers matter because they stop unrelated code from calling:

- `system("cls")`
- `getch()`

directly.

That is exactly the kind of boundary-hardening Bolt 3.1 was supposed to establish.

## 3) `main.c` Now Consumes Typed UI Enums Instead of Old Monolith Macros

The old file-local UI constants were part of the monolith.
After extraction, callers now use the public UI surface from `ui_console.h`, including:

- `UI_LIST_*`
- `UI_KEY_*`
- `UI_SELECTION_*`
- `UI_COLOR_*`

This reduces duplication and makes the header the actual source of truth for console behavior.

## 4) The Extraction Also Removed a Few UI-Layer Footguns

This version was not only a file move.
It also fixed several issues that were safe to correct inside the extracted UI layer:

- the `UI_SELECTION_BOTH` handler no longer falls through to the default error return,
- menu-label alphabet indexing now stays inside the local alphabet tables,
- and the variadic menu widget no longer carries the stray `va_start()` misuse from the monolith.

Those are small fixes, but they are the right kind of fixes to make while formalizing a shared module.

## 5) The Build Story Changed, So the Root Docs Had to Change

The normal application build now links:

- `grade-guard/main.c`
- `grade-guard/source/vector.c`
- `grade-guard/source/models.c`
- `grade-guard/source/ui_console.c`

That is why `README.md` and `CONTRIBUTING.md` both changed in this version.

Without those documentation updates, contributors would keep using stale commands and get unresolved-symbol or incomplete-link confusion.

## 6) The Acceptance Story Is Deliberately Honest

This version has strong non-interactive evidence:

- the application rebuild succeeded,
- the vector regression test still passed,
- the lifecycle regression test still passed.

But that is not the same as full Bolt 3.1 acceptance.

The remaining gap is a live Windows console run that verifies:

- menu navigation,
- field editing,
- and redraw behavior.

Because that was not executed here, `REQUIREMENTS.md` was intentionally left alone.

That is the correct release state.

## 7) `profile_controller` Still Has Not Landed Yet

It is important not to confuse Bolt 3.1 with Bolt 3.2.

This version only extracts the reusable console primitives.
The larger profile/course/activity workflow logic still lives in `main.c`.

That means the next structural step is still:

- extracting controller-style workflow functions into `profile_controller.c`,
- then reducing `main.c` further toward orchestration-only behavior.

## 8) Manual Cleanup Still Matters Before the Human Commit

Several generated artifacts are still present locally, including:

- old smoke-build output,
- older acceptance binaries,
- the new Bolt 3.1 acceptance binary,
- and the currently generated regression-test executables.

Per repository rules, they were not deleted automatically.
They are recorded in `CHANGELOG.md` under `### For Deletion` so you can remove them manually before committing.

## Validation Evidence

- Acceptance build (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/ui_console.c -o grade-guard/bolt-3-1-main.exe` succeeded.
- Vector regression compile and run (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe` succeeded, and `.\grade-guard/tests/vector_test.exe` printed `vector tests passed`.
- Lifecycle regression compile and run (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe` succeeded, and `.\grade-guard/unit-tests/models_lifecycle_test.exe` printed `models lifecycle tests passed`.
- Manual console acceptance run (2026-03-12): not executed in this session, so Bolt 3.1 remains open in `REQUIREMENTS.md`.

## Copy-Paste Commands

### Read the Bolt 3.1 design/evidence note

```powershell
Get-Content docs/unit-3-bolt-3-1-ui-console.md
```

### Rebuild the app with the extracted UI module

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/ui_console.c -o grade-guard/bolt-3-1-main.exe
```

### Re-run the shared regression checks

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe
.\grade-guard\tests\vector_test.exe

gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe
.\grade-guard\unit-tests\models_lifecycle_test.exe
```

### Run the app for the pending manual UI acceptance pass

```powershell
.\grade-guard\bolt-3-1-main.exe
```

## Pitfalls + Debugging

### 1) Unresolved `ui_*` linker errors

- Cause: `grade-guard/source/ui_console.c` was not linked in the build command.
- Fix: use the updated commands in `README.md` and `CONTRIBUTING.md`.

### 2) Console redraw behavior looks inconsistent

- Cause: host-specific console behavior around ANSI line clearing, fullscreen support, or shell-based clear-screen behavior.
- Fix: reproduce in the same Windows console host you intend to support, then verify both selection redraw and field editing manually.

### 3) It is tempting to mark Bolt 3.1 complete immediately

- Do not do that unless the live Windows console acceptance run has actually been performed.
- The non-interactive checks in this version are necessary, but they are not sufficient for the full Bolt 3.1 acceptance criteria.

## Immediate Next Actions

1. Run the live Windows console acceptance pass for Unit 3 / Bolt 3.1.
2. If that passes, update `REQUIREMENTS.md` with the dated Bolt 3.1 evidence.
3. Remove the recorded local artifacts under `### For Deletion` before making the human commit.
4. Start Unit 3 / Bolt 3.2 by extracting workflow/controller functions out of `main.c`.

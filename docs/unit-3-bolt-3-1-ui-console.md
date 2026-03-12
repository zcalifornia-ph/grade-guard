# Unit 3 Bolt 3.1: Extract Console Primitives

Objective: move the reusable Windows console helpers out of `grade-guard/main.c`, make `ui_console` the public platform boundary, and leave a traceable note for the later workflow-controller extraction.

## Executable Contract

Goal / deliverable

- Make `grade-guard/header/ui_console.h` and `grade-guard/source/ui_console.c` the source of truth for reusable console primitives.
- Remove direct Windows console API ownership from `grade-guard/main.c` so workflow code calls the UI layer instead of embedding low-level helpers.
- Document the Windows console dependencies and the current limitations that still matter before Bolt 3.2.

Bolt checklist

- Design: define a UI/platform API for screen setup, cursor movement, color, selection widgets, and field input.
- Implement: move console helper functions into dedicated modules and replace direct calls from unrelated logic.
- Test: manually verify menu navigation, field editing, and screen redraw behavior in the console.
- Docs: describe Windows-specific dependencies and any known console limitations.
- Review: confirm the UI layer does not contain grade or persistence business logic.

Acceptance criteria interpreted for this Bolt

- `main.c` no longer defines the reusable console helpers or includes raw Windows console headers.
- `ui_console.h` owns the reusable UI/platform API and remains free of domain and persistence headers.
- Generic selection, field input, cursor, color, and screen helpers now compile from `ui_console.c`.
- Shared-layer regression coverage still passes after the extraction.
- Interactive console behavior still requires a manual acceptance run before the bolt can be checked off in `REQUIREMENTS.md`.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Prior partition docs: `docs/unit-1-bolt-1-1-monolith-inventory.md` and `docs/unit-1-bolt-1-2-module-scaffold.md`
- Entry-point/workflow caller: `grade-guard/main.c`
- Public UI API: `grade-guard/header/ui_console.h`
- UI implementation: `grade-guard/source/ui_console.c`
- Shared containers and model callers that still compile after the extraction: `grade-guard/source/vector.c`, `grade-guard/source/models.c`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because the extracted UI layer must compile independently and not recreate the monolith's header coupling.
- NFR-09 matters because the point of this bolt is reducing entry-point sprawl before controller extraction starts.
- Windows-host behavior is still a risk because fullscreen, font, clear-screen, and `getch()` behavior depend on the active console host.
- Interactive regression risk remains concentrated in keyboard navigation and redraw flows, which cannot be fully proven by non-interactive test binaries alone.

## Context Summary

- `grade-guard/main.c` originally owned raw `windows.h`, `conio.h`, variadic selection rendering, cursor helpers, field input, and failure-display behavior.
- The scaffold already declared `ui_console` as the owner for cursor, color, screen mode, selection, and field-input primitives.
- Workflow code in `main.c` still needs the UI helpers, but it should consume them through `ui_console.h` rather than owning the implementation.
- `ui_console.h` depends on `vector.h` only, so the field-input widget can work without pulling in domain or persistence headers.
- `ui_show_profile_header()` and `ui_profile_login()` still belong to workflow/controller scope and were intentionally left out of this bolt.
- The extraction also created an explicit wrapper for console clearing and raw key reads so `main.c` no longer calls `system("cls")` or `getch()` directly.

## UI API Surface

The active `ui_console` boundary now owns:

- screen setup: `ui_fullscreen()`, `ui_set_font_size()`, `ui_clear_screen()`
- raw input and generic widgets: `ui_read_key()`, `ui_selection()`, `ui_selection_array()`, `ui_selection_handler()`, `ui_field_input()`
- rendering helpers: `ui_header()`, `ui_box()`, `ui_show_failure()`
- cursor and color primitives: `ui_color()`, `ui_reset_color()`, `ui_hide_caret()`, `cursor_x()`, `cursor_y()`, `gotoxy()`

Small implementation corrections made during extraction:

- fixed the `UI_SELECTION_BOTH` handler path so it no longer falls through to the default error return
- fixed the alphabet index wraparound in the selection widgets so label lookup stays inside the local alphabet tables
- removed a stray `va_start()` misuse carried over from the monolith implementation

## Windows Dependencies And Console Limitations

- The UI layer is still Windows-only because it depends on `windows.h` and `conio.h`.
- `ui_clear_screen()` still uses `system("cls")` behind the UI boundary; this is isolated now, but it still depends on a Windows shell command being available.
- `ui_fullscreen()` and `ui_set_font_size()` fail quietly if the active console host does not support those APIs or denies the operation.
- The selection widgets still use `\033[K` to clear lines during redraw, so behavior depends on the terminal host's ANSI escape handling.
- This repo still needs a real keyboard-driven manual run in a Windows console session to confirm menu navigation and redraw behavior after extraction.

## Validation Evidence

- Acceptance build (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/ui_console.c -o grade-guard/bolt-3-1-main.exe` succeeded.
- Vector regression compile and run (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/tests/vector_test.c grade-guard/source/vector.c -o grade-guard/tests/vector_test.exe` succeeded, and `.\grade-guard/tests/vector_test.exe` printed `vector tests passed`.
- Lifecycle regression compile and run (2026-03-12): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/models_lifecycle_test.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/models_lifecycle_test.exe` succeeded, and `.\grade-guard/unit-tests/models_lifecycle_test.exe` printed `models lifecycle tests passed`.
- Manual console acceptance run (2026-03-12): not executed in this non-interactive session, so the bolt remains pending manual UI verification before `REQUIREMENTS.md` should be checked off.

## Review Status

- Reviewed UI boundary: `grade-guard/source/ui_console.c` contains console primitives and generic widgets only; it does not include domain-model, grade-calculation, or persistence headers.
- Reviewed caller boundary: `grade-guard/main.c` now consumes the UI layer via `grade-guard/header/ui_console.h` instead of defining the helpers itself.
- Human validation is still required for Unit 3 UI boundaries and workflow decomposition, and manual console verification is still required for Bolt 3.1.

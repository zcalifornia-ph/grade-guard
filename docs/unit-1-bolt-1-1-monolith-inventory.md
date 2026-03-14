# Unit 1 Bolt 1.1: Monolith Inventory and Partition Map

Objective: inventory the current `grade-guard/main.c` monolith and assign every major runtime responsibility to one destination module before extraction starts.

## Executable Contract

Goal / deliverable

- Produce a responsibility map for the current monolith.
- Assign each major type, function, and responsibility cluster to exactly one owning module.
- Leave a documented partition that supports a thin `main.c` and later defect remediation.

Bolt checklist

- Design: enumerate current responsibilities in `grade-guard/main.c`.
- Implement: record the destination module for each major function and type.
- Test: verify that no major defined function is left unassigned and no module has overlapping ownership.
- Docs: leave a short map future contributors can follow during extraction.
- Review: human sign-off is still required before this Bolt can be closed.

Acceptance criteria interpreted for this Bolt

- The inventory must cover domain model, vector/utility, UI/platform, persistence, grade engine, and app flow.
- The ownership map must make `main.c` reducible to orchestration only.
- Dependencies must stay directional so later source files can compile independently.

Dependencies / interfaces touched

- Source of truth: `grade-guard/main.c`
- Upstream artifact: `REQUIREMENTS.md`
- Future consumers: Units 2 through 6

Relevant NFRs and risks for this Bolt

- NFR-04, NFR-05, and NFR-09 matter immediately because bad ownership boundaries will preserve invalid frees, compile coupling, and an oversized entry point.
- Highest current risk is the value-vs-pointer ownership mismatch around `Vector` elements and the destructors that free nested allocations.

## Context Summary

- `grade-guard/main.c` currently contains constants, type declarations, prototypes, the application loop, vector utilities, destructors, Windows console helpers, CSV persistence, grade calculations, and all profile/course/activity workflows.
- `main()` performs startup UI setup, profile-file discovery, profile loading, the top-level menu, profile lookup, and save-on-exit orchestration.
- `ui_profile_login()` is the largest controller hotspot and currently owns activity operations, course operations, parameter operations, profile settings, and grade viewing.
- `ui_show_profile_header()` mixes rendering with domain mutation by recomputing `profile->predicted_gwa` before printing it.
- `save_student_profile_to_csv()` and `load_student_profile_from_csv()` are tightly coupled to an ad hoc CSV structure and to the current in-memory layout.
- `Vector` stores elements by value, but multiple destroy paths treat retrieved elements as heap-owned pointers, which is a direct ownership/cleanup risk.
- `system("cls")`, cursor control, color handling, and key navigation are scattered through workflow code instead of being isolated behind a console layer.
- `ui_field`, `fgetline`, and `fcountlines` are declared but not defined in `main.c`; they need interface cleanup before the module scaffold solidifies.
- `isprint()` is used in `ui_field_input()` without a visible `<ctype.h>` include, which is another sign the current file boundary is hiding correctness issues.

## ADR Bullets

- Keep `vector` generic and free of domain, UI, or persistence knowledge.
- Keep all student/course/activity types and lifecycle helpers in `models`; downstream code should not free nested strings or vectors manually.
- Keep `grade_calc` pure: it calculates values and returns them, but it does not print UI or perform file I/O.
- Keep `ui_console` limited to Windows console primitives and generic text input/selection widgets.
- Keep profile/course/activity workflows in `profile_controller`; this module may call UI, models, and grade calculation, but it should not own startup file discovery.
- Keep `app` responsible for startup, shutdown, profile enumeration, top-level menu dispatch, and calling controllers. The long-term target is `main.c -> app_run()`.

## Module Ownership Charter

| Module | Owns | Must not own | Depends on |
| --- | --- | --- | --- |
| `vector` | Generic dynamic array type and operations | Student/course semantics, console code, CSV parsing | C stdlib |
| `models` | Domain structs, lifecycle helpers, ownership rules, shared grade-goal constants if needed by domain callers | Windows API calls, direct file I/O, menu loops | `vector` |
| `grade_calc` | Percentage/GWA conversion and weighted-average logic | UI printing, file access, interactive prompts | `models` |
| `ui_console` | Cursor, color, screen mode, header rendering, generic selection/input widgets | Student workflow branching, grade rules, CSV layout | Windows API, C stdlib, `vector` |
| `persistence` | Profile file naming, existence checks, CSV save/load contract | Console rendering, top-level menu logic | C stdlib, `models`, `vector` |
| `profile_controller` | Create/select/edit profile workflows, course/parameter/activity flows, profile-specific display helpers | Startup file scanning, raw Windows console primitives, CSV parsing details | `models`, `grade_calc`, `ui_console`, `vector` |
| `app` | Startup/shutdown orchestration, profile repository bootstrap, top-level menu dispatch, `main.c` replacement entry function | Low-level vector internals, raw persistence parsing, grade formulas | `persistence`, `profile_controller`, `ui_console`, `models` |

## Responsibility Map

### Constants, Types, and Declarations

| Current symbol | Current location | Destination | Notes |
| --- | --- | --- | --- |
| `NUMERIC`, `LOWS_ALPHABETICAL`, `CAPS_ALPHABETICAL`, `BULLET_POINT` | `main.c:37-40` | `ui_console.h` | Menu-presentation constants |
| Arrow/key constants and `ENTER`, `ESCAPE`, `BACK_SPACE` | `main.c:44-50` | `ui_console.h` | Input handling belongs with console helpers |
| Color constants | `main.c:54-69` | `ui_console.h` | Windows console color policy |
| `UI_VERTICAL`, `UI_HORIZONTAL`, `UI_BOTH` | `main.c:73-75` | `ui_console.h` | Selection-mode constants |
| `lower_alphabet`, `upper_alphabet` | `main.c:77-78` | `ui_console.c` | Internal presentation data for menu labels |
| `GRADE_GOALS` | `main.c:90` | `grade_calc.h` | Shared academic thresholds used by controllers |
| `Vector` | `main.c:94-99` | `vector.h` | Generic storage type |
| `Activities`, `Course_Parameter`, `Course_Component`, `Course`, `Student_Profile` | `main.c:119-156` | `models.h` | Core domain model |
| `u_long` | `main.c:117` | remove or keep inside `persistence.h` only if revived | Currently only tied to undeclared file-helper intent |
| `ui_field`, `fgetline`, `fcountlines` declarations | `main.c:176-188` | clean up in `ui_console` / `persistence` during Bolt 1.2 | Declared but not implemented; do not carry them forward blindly |

### Function-to-Module Assignment

| Function | Start line in `main.c` | Destination | Reason |
| --- | --- | --- | --- |
| `main` | `221` | `app.c` | Contains startup bootstrap, top-level menu dispatch, profile lookup, and shutdown save loop |
| `vector_create` | `339` | `vector.c` | Generic container constructor |
| `vector_destroy` | `354` | `vector.c` | Generic container teardown |
| `vector_push_back` | `359` | `vector.c` | Generic append behavior |
| `vector_pop_back` | `367` | `vector.c` | Generic pop behavior |
| `vector_remove` | `385` | `vector.c` | Generic removal behavior; current ownership semantics need documentation |
| `vector_at` | `414` | `vector.c` | Generic indexed access |
| `vector_size` | `420` | `vector.c` | Generic size query |
| `vector_resize` | `422` | `vector.c` | Generic capacity management |
| `activities_destroy` | `434` | `models.c` | Domain lifecycle helper |
| `course_parameter_destroy` | `445` | `models.c` | Domain lifecycle helper |
| `course_component_destroy` | `469` | `models.c` | Domain lifecycle helper |
| `course_destroy` | `491` | `models.c` | Domain lifecycle helper |
| `student_profile_destroy` | `506` | `models.c` | Domain lifecycle helper |
| `ui_set_font_size` | `533` | `ui_console.c` | Windows console primitive |
| `ui_fullscreen` | `560` | `ui_console.c` | Windows console primitive |
| `ui_header` | `581` | `ui_console.c` | Shared banner rendering |
| `ui_box` | `612` | `ui_console.c` | Generic UI wrapper |
| `ui_selection` | `620` | `ui_console.c` | Generic selection widget |
| `ui_selection_array` | `709` | `ui_console.c` | Generic selection widget variant |
| `ui_color` | `773` | `ui_console.c` | Windows console primitive |
| `ui_reset_color` | `781` | `ui_console.c` | Windows console primitive |
| `ui_hide_caret` | `783` | `ui_console.c` | Windows console primitive |
| `ui_selection_handler` | `795` | `ui_console.c` | Generic selection input handler |
| `save_student_profile_to_csv` | `837` | `persistence.c` | CSV serialization |
| `load_student_profile_from_csv` | `933` | `persistence.c` | CSV parsing and object reconstruction |
| `file_exists` | `1029` | `persistence.c` | File repository helper |
| `append_size_to_csv` | `1040` | `persistence.c` | Current profile-file naming policy |
| `cursor_x` | `1062` | `ui_console.c` | Windows cursor helper |
| `cursor_y` | `1072` | `ui_console.c` | Windows cursor helper |
| `gotoxy` | `1082` | `ui_console.c` | Windows cursor helper |
| `ui_field_input` | `1090` | `ui_console.c` | Generic typed-input widget |
| `input_courses` | `1151` | `profile_controller.c` | Interactive course creation workflow |
| `ui_show_failure` | `1242` | `ui_console.c` | Generic failure display primitive |
| `percentage_to_gwa` | `1258` | `grade_calc.c` | Pure grade conversion |
| `calculate_goal_percentage` | `1265` | `grade_calc.c` | Pure goal-progress calculation |
| `calculate_weighted_average` | `1285` | `grade_calc.c` | Core grade engine logic |
| `ui_show_profile_header` | `1382` | `profile_controller.c` | Profile-specific display helper; should call `grade_calc` instead of owning the computation side effect |
| `ui_profile_login` | `1397` | `profile_controller.c` | Profile/course/activity controller hotspot |
| `create_new_profile` | `2016` | `profile_controller.c` | Interactive profile creation workflow |

## Current Responsibility Clusters Inside `main()`

| Current behavior in `main()` | Current line range | Destination |
| --- | --- | --- |
| Console startup (`ui_hide_caret`, fullscreen, font sizing) | `228-230` | `app.c` calling `ui_console.c` |
| Profile repository bootstrap by probing `0.csv`, `1.csv`, ... | `232-248` | `app.c` using `persistence.c` |
| Top-level main menu | `250-331` | `app.c` |
| New-profile creation dispatch | `264-269` | `app.c` calling `profile_controller.c` |
| Profile lookup by student number | `272-304` | `app.c` with helper extraction likely needed in `profile_controller.c` or `app.c` |
| Save-all-on-exit loop | `309-316` | `app.c` using `persistence.c` |

## Thin Entry-Point Target

The ownership split above supports this target shape:

```c
int main(void)
{
    return app_run();
}
```

That keeps `main.c` below the maintainability limit while moving reusable logic into named modules.

## Validation Notes

Static checks expected for this Bolt

- Every defined top-level function in `main.c` appears in the function-assignment table above.
- Each responsibility cluster has one owning module only.
- UI/platform ownership is separated from persistence and grade calculations.
- Domain lifecycle helpers are separated from generic vector behavior.

Manual findings to carry into later Bolts

- `Vector` currently copies structs by value, but several destructors free those structs as if each vector element were heap allocated. Unit 2 must resolve this before broad extraction.
- `ui_profile_login()` edits `Course_Component component` by value, which leaks aliasing assumptions through nested vector pointers.
- `load_student_profile_from_csv()` depends on blank-line and section-order sentinels that are not yet formalized as a contract.
- `ui_show_profile_header()` mutates profile state while rendering; later extraction should remove that side effect.

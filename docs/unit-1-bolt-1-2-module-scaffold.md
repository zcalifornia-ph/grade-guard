# Unit 1 Bolt 1.2: Public Interface and File Layout Scaffold

Objective: define the first stable `grade-guard/header/` and `grade-guard/source/` scaffold so later Units can move implementation out of `grade-guard/main.c` without re-deciding module boundaries.

## Executable Contract

Goal / deliverable

- Create the agreed `header/` and `source/` module tree from the Unit 1 plan.
- Publish the first header inventory with include guards, ownership boundaries, and dependency direction.
- Leave compile-safe placeholders so each new implementation file can be built independently before heavy extraction starts.

Bolt checklist

- Design: specify the initial headers, sources, include guards, and shared constants strategy.
- Implement: create the scaffold under `grade-guard/header/` and `grade-guard/source/`.
- Test: compile each new source file with the new headers.
- Docs: record the module list, ownership rules, and validation evidence.
- Review: human sign-off is still required before the Bolt can be closed.

Acceptance criteria interpreted for this Bolt

- The target module map must exist on disk and match the Unit 1 design baseline.
- Public headers must compile cleanly with their owning source file and documented dependency direction.
- Constants shared across modules must have one owning header instead of remaining scattered in `main.c`.
- `main.c` remains untouched as the current runtime source of truth while the scaffold is proven.

Dependencies / interfaces touched

- Source of truth: `grade-guard/main.c`
- Prior planning artifact: `docs/unit-1-bolt-1-1-monolith-inventory.md`
- Upstream contract: `REQUIREMENTS.md`
- New scaffold: `grade-guard/header/*.h` and `grade-guard/source/*.c`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because bad header layering would recreate the current compile coupling in multiple files.
- NFR-05 matters because ownership-facing APIs must be named before lifecycle fixes begin in Unit 2.
- NFR-09 matters because the scaffold must support a thin `main.c` target instead of preserving another monolith.
- The biggest current risk is locking in the wrong ownership semantics before Unit 2 resolves value-versus-pointer cleanup.

## Context Summary

- `grade-guard/main.c` is still the only runtime implementation file and remains the behavioral baseline.
- Unit 1 Bolt 1.1 already mapped every major function and type into one destination module with no overlap.
- The proposed target module list in `REQUIREMENTS.md` already names seven public modules plus the thin entry point.
- Undefined declarations in `main.c` (`ui_field`, `fgetline`, `fcountlines`) should not be copied into the new public API until they are justified.
- Windows console helpers must stay separate from domain, persistence, and grade logic.
- `Vector` is the lowest shared dependency and therefore anchors the include graph.
- The scaffold needs compile-safe placeholders so file boundaries can be validated without mixing extraction and bug fixes into one step.

## Design Decisions

### File Layout

- `grade-guard/header/app.h`
- `grade-guard/header/vector.h`
- `grade-guard/header/models.h`
- `grade-guard/header/grade_calc.h`
- `grade-guard/header/persistence.h`
- `grade-guard/header/ui_console.h`
- `grade-guard/header/profile_controller.h`
- `grade-guard/source/app.c`
- `grade-guard/source/vector.c`
- `grade-guard/source/models.c`
- `grade-guard/source/grade_calc.c`
- `grade-guard/source/persistence.c`
- `grade-guard/source/ui_console.c`
- `grade-guard/source/profile_controller.c`

### Include Guard Convention

- Every public header uses `GRADE_GUARD_HEADER_<MODULE>_H`.
- No `#pragma once` is used at this stage so the project stays compatible with conservative C toolchains.

### Header Dependency Rules

- `vector.h` depends only on the C standard library.
- `models.h` may depend on `vector.h`, but `vector.h` must never depend on domain headers.
- `grade_calc.h`, `persistence.h`, and `profile_controller.h` depend on `models.h`.
- `ui_console.h` depends on `vector.h` only; it must not include domain or persistence headers.
- `app.h` is the top-level orchestrator interface and should not be included by other module headers.
- Source files include only their own public header during scaffold stage to prove independent compilation.

### Shared Constants Strategy

- UI-facing constants now belong to `ui_console.h` as typed enums for list modes, selection axes, key codes, and colors.
- Grade-goal thresholds belong to `grade_calc.h` as `GRADE_GOALS` plus `GRADE_GUARD_GRADE_GOAL_COUNT`.
- No catch-all `common.h` is introduced in Unit 1; constants stay in the header owned by the module that uses and defines them.
- Implementation-only constants should stay `static` in source files once real logic starts moving.

## Public Header Inventory

| Header | Purpose | Public ownership |
| --- | --- | --- |
| `app.h` | Entry-point orchestration contract | Startup/shutdown flow only |
| `vector.h` | Generic dynamic array API | Reusable storage, no domain knowledge |
| `models.h` | Domain structs and lifecycle helpers | Student/course/activity data |
| `grade_calc.h` | Grade conversion and weighted-average API | Pure grade computations and shared goal thresholds |
| `persistence.h` | Save/load and file naming contract | CSV persistence and profile repository helpers |
| `ui_console.h` | Windows console and generic input/output primitives | Screen, color, cursor, and selection helpers |
| `profile_controller.h` | Profile/course/activity workflow API | Interactive profile-level flows |

## Ownership Rules

| Module | Must own | Must not own |
| --- | --- | --- |
| `vector` | Generic container behavior | Domain cleanup policy, console behavior, CSV details |
| `models` | Domain types and lifecycle helpers | Windows API calls, menu loops, file parsing |
| `grade_calc` | Percentage and GWA math | UI rendering, file access, profile persistence |
| `persistence` | Save/load contract and filename helpers | Console drawing, grade formulas, menu control |
| `ui_console` | Windows console primitives and generic widgets | Student workflows, CSV parsing, domain mutation |
| `profile_controller` | Profile workflows and course/activity interactions | Startup repository bootstrap, raw console internals |
| `app` | Top-level orchestration and eventual `main.c` replacement | Low-level vector internals and CSV parsing details |

## Validation Evidence

Smoke-test command used for this scaffold:

```powershell
$buildDir = "grade-guard\\build-smoke"
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
Get-ChildItem grade-guard\\source\\*.c | ForEach-Object {
    $output = Join-Path $buildDir ($_.BaseName + ".o")
    gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header -c $_.FullName -o $output
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}
```

Result (2026-03-12): passed with `gcc.exe (tdm64-1) 10.3.0`; object files were produced for `app`, `vector`, `models`, `grade_calc`, `persistence`, `ui_console`, and `profile_controller` under `grade-guard/build-smoke/`.

## Review Status

- Human review is still required for Unit 1 scope, module boundaries, and scaffold stability before Bolt 1.2 can be checked off.
